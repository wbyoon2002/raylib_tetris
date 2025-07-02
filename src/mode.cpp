#include "mode.h"
#include <string>
#include <vector>

Mode::Mode(): exitMode(false), nextMode(0), hasPaused(false), pauseMenuSelection(0) {
    std::string assetsBasePath = ASSETS_PATH;
    std::string fontPath = assetsBasePath + "/Font/monogram.ttf";
    font = LoadFontEx(fontPath.c_str(), 64, 0, 0);
}

void Mode::DrawPausedScreen() {
    // Draw semi-transparent overlay
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));

    // Menu options
    std::vector<std::string> menuOptions = {"Resume", "Restart", "Exit to Menu"};
    
    // Calculate menu dimensions
    int menuFontSize = 28;
    int menuSpacing = 40;
    int menuPadding = 10;
    
    // Find the widest menu option for popup sizing
    int maxTextWidth = 0;
    for (const auto& option : menuOptions) {
        Vector2 textSize = MeasureTextEx(font, option.c_str(), menuFontSize, 1);
        if (textSize.x > maxTextWidth) {
            maxTextWidth = textSize.x;
        }
    }
    
    // Calculate popup dimensions
    int popupWidth = maxTextWidth + menuPadding * 2;
    int popupHeight = menuOptions.size() * menuSpacing + menuPadding * 2;
    int popupX = GetScreenWidth() / 2 - popupWidth / 2;
    int popupY = GetScreenHeight() / 2 - popupHeight / 2;
    
    // Draw popup background
    Rectangle popupRect = {(float)popupX, (float)popupY, (float)popupWidth, (float)popupHeight};
    DrawRectangleRec(popupRect, DARKGRAY);
    
    // Draw menu options
    for (int i = 0; i < menuOptions.size(); i++) {
        Vector2 textSize = MeasureTextEx(font, menuOptions[i].c_str(), menuFontSize, 1);
        Vector2 textPos = {
            (float)(GetScreenWidth() / 2 - textSize.x / 2),
            (float)(popupY + menuPadding + i * menuSpacing)
        };
        
        Color textColor = (i == pauseMenuSelection) ? YELLOW : WHITE;
        DrawTextEx(font, menuOptions[i].c_str(), textPos, menuFontSize, 1, textColor);
    }
}