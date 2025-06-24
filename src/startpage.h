#ifndef STARTPAGE_H
#define STARTPAGE_H
#include "mode.h"
#include <string>
#include <user.h>
#include <vector>

namespace Pages {
    class StartPage: public Mode {
    public:
        StartPage();
        ~StartPage();
        virtual void Draw();
        virtual void HandleInput();
    private:
        int selectedOption;
        std::string title;
        std::string input_nickname;
        User user;
        std::vector<std::string> menuOptions;

        bool isMarathon;
        Texture2D logoTexture{};

        void ResetToMainMenu();
        void DrawPopup(const std::vector<std::string>& options, const std::string& title);
        void DrawHeader();
        void DrawNicknameInput();
        void DrawMenu();
        void DrawPopups();

        void HandleNicknameInput();
        void HandlePopupInput();
        void HandleMenuInput();

        bool showModePopup;
        bool showLevelPopup;
        int popupSelected;
        Vector2 headerEndPos;
    };
}

#endif
