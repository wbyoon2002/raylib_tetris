#ifndef LOCALPVP_H
#define LOCALPVP_H
#include "player.h"

namespace Pages {
    class LocalPvP: public Mode {
    public:
        LocalPvP();
        ~LocalPvP();
        void HandleInput() override;
        void Draw() override;
    private:
        Player** players;
        void DrawPopup(const std::string&);
        int winnerIndex;
        bool hasPaused;
    };
}

#endif