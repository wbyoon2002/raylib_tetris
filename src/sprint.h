#ifndef SPRINT_H
#define SPRINT_H
#include "game.h"

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
        void Reset();
        bool isFinished;
        int totalLines;
        Sound victorySound;
    };
}

#endif