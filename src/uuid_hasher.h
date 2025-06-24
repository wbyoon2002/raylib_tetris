#ifndef UUID_HASHER_H
#define UUID_HASHER_H

#include <string>

class UUIDHasher {

public:
    static std::string generate_uuid();
    static bool is_self(const std::string& uuid);
    static void set_uuid(const std::string& new_uuid);

private:
    static std::string folder_path;
    static std::string file_path;
    static std::string uuid;
};

#endif