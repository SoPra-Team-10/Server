#include <iostream>

#include <SopraNetwork/WebSocketServer.hpp>
#include <Communication/MessageHandler.hpp>
#include <Communication/Messages/Unicast/PrivateDebug.hpp>

int main() {
    communication::MessageHandler communicator{8080};
}
