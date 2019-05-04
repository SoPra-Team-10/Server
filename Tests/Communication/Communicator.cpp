//
// Created by paulnykiel on 04.05.19.
//

#include "MessageHandlerMock.h"
#include "CommunicatorTest.h"

#include <gtest/gtest.h>

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

TEST(CommunicationCommunicator, JoinSameLobby) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    communication::messages::Message joinRequestA{communication::messages::request::JoinRequest{"lobby", "a", ""}};
    communication::messages::Message joinRequestB{communication::messages::request::JoinRequest{"lobby", "b", ""}};
    communication::messages::Message joinResponse{communication::messages::unicast::JoinResponse{"Welcome to the Lobby"}};
    communication::messages::Message loginGreetingA{communication::messages::broadcast::LoginGreeting{"a"}};
    communication::messages::Message loginGreetingB{communication::messages::broadcast::LoginGreeting{"b"}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingA, 1)).Times(1);

    EXPECT_CALL(messageHandler, send(joinResponse, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 2)).Times(1);

    communication::CommunicatorTest communicator{messageHandler, log, {}};
    EXPECT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    EXPECT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
}

TEST(CommunicationCommunicator, JoinDifferentLobby) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    communication::messages::Message joinRequestA{communication::messages::request::JoinRequest{"lobby1", "a", ""}};
    communication::messages::Message joinRequestB{communication::messages::request::JoinRequest{"lobby2", "b", ""}};
    communication::messages::Message joinResponse{communication::messages::unicast::JoinResponse{"Welcome to the Lobby"}};
    communication::messages::Message loginGreetingA{communication::messages::broadcast::LoginGreeting{"a"}};
    communication::messages::Message loginGreetingB{communication::messages::broadcast::LoginGreeting{"b"}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingA, 1)).Times(1);

    EXPECT_CALL(messageHandler, send(joinResponse, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 1)).Times(0);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 2)).Times(1);

    communication::CommunicatorTest communicator{messageHandler, log, {}};
    EXPECT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    EXPECT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
}

TEST(CommunicationCommunicator, MultipleJoin) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    communication::messages::Message joinRequestA{communication::messages::request::JoinRequest{"lobby1", "a", ""}};
    communication::messages::Message joinRequestB{communication::messages::request::JoinRequest{"lobby2", "a", ""}};
    communication::messages::Message joinResponse{communication::messages::unicast::JoinResponse{"Welcome to the Lobby"}};
    communication::messages::Message loginGreetingA{communication::messages::broadcast::LoginGreeting{"a"}};
    communication::messages::Message errorResponse{communication::messages::unicast::PrivateDebug{"You are already in a Lobby!"}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingA, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(errorResponse, 1)).Times(1);

    communication::CommunicatorTest communicator{messageHandler, log, {}};
    EXPECT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    EXPECT_NO_THROW(communicator.receiveTest(joinRequestB, 1));
}

TEST(CommunicationCommunicator, NoJoinRequest) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    communication::messages::Message teamConfig{communication::messages::request::TeamConfig{}};
    communication::messages::Message joinResponse{communication::messages::unicast::JoinResponse{"Welcome to the Lobby"}};
    communication::messages::Message loginGreetingA{communication::messages::broadcast::LoginGreeting{"a"}};
    communication::messages::Message errorResponse{communication::messages::unicast::PrivateDebug{"You need to send a JoinRequest first!"}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(0);
    EXPECT_CALL(messageHandler, send(loginGreetingA, 1)).Times(0);
    EXPECT_CALL(messageHandler, send(errorResponse, 1)).Times(1);

    communication::CommunicatorTest communicator{messageHandler, log, {}};
    EXPECT_NO_THROW(communicator.receiveTest(teamConfig, 1));
}