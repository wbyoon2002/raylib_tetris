#include "game.h"
#include "marathon.h"
#include "sprint.h"
#include "startpage.h"
#include "ranking.h"
#include "localpvp.h"
#include "interface.h"

int main(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Raylib Tetris");
    SetTargetFPS(TARGET_FPS);
    InitAudioDevice();
    // Don't close the window when the exit key is pressed
    SetExitKey(0);

    Mode* mode = new Pages::StartPage;
    while (WindowShouldClose() == false) {    // Keep opening the window until the 'esc' key is pressed or the window is manually closed
        mode->HandleInput();
        if (mode->exitMode) {
            int nextMode = mode->nextMode;
            delete mode;
            if (nextMode == -1) {
                // Quit the game
                CloseAudioDevice();
                CloseWindow();
                return 0;
            }

            if (nextMode == 0) {
                // Return to the start page
                mode = new Pages::StartPage;
            }
            else if (nextMode < 5) {
                // Start the marathon game
                mode = new Pages::Marathon((nextMode - 1) * 5);
            }
            else if (nextMode == 5) {
                // Start the sprint game
                mode = new Pages::Sprint;
            }
            else if (nextMode == 6) {
                // Draw the ranking page
                mode = new Pages::Ranking;
            }
            else if (nextMode == 7) {
                // Start a local PvP game
                mode = new Pages::LocalPvP;
            }
        }
        BeginDrawing();
        mode->Draw();
        EndDrawing();
    }

    delete mode;
    CloseAudioDevice();
    CloseWindow();
}