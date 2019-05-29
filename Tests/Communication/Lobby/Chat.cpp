#include "Tests/Communication/MessageHandlerMock.h"
#include "Tests/Communication/CommunicatorTest.h"

#include <gtest/gtest.h>

using namespace communication::messages;

TEST(CommunicationLobby, ChatSingle) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    Message joinRequestA{request::JoinRequest{"lobby", "a", ""}};
    Message joinResponse{unicast::JoinResponse{"Welcome to the Lobby"}};
    Message loginGreetingA{broadcast::LoginGreeting{"a"}};
    Message chatMessage{mods::request::SendChat{"Abc"}};
    Message chatReply{mods::broadcast::GlobalChat{"a", "Abc"}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingA, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(chatReply, 1)).Times(1);

    communication::CommunicatorTest communicator{messageHandler, log, {}};
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    ASSERT_NO_THROW(communicator.receiveTest(chatMessage, 1));
}

TEST(CommunicationLobby, ChatTwoSendA) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    Message joinRequestA{request::JoinRequest{"lobby", "a", ""}};
    Message joinRequestB{request::JoinRequest{"lobby", "b", ""}};
    Message joinResponse{unicast::JoinResponse{"Welcome to the Lobby"}};
    Message loginGreetingA{broadcast::LoginGreeting{"a"}};
    Message loginGreetingB{broadcast::LoginGreeting{"b"}};
    Message chatMessage{mods::request::SendChat{"Abc"}};
    Message chatReply{mods::broadcast::GlobalChat{"a", "Abc"}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingA, 1)).Times(1);

    EXPECT_CALL(messageHandler, send(joinResponse, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(chatReply, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(chatReply, 2)).Times(1);

    communication::CommunicatorTest communicator{messageHandler, log, {}};
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
    ASSERT_NO_THROW(communicator.receiveTest(chatMessage, 1));
}

TEST(CommunicationLobby, ChatTwoSendB) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    Message joinRequestA{request::JoinRequest{"lobby", "a", ""}};
    Message joinRequestB{request::JoinRequest{"lobby", "b", ""}};
    Message joinResponse{unicast::JoinResponse{"Welcome to the Lobby"}};
    Message loginGreetingA{broadcast::LoginGreeting{"a"}};
    Message loginGreetingB{broadcast::LoginGreeting{"b"}};
    Message chatMessage{mods::request::SendChat{"Abc"}};
    Message chatReply{mods::broadcast::GlobalChat{"b", "Abc"}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingA, 1)).Times(1);

    EXPECT_CALL(messageHandler, send(joinResponse, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(chatReply, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(chatReply, 2)).Times(1);

    communication::CommunicatorTest communicator{messageHandler, log, {}};
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
    ASSERT_NO_THROW(communicator.receiveTest(chatMessage, 2));
}

TEST(CommunicationLobby, ChatThreeSendA) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    Message joinRequestA{request::JoinRequest{"lobby", "a", ""}};
    Message joinRequestB{request::JoinRequest{"lobby", "b", ""}};
    Message joinRequestC{request::JoinRequest{"lobby", "c", ""}};
    Message joinResponse{unicast::JoinResponse{"Welcome to the Lobby"}};
    Message loginGreetingA{broadcast::LoginGreeting{"a"}};
    Message loginGreetingB{broadcast::LoginGreeting{"b"}};
    Message loginGreetingC{broadcast::LoginGreeting{"c"}};
    Message chatMessage{mods::request::SendChat{"Abc"}};
    Message chatReply{mods::broadcast::GlobalChat{"a", "Abc"}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingA, 1)).Times(1);

    EXPECT_CALL(messageHandler, send(joinResponse, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(joinResponse, 3)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingC, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingC, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingC, 3)).Times(1);

    EXPECT_CALL(messageHandler, send(chatReply, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(chatReply, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(chatReply, 3)).Times(1);

    communication::CommunicatorTest communicator{messageHandler, log, {}};
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestC, 3));
    ASSERT_NO_THROW(communicator.receiveTest(chatMessage, 1));
}

TEST(CommunicationLobby, ChatThreeSendB) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    Message joinRequestA{request::JoinRequest{"lobby", "a", ""}};
    Message joinRequestB{request::JoinRequest{"lobby", "b", ""}};
    Message joinRequestC{request::JoinRequest{"lobby", "c", ""}};
    Message joinResponse{unicast::JoinResponse{"Welcome to the Lobby"}};
    Message loginGreetingA{broadcast::LoginGreeting{"a"}};
    Message loginGreetingB{broadcast::LoginGreeting{"b"}};
    Message loginGreetingC{broadcast::LoginGreeting{"c"}};
    Message chatMessage{mods::request::SendChat{"Abc"}};
    Message chatReply{mods::broadcast::GlobalChat{"b", "Abc"}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingA, 1)).Times(1);

    EXPECT_CALL(messageHandler, send(joinResponse, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(joinResponse, 3)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingC, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingC, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingC, 3)).Times(1);

    EXPECT_CALL(messageHandler, send(chatReply, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(chatReply, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(chatReply, 3)).Times(1);

    communication::CommunicatorTest communicator{messageHandler, log, {}};
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestC, 3));
    ASSERT_NO_THROW(communicator.receiveTest(chatMessage, 2));
}

TEST(CommunicationLobby, ChatThreeSendC) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    Message joinRequestA{request::JoinRequest{"lobby", "a", ""}};
    Message joinRequestB{request::JoinRequest{"lobby", "b", ""}};
    Message joinRequestC{request::JoinRequest{"lobby", "c", ""}};
    Message joinResponse{unicast::JoinResponse{"Welcome to the Lobby"}};
    Message loginGreetingA{broadcast::LoginGreeting{"a"}};
    Message loginGreetingB{broadcast::LoginGreeting{"b"}};
    Message loginGreetingC{broadcast::LoginGreeting{"c"}};
    Message chatMessage{mods::request::SendChat{"Abc"}};
    Message chatReply{mods::broadcast::GlobalChat{"c", "Abc"}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingA, 1)).Times(1);

    EXPECT_CALL(messageHandler, send(joinResponse, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(joinResponse, 3)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingC, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingC, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingC, 3)).Times(1);

    EXPECT_CALL(messageHandler, send(chatReply, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(chatReply, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(chatReply, 3)).Times(1);

    communication::CommunicatorTest communicator{messageHandler, log, {}};
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestC, 3));
    ASSERT_NO_THROW(communicator.receiveTest(chatMessage, 3));
}

