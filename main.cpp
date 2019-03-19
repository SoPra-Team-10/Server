#include <iostream>

#include <SopraNetwork/WebSocketServer.hpp>
#include <Communication/Communicator.hpp>
#include <Communication/Messages/Unicast/PrivateDebug.hpp>

int main() {
    communication::Communicator communicator{8080};
    communication::messages::unicast::PrivateDebug privateDebug{"Test"};
    communicator.sendAll(privateDebug);
}
