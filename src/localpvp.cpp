#include "localpvp.h"

Pages::LocalPvP::LocalPvP() {
    std::string assetsBasePath = ASSETS_PATH;
    std::string musicPath = assetsBasePath + "/Sounds/music.mp3";
    music = LoadMusicStream(musicPath.c_str());
    PlayMusicStream(music);
    players = new Player*[2];
    players[0] = new Player(0, 90, 1, "Player 1");
    players[1] = new Player(710, 90, 2, "Player 2");
    pauseMenuSelection = 0;
}

Pages::LocalPvP::~LocalPvP() {
    UnloadMusicStream(music);
    for (int i = 0; i < 2; i++) {
        delete players[i];
    }
    delete[] players;
}

void Pages::LocalPvP::HandleInput() {
    Game::ReadInput();
    for (int i = 0; i < 2; i++) {
        players[i]->HandleInput();
        players[i]->HandleAttack(*players[1 - i]);
    }
    if (players[0]->HasLost() && !players[1]->HasLost()) {
        StopMusicStream(music);
        players[1]->Stop();
        winnerIndex = 1;
    }
    else if (players[1]->HasLost() && !players[0]->HasLost()) {
        StopMusicStream(music);
        players[0]->Stop();
        winnerIndex = 0;
    }
    else if (players[0]->HasLost() && players[1]->HasLost()) {
        if (Game::keyPressed == KEY_ENTER) {
            Reset();
        }
        else if (Game::keyPressed == KEY_ESCAPE) {
            exitMode = true;
            return;
        }
    }
    else if (!hasPaused && Game::keyPressed == KEY_ESCAPE) {
        for (int i = 0; i < 2; i++) {
            players[i]->Pause();
        }
        hasPaused = true;
        PauseMusicStream(music);
    }
    else if (hasPaused) {
        if (Game::keyPressed == KEY_UP) {
            pauseMenuSelection = (pauseMenuSelection - 1 + 3) % 3;
        }
        else if (Game::keyPressed == KEY_DOWN) {
            pauseMenuSelection = (pauseMenuSelection + 1) % 3;
        }
        else if (Game::keyPressed == KEY_ENTER) {
            if (pauseMenuSelection == 0) {
                // Resume game
                for (int i = 0; i < 2; i++) {
                    players[i]->Resume();
                }
                hasPaused = false;
                ResumeMusicStream(music);
            }
            else if (pauseMenuSelection == 1) {
                hasPaused = false;
                pauseMenuSelection = 0;
                StopMusicStream(music);
                Reset();
            }
             else if (pauseMenuSelection == 2) {
                // Exit game
                exitMode = true;
                return;
            }
        }
        else if (Game::keyPressed == KEY_ESCAPE) {
            exitMode = true;
            return;
        }
    }
}

void Pages::LocalPvP::Draw() {
    UpdateMusicStream(music);
    for (int i = 0; i < 2; i++) {
        players[i]->Draw();
    }
    if (players[0]->HasLost() || players[1]->HasLost()) {
        std::string title = players[winnerIndex]->getName() + " has won the game!";
        DrawPopup(title);
    }
    if (hasPaused) {
        DrawPausedScreen();
    }
}

void Pages::LocalPvP::DrawPopup(const std::string& title) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));

    int titleFontSize = 32;
    Vector2 titleSize = MeasureTextEx(font, title.c_str(), titleFontSize, 1);
    
    int popupWidth = titleSize.x + 30;
    int popupHeight = 60;
    int popupX = GetScreenWidth() / 2 - popupWidth / 2;
    int popupY = GetScreenHeight() / 2 - popupHeight / 2;
    
    Rectangle popupRect = {(float)popupX, (float)popupY, (float)popupWidth, (float)popupHeight};
    DrawRectangleRec(popupRect, DARKGRAY);

    DrawTextEx(font, title.c_str(), {(float)(GetScreenWidth() / 2 - titleSize.x / 2), (float)(GetScreenHeight() / 2 - titleSize.y / 2)}, titleFontSize, 1, WHITE);
}

void Pages::LocalPvP::Reset() {
    PlayMusicStream(music);
    players[0]->Restart();
    players[1]->Restart();
}