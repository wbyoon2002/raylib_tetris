#include "user.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <socket_manager.h>
#include <uuid_hasher.h>

using json = nlohmann::json;

void User::set_uuid(const std::string &uuid) {
    if (UUIDHasher::is_self(uuid))
        this->uuid = uuid;
}

void User::set_nickname(const std::string &nickname) {
    if (UUIDHasher::is_self(uuid))
        this->nickname = nickname;
}

std::string User::get_nickname() const {
    return nickname;
}

std::string User::get_uuid() const {
    return uuid;
}

bool User::set_best_time(double time) {
    if (UUIDHasher::is_self(uuid)) {
        if (sprint_time == 0 || sprint_time > time) {
            sprint_time = time;
            return true;
        }
    }
    return false;
}

bool User::set_high_score(GameType game, MarathonDifficulty difficulty, int score) {
    if (UUIDHasher::is_self(uuid)) {
        if (game == GameType::MARATHON) {
            if (marathon_scores[difficulty] < score) {
                marathon_scores[difficulty] = score;
                return true;
            }
        }
    }
    return false;
}

double User::get_best_time() const {
    return sprint_time;
}

int User::get_high_score(GameType game, MarathonDifficulty difficulty) const {
    if (game == GameType::MARATHON) {
        auto it = marathon_scores.find(difficulty);
        return (it != marathon_scores.end()) ? it->second : 0;
    }
    return 0;
}

std::string marathon_difficulty_to_string(MarathonDifficulty diff) {
    switch (diff) {
        case MarathonDifficulty::EASY: return "Easy";
        case MarathonDifficulty::MEDIUM: return "Medium";
        case MarathonDifficulty::HARD: return "Hard";
        default: return "Extreme";
    }
}

MarathonDifficulty marathon_difficulty_from_string(const std::string &s) {
    if (s == "Easy") return MarathonDifficulty::EASY;
    if (s == "Medium") return MarathonDifficulty::MEDIUM;
    if (s == "Hard") return MarathonDifficulty::HARD;
    return MarathonDifficulty::EXTREME;
}

void User::store_user() {
    if (UUIDHasher::is_self(uuid)) {
        json j;
        j["uuid"] = uuid;
        j["nickname"] = nickname;

        j["scores"]["sprint"] = sprint_time;

        for (const auto &[diff, score]: marathon_scores) {
            j["scores"]["marathon"][marathon_difficulty_to_string(diff)] = score;
        }

        std::ofstream out("user.json");
        out << std::setw(4) << j << std::endl;
        update_server(j);
    }
}

void User::load_user() {
    std::ifstream in("user.json");
    if (!in.is_open()) return;

    json j;
    in >> j;

    uuid = j.value("uuid", "");
    nickname = j.value("nickname", "");

    sprint_time = j["scores"]["sprint"].get<double>();

    for (auto &[key, val]: j["scores"]["marathon"].items()) {
        MarathonDifficulty diff = marathon_difficulty_from_string(key);
        marathon_scores[diff] = val.get<int>();
    }
}

void User::update_server(json j) {
    auto& h = SocketManager::get();
    if (!h.opened()) {
        h.connect("http://localhost:3000");

        h.set_open_listener([j, this, &h]() {
            auto msg = sio::string_message::create(to_string(j));
            h.socket()->emit("update_user", msg);
        });
    } else {
        auto msg = sio::string_message::create(to_string(j));
        h.socket()->emit("update_user", msg);
    }
}
