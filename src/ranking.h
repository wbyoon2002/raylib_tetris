#ifndef RANKING_H
#define RANKING_H
#include "mode.h"

namespace Pages {
    class Ranking: public Mode {
    public:
        Ranking() = default;
        ~Ranking() = default;
        void Draw() override;
        void HandleInput() override;
    };
}

#endif
