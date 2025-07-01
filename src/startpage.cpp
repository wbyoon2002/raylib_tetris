#include "startpage.h"
#include "colors.h"
#include "interface.h"

// Initialize with the first option selected
Pages::StartPage::StartPage()
    : selectedOption(0), isMarathon(false) {
    menuOptions = {"Marathon", "Sprint", "Multiplay", "View Rankings", "Quit"};

    // Load the logo texture
    std::string logoPath = std::string(ASSETS_PATH) + "/Icons/Tetris_logo.png";
    logoTexture = LoadTexture(logoPath.c_str());
}

Pages::StartPage::~StartPage() {
    UnloadTexture(logoTexture);
}

void Pages::StartPage::Draw() {
    ClearBackground(darkBlue);
    DrawHeader();
    DrawMenu();
}

void Pages::StartPage::DrawHeader() {
    if (isMarathon) {
        // Draw the title text for marathon mode
        float titleFontSize = 75;
        Vector2 titleSize = MeasureTextEx(font, title.c_str(), titleFontSize, 2);
        Vector2 titlePos = {(GetScreenWidth() - titleSize.x) / 2, GetScreenHeight() / 2 - titleSize.y - 20};
        DrawTextEx(font, title.c_str(), titlePos, titleFontSize, 2, WHITE);
    }
    else {
        // Draw the logo
        float logoScale = 0.25f;  // Adjust this to make the logo bigger or smaller
        Vector2 logoPos = {(GetScreenWidth() - logoTexture.width * logoScale) / 2, GetScreenHeight() / 2 - logoTexture.height * logoScale - 20};
        DrawTextureEx(logoTexture, logoPos, 0.0f, logoScale, WHITE);
    }
    headerEndPos = {(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 - 20};
}

void Pages::StartPage::DrawMenu() {
    float menuFontSize = 38;
    float menuSpacing = 50;
    Vector2 menuStartPos = {(float)GetScreenWidth() / 2, headerEndPos.y + 40};

    for (int i = 0; i < menuOptions.size(); i++) {
        Vector2 textSize = MeasureTextEx(font, menuOptions[i].c_str(), menuFontSize, 2);
        Vector2 textPos = {menuStartPos.x - textSize.x / 2, menuStartPos.y + i * menuSpacing};
        Color textColor = (i == selectedOption) ? YELLOW : WHITE;
        DrawTextEx(font, menuOptions[i].c_str(), textPos, menuFontSize, 2, textColor);
    }
}

void Pages::StartPage::HandleInput() {
    HandleMenuInput();
}

void Pages::StartPage::HandleMenuInput() {
    int keyPressed = GetKeyPressed();

    // Handle menu navigation
    if (keyPressed == KEY_DOWN) selectedOption = (selectedOption + 1) % menuOptions.size();
    if (keyPressed == KEY_UP) selectedOption = (selectedOption - 1 + menuOptions.size()) % menuOptions.size();

    if (keyPressed == KEY_ENTER) {
        exitMode = true;

        if (!isMarathon) {
            switch (selectedOption) {
                case 0: title = "Marathon"; menuOptions = {"Easy(Level 1)", "Medium(Level 5)", "Hard(Level 10)", "Extreme(Level 15)", "Back"}; isMarathon = true; selectedOption = 0; exitMode = false; break;  // Marathon
                case 1: nextMode = 5; break; // Sprint
                case 2: nextMode = 7; break;    // Multiplayer
                case 3: nextMode = 6; break;  // View Rankings
                case 4: nextMode = -1; break;  // Quit
            }
        } else if (isMarathon) {
            if (selectedOption == 4) ResetToMainMenu();
            else nextMode = selectedOption + 1;
        }
    }
}

void Pages::StartPage::ResetToMainMenu() {
    title = "Tetris";
    menuOptions = {"Marathon", "Sprint", "Multiplay", "View Rankings", "Quit"};
    selectedOption = 0;
    isMarathon = false;
    exitMode = false;
}
