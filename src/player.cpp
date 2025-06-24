#include "player.h"

Player::Player(float x, float y, int layout, std::string name)
: Game(x, y, layout), name(name), attackTrigger(false), incomeTrigger(false), incomeLines(0) {
    garbageLines = {0, 1, 2, 4, 0, 2, 4, 6, 0, 0, 1};
}

void Player::Draw() {
    DrawTextEx(font, name.c_str(), {offsetX + 355 - MeasureTextEx(font, name.c_str(), fontSize, 2).x / 2, 90 - fontSize}, fontSize, 2, WHITE);
    Game::Draw();
    double currentTime = GetTime();
    if (incomeTrigger) {
        if (currentTime - incomeTime < 0.5) {
            float rectHeight = 150.0 * (1.0 - (currentTime - incomeTime) * 2);
            Rectangle countdownRect = {offsetX + 175, offsetY + 611 - rectHeight, 10, rectHeight};
            DrawRectangleRec(countdownRect, RED);
        }
        else {
            std::string warning = std::to_string(incomeLines) + " garbage lines incoming";
            DrawTextEx(font, warning.c_str(), {offsetX + 355 - MeasureTextEx(font, warning.c_str(), fontSize, 2).x / 2, 90 - fontSize * 2 - 5}, fontSize, 2, RED);
        }
    }
}

void Player::HandleInput() {
    float currentTime = GetTime();
    attackTrigger = false;
    Game::HandleInput();
    if (actionTrigger && garbageLines[actionType] != 0) {
        attackLines = garbageLines[actionType];
        if (isBacktoBack) {
            if (actionType == MTSS || actionType == MTSD || actionType == TSS) {
                attackLines += 1;
            }
            else if (actionType == TSD || actionType == TETRIS) {
                attackLines += 2;
            }
            else if (actionType == TST) {
                attackLines += 3;
            }
        }
        if (incomeTrigger && (currentTime - incomeTime < 0.5)) {
            // initiate counterattack/elimination of incoming garbage lines
            attackLines -= incomeLines;
        }
        if (attackLines >= 0) {
            // resolve garbage income if all the incoming garbage lines are eliminated
            incomeTrigger = false;
            incomeLines = 0;
            if (attackLines > 0) {
                attackTrigger = true;
            }
        }
        else {
            incomeLines = -attackLines;
        }
    }
    if (incomeTrigger && (currentTime - incomeTime > 0.5) && lockBlockTrigger) {
        if (grid->CreateGarbageRows(incomeLines)) {
            Game::PlaceGhostBlock();
            incomeTrigger = false;
            incomeLines = 0;
        }
        else {
            gameOver = true;
            gameOverTrigger = true;
        }
    }
}

bool Player::HasLost()
{
    return gameOver;
}

void Player::Stop() {
    gameOver = true;
    gameOverTrigger = true;
}

void Player::Restart() {
    attackTrigger = false;
    incomeTrigger = false;
    incomeLines = 0;
    Game::Reset();
}

std::string Player::getName() {
    return name;
}

void Player::HandleAttack(const Player& other) {
    if (other.attackTrigger) {
        incomeTrigger = true;
        incomeTime = GetTime();
        incomeLines += other.attackLines;
    }
}