#ifndef STARTPAGE_H
#define STARTPAGE_H
#include "mode.h"
#include <string>
#include <vector>

namespace Pages {
    class StartPage: public Mode {
    public:
        StartPage();
        ~StartPage();
        void Draw() override;
        void HandleInput() override;
    private:
        int selectedOption;
        std::string title;
        std::vector<std::string> menuOptions;

        bool isMarathon;
        Texture2D logoTexture{};

        void ResetToMainMenu();
        void DrawHeader();
        void DrawMenu();

        void HandleMenuInput();

        Vector2 headerEndPos;
    };
}

#endif
