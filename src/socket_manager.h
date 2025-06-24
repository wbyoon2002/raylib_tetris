#ifndef SOCKET_MANAGER_H
#define SOCKET_MANAGER_H

#include <sio_client.h>

class SocketManager {
public:
    static sio::client& get();
    static void initialize(const std::string& url);
    static void close();

private:
    static sio::client client;
    static bool initialized;
};

#endif