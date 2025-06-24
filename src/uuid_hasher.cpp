#include "uuid_hasher.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <sys/stat.h>
#include <direct.h>

std::string UUIDHasher::folder_path = R"(C:\ProgramData\project-team-26)";
std::string UUIDHasher::file_path = folder_path + R"(\uuid.txt)";
std::string UUIDHasher::uuid;

std::string UUIDHasher::generate_uuid() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    std::uniform_int_distribution<> dis2(8, 11);

    std::stringstream ss;
    int i;
    ss << std::hex;
    for (i = 0; i < 8; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 4; i++) {
        ss << dis(gen);
    }
    ss << "-4";
    for (i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    ss << dis2(gen);
    for (i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 12; i++) {
        ss << dis(gen);
    };

    uuid = ss.str();
    return uuid;
}

bool UUIDHasher::is_self(const std::string& uuid) {
    if (uuid == UUIDHasher::uuid)
        return true;

    return false;
}

void UUIDHasher::set_uuid(const std::string& new_uuid) {
    if (!uuid.empty()) {
        return;
    }

    uuid = new_uuid;
}