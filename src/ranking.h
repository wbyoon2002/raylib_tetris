#ifndef RANKING_H
#define RANKING_H

#include <json.hpp>

#include "mode.h"
#include <string>
#include <vector>

using json = nlohmann::json;

namespace Pages {
    struct RankingEntry {
        std::string name;
        std::string uuid;
        int score;
    };


    class Ranking: public Mode {
    public:
        Ranking();
        ~Ranking();
        virtual void Draw();
        virtual void HandleInput();


    private:
        int selectedMode = 0; // 0 = Marathon, 1 = Sprint
        int selectedDifficulty = 0; // 0~3
        int maxVisibleEntries = 8;

        std::vector<RankingEntry> marathonRankings[4]; // Easy ~ Extreme
        std::vector<RankingEntry> sprintRankings;
        std::vector<std::string> difficulties = {"Easy", "Medium", "Hard", "Extreme"};
        std::vector<RankingEntry> rankingList; // 현재 선택에 따라 동적으로 로드

        Texture2D rank1Icon, rank2Icon, rank3Icon;

        json get_ranking();
        void DrawBackgroundBlocks();
        void parse_ranking_json(const json &j);
    };
}

#endif
