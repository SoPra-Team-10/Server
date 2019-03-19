#include <iostream>

#include <SopraNetwork/WebSocketServer.hpp>

int main() {
    network::WebSocketServer server{8080, "test"};
}
