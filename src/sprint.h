#ifndef SPRINT_H
#define SPRINT_H
#include "game.h"
#include "user.h"

namespace Pages {
    class Sprint : public Game {
    public:
        Sprint(int totalLines = 40);
        ~Sprint();
        virtual void Draw();
        virtual void HandleInput();
    private:
        double startTime;
        double finalTime;
        User user;
        void Reset();
        bool isFinished;
        int totalLines;
        Sound victorySound;
    };
}

#endif