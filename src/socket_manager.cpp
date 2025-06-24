#include "socket_manager.h"

sio::client SocketManager::client;
bool SocketManager::initialized = false;

sio::client& SocketManager::get() {
    return client;
}

void SocketManager::initialize(const std::string& url) {
    if (!initialized) {
        client.connect(url);
        initialized = true;
    }
}

void SocketManager::close() {
    if (initialized) {
        client.close();
        initialized = false;
    }
}