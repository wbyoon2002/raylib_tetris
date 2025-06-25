#ifndef MARATHON_H
#define MARATHON_H
#include "game.h"

namespace Pages {
    class Marathon : public Game {
    public:
        Marathon(int initLevel = 1);
        ~Marathon();
        virtual void Draw();
        virtual void HandleInput();
    private:
        // 1. basic implementation
        int score;
        void UpdateScore(int points);
        void Reset();
        // 6. score UI/scoring implementation
        void UpdateBonusPoints();
        int bonusPoints;
        // bonus points for each action
        std::vector<int> actionMultipliers;
        // trigger function for displaying the level-up display
        bool LevelUpTriggered();
        double lastLevelUpTime;
        // 5. difficulty control implementation
        int initLevel;
        int level;
        // update the drop period according to the current level
        void UpdateDropPeriod();
    };
}

#endif