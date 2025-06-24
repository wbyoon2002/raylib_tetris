#include "ranking.h"

#include <future>
#include <socket_manager.h>

#include "colors.h"
#include <vector>
#include <string>
#include <uuid_hasher.h>

using json = nlohmann::json;

float spacing = 1.0f;

int selectedMode = 0; // 0 = Marathon, 1 = Sprint
int selectedDifficulty = 0; // 0 = Easy, 1 = Medium, 2 = Hard, 3 = Extreme
const char *modes[2] = {"Marathon", "Sprint"};
const char *difficulties[4] = {"Easy", "Medium", "Hard", "Extreme"};
std::vector<Pages::RankingEntry> marathonRankings[4];
std::vector<Pages::RankingEntry> sprintRankings;

Pages::Ranking::Ranking() {
    json ranking = get_ranking();
    parse_ranking_json(ranking);

    std::string assetsBasePath = ASSETS_PATH;
    rank1Icon = LoadTexture((assetsBasePath + "/Icons/1st.png").c_str());
    rank2Icon = LoadTexture((assetsBasePath + "/Icons/2nd.png").c_str());
    rank3Icon = LoadTexture((assetsBasePath + "/Icons/3rd.png").c_str());
}

Pages::Ranking::~Ranking() {
    UnloadTexture(rank1Icon);
    UnloadTexture(rank2Icon);
    UnloadTexture(rank3Icon);
}

json Pages::Ranking::get_ranking() {
    auto &h = SocketManager::get();

    std::promise<json> promise;
    auto future = promise.get_future();

    h.socket()->on("get_ranking", [&](sio::event &ev) {
        auto message = ev.get_message()->get_string();

        h.socket()->off("get_ranking");
        promise.set_value(json::parse(message));
    });

    h.socket()->emit("get_ranking");

    return future.get();
}


void Pages::Ranking::parse_ranking_json(const json &j) {
    std::map<std::string, int> diffIndex = {
        {"easy", 0},
        {"medium", 1},
        {"hard", 2},
        {"extreme", 3}
    };

    for (int i = 0; i < 4; ++i)
        marathonRankings[i].clear();
    sprintRankings.clear();

    for (const auto &[diffStr, scoreArray]: j["marathon"].items()) {
        auto it = diffIndex.find(diffStr);
        if (it != diffIndex.end() && scoreArray.is_array()) {
            for (const auto &entry: scoreArray) {
                marathonRankings[it->second].push_back({entry["nickname"], entry["uuid"], entry["score"]});
            }
        }
    }

    if (j.contains("sprint")) {
        for (const auto &entry: j["sprint"]) {
            sprintRankings.push_back({entry["nickname"], entry["uuid"], entry["time"]});
        }
    }
}

void Pages::Ranking::DrawBackgroundBlocks() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int blockSize = 20;

    for (int y = 0; y < screenHeight; y += blockSize * 2) {
        for (int x = (y / blockSize) % 2 == 0 ? 0 : blockSize; x < screenWidth; x += blockSize * 2) {
            Color faintColor = ColorAlpha(GRAY, 0.05f);
            DrawRectangle(x, y, blockSize, blockSize, faintColor);
        }
    }
}

void Pages::Ranking::Draw() {
    ClearBackground(darkBlue);
    DrawBackgroundBlocks();
    float screenWidth = GetScreenWidth();

    // --- 상단 모드 탭 ---
    float tabWidth = 156;
    float tabHeight = 46.8f;
    float tabSpacing = 26;
    float totalTabWidth = 2 * tabWidth + tabSpacing;
    float tabStartX = (screenWidth - totalTabWidth) / 2;

    for (int i = 0; i < 2; ++i) {
        Color tabColor = (selectedMode == i) ? ORANGE : LIGHTGRAY;
        float x = tabStartX + i * (tabWidth + tabSpacing);
        DrawRectangleRounded({x, 40, tabWidth, tabHeight}, 0.3f, 8, tabColor);

        Vector2 textSize = MeasureTextEx(font, modes[i], 29.9f, spacing);
        float textX = x + (tabWidth - textSize.x) / 2;
        float textY = 40 + (tabHeight - textSize.y) / 2;
        DrawTextEx(font, modes[i], {textX, textY}, 29.9f, spacing, BLACK);
    }

    // --- 난이도 선택 탭 ---
    if (selectedMode == 0) {
        float diffTabWidth = 117;
        float diffTabHeight = 39;
        float diffSpacing = 19.5f;
        float totalWidth = 4 * diffTabWidth + 3 * diffSpacing;
        float startX = (screenWidth - totalWidth) / 2;

        // 살구색 박스 Y 위치 및 높이
        float bgY = 110;
        float bgHeight = 65;

        float buttonY = bgY + (bgHeight - diffTabHeight) / 2;
        DrawRectangleRounded({startX - 13, bgY, totalWidth + 26, bgHeight}, 0.2f, 10, {255, 230, 200, 255});

        for (int i = 0; i < 4; ++i) {
            float x = startX + i * (diffTabWidth + diffSpacing);
            Color tabColor = selectedDifficulty == i ? ORANGE : LIGHTGRAY;

            DrawRectangleRounded({x, buttonY, diffTabWidth, diffTabHeight}, 0.25f, 6, tabColor);

            Vector2 textSize = MeasureTextEx(font, difficulties[i].c_str(), 26.0f, spacing);
            float textX = x + (diffTabWidth - textSize.x) / 2;
            float textY = buttonY + (diffTabHeight - textSize.y) / 2;
            DrawTextEx(font, difficulties[i].c_str(), {textX, textY}, 26.0f, spacing, BLACK);
        }
    }

    const std::vector<RankingEntry> &currentRanking =
            (selectedMode == 0) ? marathonRankings[selectedDifficulty] : sprintRankings;

    float listStartY = selectedMode == 0 ? 200 : 112;
    float entryHeight = 58.5f;
    int totalEntries = currentRanking.size();

    // --- 랭킹 카드 출력 ---
    for (int i = 0; i < totalEntries; ++i) {
        float y = listStartY + i * (entryHeight + 10);
        if (y < listStartY - entryHeight || y > 600) continue;

        Rectangle entryRect = {screenWidth / 2 - 286, y, 572, entryHeight - 6.5f};

        Color bg = LIGHTGRAY;
        if (i == 0) bg = GOLD;
        else if (i == 1) bg = GRAY;
        else if (i == 2) bg = BEIGE;

        if (UUIDHasher::is_self(currentRanking[i].uuid)) {
            DrawRectangleRoundedLinesEx(entryRect, 0.25f, 8, 5, BLUE);

            Rectangle outerGlow = {
                entryRect.x - 2, entryRect.y - 2,
                entryRect.width + 4, entryRect.height + 4
            };
            DrawRectangleRoundedLinesEx(outerGlow, 0.25f, 8, 2, SKYBLUE);
        }

        DrawRectangleRounded(entryRect, 0.25f, 8, bg);

        float iconY = entryRect.y + (entryHeight - 42.9f) / 2;
        Vector2 iconPos = {entryRect.x + 6.5f, iconY};
        float scale = 42.9f / 500.0f;

        if (i == 0)
            DrawTextureEx(rank1Icon, iconPos, 0.0f, scale, WHITE);
        else if (i == 1)
            DrawTextureEx(rank2Icon, iconPos, 0.0f, scale, WHITE);
        else if (i == 2)
            DrawTextureEx(rank3Icon, iconPos, 0.0f, scale, WHITE);
        else
            DrawTextEx(font, std::to_string(i + 1).c_str(), {iconPos.x + 14.0f, entryRect.y + 15.6f}, 26.0f, spacing,
                       BLACK);

        float nameX = entryRect.x + 65;
        DrawTextEx(font, currentRanking[i].name.c_str(), {nameX, entryRect.y + 15.6f}, 26.0f, spacing, BLACK);
        DrawTextEx(font, std::to_string(currentRanking[i].score).c_str(), {entryRect.x + 429, entryRect.y + 15.6f},
                   26.0f, spacing, BLACK);
    }
}

void Pages::Ranking::HandleInput() {
    if (IsKeyPressed(KEY_ESCAPE)) {
        exitMode = true;
        return;
    }

    if (IsKeyPressed(KEY_TAB)) selectedMode = (selectedMode + 1) % 2;

    if (selectedMode == 0) {
        if (IsKeyPressed(KEY_LEFT)) selectedDifficulty = (selectedDifficulty + 3) % 4;
        if (IsKeyPressed(KEY_RIGHT)) selectedDifficulty = (selectedDifficulty + 1) % 4;
    }
}
