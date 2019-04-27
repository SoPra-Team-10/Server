#include <iostream>

#include <SopraNetwork/WebSocketServer.hpp>
#include <Communication/MessageHandler.hpp>
#include <Communication/Messages/Unicast/PrivateDebug.hpp>

void onMessage(std::shared_ptr<communication::MessageHandler> messageHandler, communication::messages::Message, int client) {
    communication::messages::Message reply{communication::messages::unicast::PrivateDebug{"Hello World!"}};
    messageHandler->send(reply, client);
}

int main() {
    auto messageHandler = std::make_shared<communication::MessageHandler>(8080);
    messageHandler->onReceive(
            std::bind(&onMessage, messageHandler, std::placeholders::_1, std::placeholders::_2));

    while (true);
}
