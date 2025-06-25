#include "ranking.h"
#include "colors.h"

void Pages::Ranking::Draw() {
    ClearBackground(darkBlue);
}

void Pages::Ranking::HandleInput() {
    int keyPressed = GetKeyPressed();
    if (keyPressed == KEY_ESCAPE) {
        exitMode = true;
    }
}