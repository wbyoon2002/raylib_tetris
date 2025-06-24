#include "startpage.h"
#include "colors.h"
#include "interface.h"
#include "user.h"

// Initialize with the first option selected
Pages::StartPage::StartPage()
    : selectedOption(0), isMarathon(false),
      showModePopup(false), showLevelPopup(false), popupSelected(0), headerEndPos() {
    title = "Tetris";
    menuOptions = {"Marathon", "Sprint", "Multiplay", "View Rankings", "Quit"};
    user.load_user();

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

    if (user.get_nickname().empty()) {
        DrawNicknameInput();
        return;
    }

    DrawPopups();
}

void Pages::StartPage::DrawHeader() {
    if (isMarathon) {
        // Draw the title text for marathon mode
        float titleFontSize = 75;
        Vector2 titleSize = MeasureTextEx(font, title.c_str(), titleFontSize, 2);
        Vector2 titlePos = {(WINDOW_WIDTH - titleSize.x) / 2, WINDOW_HEIGHT / 4};
        DrawTextEx(font, title.c_str(), titlePos, titleFontSize, 2, WHITE);
        headerEndPos = {titlePos.x, titlePos.y + titleSize.y};
    }
    else {
        // Draw the logo
        float logoScale = 0.25f;  // Adjust this to make the logo bigger or smaller
        Vector2 logoPos = {(WINDOW_WIDTH - logoTexture.width * logoScale) / 2, WINDOW_HEIGHT / 6};
        DrawTextureEx(logoTexture, logoPos, 0.0f, logoScale, WHITE);

        // Draw attribution text beneath the logo
        std::string attribution = "modified by Wonbeen and Jaeuk";
        float attrFontSize = 24;
        Vector2 attrSize = MeasureTextEx(font, attribution.c_str(), attrFontSize, 2);
        Vector2 attrPos = {(WINDOW_WIDTH - attrSize.x) / 2, logoPos.y + logoTexture.height * logoScale + 20};
        DrawTextEx(font, attribution.c_str(), attrPos, attrFontSize, 2, WHITE);
        headerEndPos = {attrPos.x, attrPos.y + attrSize.y};
    }

    if (!showModePopup && !showLevelPopup && !user.get_nickname().empty()) {
        DrawMenu();
    }
}

void Pages::StartPage::DrawNicknameInput() {
    Rectangle inputBox = {WINDOW_WIDTH / 2 - 200, headerEndPos.y + 80, 400, 60};

    DrawRectangleRec(inputBox, DARKGRAY);
    DrawRectangleLinesEx(inputBox, 3, GRAY);

    std::string prompt = input_nickname.empty() ? " " : input_nickname;
    prompt += ((int)(GetTime() * 1.5) % 2 == 0) ? "_" : "";
    Vector2 textSize = MeasureTextEx(font, prompt.c_str(), 38, 2);
    Vector2 textPos = {inputBox.x + (inputBox.width - textSize.x) / 2, inputBox.y + (inputBox.height - textSize.y) / 2};
    DrawTextEx(font, prompt.c_str(), textPos, 38, 2, WHITE);

    if (!input_nickname.empty()) {
        float alpha = 0.5f * (sinf(GetTime() * 2) + 1.0f);
        unsigned char a = (unsigned char)(alpha * 255);
        Color fadeColor = {200, 200, 200, a};

        std::string hint = "Press ENTER to continue";
        Vector2 hintSize = MeasureTextEx(font, hint.c_str(), 20, 2);
        Vector2 hintPos = {(WINDOW_WIDTH - hintSize.x) / 2, inputBox.y + inputBox.height + 20};
        DrawTextEx(font, hint.c_str(), hintPos, 20, 2, fadeColor);
    }

    if (input_nickname.length() >= 12) {
        std::string warning = "Nickname too long!";
        Vector2 warnSize = MeasureTextEx(font, warning.c_str(), 18, 2);
        Vector2 warnPos = {(WINDOW_WIDTH - warnSize.x) / 2, inputBox.y - 30};
        DrawTextEx(font, warning.c_str(), warnPos, 18, 2, RED);
    }
}

void Pages::StartPage::DrawMenu() {
    float menuFontSize = 38;
    float menuSpacing = 50;
    Vector2 menuStartPos = {WINDOW_WIDTH / 2, headerEndPos.y + 40};

    for (int i = 0; i < menuOptions.size(); i++) {
        Vector2 textSize = MeasureTextEx(font, menuOptions[i].c_str(), menuFontSize, 2);
        Vector2 textPos = {menuStartPos.x - textSize.x / 2, menuStartPos.y + i * menuSpacing};
        Color textColor = (i == selectedOption) ? YELLOW : WHITE;
        DrawTextEx(font, menuOptions[i].c_str(), textPos, menuFontSize, 2, textColor);

        // Draw triangle next to selected option
        if (i == selectedOption) {
            Vector2 p1 = {textPos.x - 40, textPos.y + textSize.y / 2}; // Triangle point
            Vector2 p2 = {p1.x - 15, p1.y - 10}; // Top vertex
            Vector2 p3 = {p1.x - 15, p1.y + 10};
            DrawTriangle(p1, p2, p3, WHITE); // Bottom vertex
        }
    }
}

void Pages::StartPage::DrawPopup(const std::vector<std::string>& options, const std::string& title) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));

    int popupWidth = 400;
    int popupHeight = 60 + (int)options.size() * 60;
    int popupX = GetScreenWidth() / 2 - popupWidth / 2;
    int popupY = GetScreenHeight() / 2 - popupHeight / 2;

    Rectangle popupRect = {(float)popupX, (float)popupY, (float)popupWidth, (float)popupHeight};

    DrawRectangleRec(popupRect, DARKGRAY);
    DrawRectangleLinesEx(popupRect, 3, GRAY);

    int titleFontSize = 32;
    Vector2 titleSize = MeasureTextEx(font, title.c_str(), titleFontSize, 1);
    DrawTextEx(font, title.c_str(), {(float)(GetScreenWidth() / 2 - titleSize.x / 2), (float)(popupY + 20)}, titleFontSize, 1, WHITE);

    int optionFontSize = 26;
    int optionHeight = 40;
    int spacing = 15;
    int startY = popupY + 70;

    for (int i = 0; i < options.size(); i++) {
        Rectangle btnRect = {popupRect.x + 40, (float)(startY + i * (optionHeight + spacing)), popupRect.width - 80, (float)optionHeight};
        bool isSelected = (i == popupSelected);

        Color bgColor = isSelected ? Fade(SKYBLUE, 0.8f) : Fade(LIGHTGRAY, 0.3f);
        Color textColor = isSelected ? BLACK : WHITE;

        DrawRectangleRec(btnRect, bgColor);
        DrawRectangleLinesEx(btnRect, 2, GRAY);

        Vector2 textSize = MeasureTextEx(font, options[i].c_str(), optionFontSize, 1);
        DrawTextEx(font, options[i].c_str(),
            {btnRect.x + btnRect.width / 2 - textSize.x / 2, btnRect.y + btnRect.height / 2 - textSize.y / 2},
            optionFontSize, 1, textColor);
    }
}

void Pages::StartPage::DrawPopups() {
    if (showModePopup) {
        DrawPopup({"Marathon", "Sprint"}, "Select Game Mode");
    } else if (showLevelPopup) {
        DrawPopup({"Easy(Level 1)", "Medium(Level 5)", "Hard(Level 10)", "Extreme(Level 15)"}, "Select Difficulty");
    } else {
        DrawMenu();
    }
}

void Pages::StartPage::HandleInput() {
    if (user.get_nickname().empty()) {
        HandleNicknameInput();
        return;
    }
    if (showModePopup || showLevelPopup) {
        HandlePopupInput();
        return;
    }
    HandleMenuInput();
}

void Pages::StartPage::HandleNicknameInput() {
    int key = GetCharPressed();
    while (key > 0) {
        if ((key >= 32 && key <= 125) && input_nickname.length() < 12) {
            input_nickname += (char)key;
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !input_nickname.empty()) {
        input_nickname.pop_back();
    }

    if (IsKeyPressed(KEY_ENTER) && !input_nickname.empty()) {
        user.set_nickname(input_nickname);
        user.store_user();
    }
}

void Pages::StartPage::HandlePopupInput() {
    int keyPressed = GetKeyPressed();
    int optionSize = showModePopup ? 2 : 4;

    if (keyPressed == KEY_DOWN) popupSelected = (popupSelected + 1) % optionSize;
    if (keyPressed == KEY_UP) popupSelected = (popupSelected - 1 + optionSize) % optionSize;

    if (keyPressed == KEY_ESCAPE) {
        showModePopup = showLevelPopup = false;
        popupSelected = 0;
        return;
    }

    if (keyPressed == KEY_ENTER) {
        if (showModePopup) {
            if (popupSelected == 0) { // Marathon
                showModePopup = false;
                showLevelPopup = true;
                popupSelected = 0;
            } else {
                nextMode = 5;
                showModePopup = false;
                exitMode = true;
            }
        } else {
            nextMode = popupSelected + 1;
            showLevelPopup = false;
            exitMode = true;
        }
    }
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
                case 2: nextMode = 7; break;    // multiplayer
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
    showModePopup = false;
    showLevelPopup = false;
    popupSelected = 0;
}
