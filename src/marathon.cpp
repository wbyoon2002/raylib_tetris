#include "marathon.h"

Pages::Marathon::Marathon(int initLevel)
    : score(0), initLevel(initLevel), level(initLevel), bonusPoints(level) {
    std::string assetsBasePath = ASSETS_PATH;
    std::string musicPath = assetsBasePath + "/Sounds/music.mp3";
    music = LoadMusicStream(musicPath.c_str());
    PlayMusicStream(music);
    if (initLevel == 0) {
        this->initLevel = 1;
        this->level = 1;
    }
    lastLevelUpTime = GetTime();
    UpdateDropPeriod();
    actionMultipliers = {100, 300, 500, 800, 400, 800, 1200, 1600, 100, 200, 400};
}

Pages::Marathon::~Marathon() {
    UnloadMusicStream(music);
}

void Pages::Marathon::Draw() {
    UpdateMusicStream(music);
    Game::Draw();
    // Display high score
    DrawTextEx(font, "High Score", {offsetX + 500, offsetY + 620 - fontSize * 2 - 15}, fontSize, 2, WHITE);
    char highscoreText[10];

    int high_score = 10000;
    sprintf(highscoreText, "%d", high_score);
    DrawTextEx(font, highscoreText, {offsetX + 500, offsetY + 620 - fontSize - 10}, fontSize, 2, WHITE);

    // Display current score
    char scoreText[10];
    sprintf(scoreText, "%d", score);
    DrawTextEx(font, "Score", {offsetX + 500, offsetY + 620 - fontSize * 4 - 25}, fontSize, 2, WHITE);
    DrawTextEx(font, scoreText, {offsetX + 500, offsetY + 620 - fontSize * 3 - 20}, fontSize, 2, WHITE);

    // Display the current level
    DrawTextEx(font, "Level", {
                   offsetX + 180 - MeasureTextEx(font, "Level", fontSize, 2).x, offsetY + 620 - fontSize * 4 - 25
               }, fontSize, 2, WHITE);
    char levelText[4];
    sprintf(levelText, "%d", level);
    DrawTextEx(font, levelText, {
                   offsetX + 180 - MeasureTextEx(font, levelText, fontSize, 2).x, offsetY + 620 - fontSize * 3 - 20
               }, fontSize, 2, WHITE);

    // Display "Level Up"
    if (LevelUpTriggered()) {
        DrawTextEx(font, "Level Up!", {
                       offsetX + 180 - MeasureTextEx(font, "Level Up!", fontSize, 2).x,
                       offsetY + 620 - fontSize * 5 - 30
                   }, fontSize, 2, WHITE);
    }

    if (ActionTriggered()) {
        char bonusText[10];
        sprintf(bonusText, "+%d", bonusPoints);
        DrawTextEx(font, bonusText, {
                       offsetX + 500, offsetY + 620 - fontSize * 5 - 30
                   }, fontSize, 2, WHITE);
    }

    // Display "GAME OVER" under the "Next" box
    if (gameOver) {
        DrawTextEx(font, "GAME OVER", {offsetX + 500, offsetY + 340}, fontSize, 2, WHITE);
    }

    // Display the number of cleared lines
    DrawTextEx(font, "Lines", {
                   offsetX + 180 - MeasureTextEx(font, "Lines", fontSize, 2).x, offsetY + 620 - fontSize * 2 - 15
               }, fontSize, 2, WHITE);
    char lineText[5];
    sprintf(lineText, "%d", totalRowsCleared);
    DrawTextEx(font, lineText, {
                   offsetX + 180 - MeasureTextEx(font, lineText, fontSize, 2).x, offsetY + 620 - fontSize - 10
               }, fontSize, 2, WHITE);
    if (hasPaused) {
        DrawPausedScreen();
    }
}

void Pages::Marathon::HandleInput() {
    ReadInput();
    Game::HandleInput();
    if (gameOverTrigger) {
        StopMusicStream(music);
    }
    if (gameOver && keyPressed != 0) {      // when a key is pressed after the game is over
        // exit the game when the esc key is pressed
        if (keyPressed == KEY_ESCAPE) {
            exitMode = true;
            return;
        }
        // reset the game when the enter key is pressed
        else if (keyPressed == KEY_ENTER) {
            Game::Reset();
            PlayMusicStream(music);
        }
    }
    else if (hasPaused && keyPressed != 0) {
        if (Game::keyPressed == KEY_UP) {
            pauseMenuSelection = (pauseMenuSelection - 1 + 2) % 2;
        }
        else if (Game::keyPressed == KEY_DOWN) {
            pauseMenuSelection = (pauseMenuSelection + 1) % 2;
        }
        else if (Game::keyPressed == KEY_ENTER) {
            if (pauseMenuSelection == 0) {
                Resume();
                ResumeMusicStream(music);
            }
            else if (pauseMenuSelection == 1) {
                // Exit game
                exitMode = true;
                return;
            }
        }
        // exit the game when the esc key is pressed
        if (keyPressed == KEY_ESCAPE) {
            exitMode = true;
            return;
        }
    }
    else if (!(gameOver || hasPaused)) {
        if (keyPressed == softDropKey[keyBoardLayout]) {
            // increase the score by 1 if the block can move down (soft drop)
            UpdateScore(hardDropDistance > 0 ? 1 : 0);
        }
        else if (keyPressed == hardDropKey[keyBoardLayout]) {
            UpdateScore(2 * hardDropDistance);
        }
        else if (keyPressed == KEY_ESCAPE) {
            Pause();
            PauseMusicStream(music);
        }
        if (softDropTrigger) {
            UpdateScore(softDropDistance);
        }
    }
    if (actionTrigger) {
        UpdateBonusPoints();
    }
    if (resetTrigger) {
        Reset();
    }
}

void Pages::Marathon::UpdateScore(int points) {
    score += points;
}

void Pages::Marathon::Reset() {
    score = 0;
    level = initLevel;
    UpdateDropPeriod();
}

void Pages::Marathon::UpdateBonusPoints() {
    // bonusPoints are a multiple of level according to the Tetris guideline
    bonusPoints = level;
    // calculate the bonus points according to the action type
    bonusPoints *= actionMultipliers[actionType];
    if (rowsCleared > 0) {
        if (isBacktoBack) {
            bonusPoints *= 1.5;
        }
        if (isCombo) {
            bonusPoints += 50 * combo * level;
        }
        // increase the level for a certain amount of lines cleared
        if (totalRowsCleared >= level * 10) {
            level = totalRowsCleared / 10 + 1;
            lastLevelUpTime = GetTime();
            UpdateDropPeriod();
        }
    }
    UpdateScore(bonusPoints);
}

bool Pages::Marathon::LevelUpTriggered() {
    double currentTime = GetTime();
    if (currentTime - lastLevelUpTime <= 1.0 && level != initLevel) {
        return true;
    }
    return false;
}

void Pages::Marathon::UpdateDropPeriod() {
    double temp = 0.8 - 0.007 * (level - 1);
    dropPeriod = 1.0;
    for (int i = 0; i < level - 1; i++) {
        dropPeriod *= temp;
    }
}
