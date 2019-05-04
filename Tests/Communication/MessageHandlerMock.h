#ifndef SERVER_MESSAGEHANDLERMOCK_H
#define SERVER_MESSAGEHANDLERMOCK_H

#include <Communication/MessageHandler.hpp>

#include <gmock/gmock.h>

namespace communication {
    class MessageHandlerMock : public MessageHandler{
    public:
        using MessageHandler::MessageHandler;
        MOCK_METHOD2(send, void(const messages::Message &message, int client));
    };
}

#endif //SERVER_MESSAGEHANDLERMOCK_H
