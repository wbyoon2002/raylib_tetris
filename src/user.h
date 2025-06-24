#ifndef USER_H
#define USER_H

#include <json.hpp>
#include <string>
#include <map>

enum class GameType {
    SPRINT,
    MARATHON
};

enum class MarathonDifficulty {
    EASY,
    MEDIUM,
    HARD,
    EXTREME
};

class User {
public:
    User() = default;
    User(std::string nickname, std::string uuid): nickname(nickname), uuid(uuid) {};

    void store_user();
    void load_user();
    void update_server(nlohmann::json j);

    void set_nickname(const std::string &nickname);
    void set_uuid(const std::string &uuid);

    bool set_best_time(double score);
    bool set_high_score(GameType game, MarathonDifficulty difficulty, int score);

    double get_best_time() const;
    int get_high_score(GameType game, MarathonDifficulty difficulty) const;

    std::string get_nickname() const;
    std::string get_uuid() const;

private:
    std::string nickname{};
    std::string uuid{};
    double sprint_time{};
    std::map<MarathonDifficulty, int> marathon_scores;
};

#endif //USER_H
