#include <iostream>

#include <SopraNetwork/WebSocketServer.hpp>

int main() {
    network::WebSocketServer server{80, "test"};
}
