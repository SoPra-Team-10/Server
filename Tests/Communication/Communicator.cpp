//
// Created by paulnykiel on 04.05.19.
//

#include "MessageHandlerMock.h"
#include <gtest/gtest.h>
#include <Communication/Communicator.hpp>

namespace communication {
    class CommunicatorTest : public Communicator {
    public:
        using Communicator::Communicator;
        void receiveTest(messages::Message message, int client) {
            this->receive(message, client);
        }
    };
}

TEST(CommunicationCommunicator, Send) {
    std::stringstream sstream;
    util::Logging log{sstream, 0};
    communication::MessageHandlerMock messageHandler{8080, log};

    communication::messages::unicast::PrivateDebug privateDebug{"abc"};
    communication::messages::Message message{privateDebug};

    EXPECT_CALL(messageHandler, send(message, 1)).Times(1);

    communication::Communicator communicator{messageHandler, log, {}};
    EXPECT_NO_THROW(communicator.send(message, 1));
}

TEST(CommunicationCommunicator, Join) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    communication::messages::Message joinRequest{communication::messages::request::JoinRequest{}};
    communication::messages::Message joinResponse{communication::messages::unicast::JoinResponse{"Welcome to the Lobby"}};
    communication::messages::Message loginGreeting{communication::messages::broadcast::LoginGreeting{}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreeting, 1)).Times(1);

    communication::CommunicatorTest communicator{messageHandler, log, {}};
    EXPECT_NO_THROW(communicator.receiveTest(joinRequest, 1));
}