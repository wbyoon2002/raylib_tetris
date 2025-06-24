#include "localpvp.h"

Pages::LocalPvP::LocalPvP() {
    std::string assetsBasePath = ASSETS_PATH;
    std::string musicPath = assetsBasePath + "/Sounds/music.mp3";
    music = LoadMusicStream(musicPath.c_str());
    PlayMusicStream(music);
    players = new Player*[2];
    players[0] = new Player(0, 90, 1, "Player 1");
    players[1] = new Player(710, 90, 2, "Player 2");
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
    if (Game::keyPressed == KEY_ESCAPE) {
        exitMode = true;
    }
    if (players[0]->HasLost() && !players[1]->HasLost()) {
        StopMusicStream(music);
        players[1]->Stop();
        winnerIndex = 1;
    }
    if (players[1]->HasLost() && !players[0]->HasLost()) {
        StopMusicStream(music);
        players[0]->Stop();
        winnerIndex = 0;
    }
    if (players[0]->HasLost() && players[1]->HasLost() && Game::keyPressed == KEY_ENTER) {
        // reset the game when the enter key is pressed
        PlayMusicStream(music);
        players[0]->Restart();
        players[1]->Restart();
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
