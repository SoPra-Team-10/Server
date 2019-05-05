#include "MessageHandlerMock.h"
#include "CommunicatorTest.h"

#include <gtest/gtest.h>

TEST(CommunicationLobby, SendTeamConfigs) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    communication::messages::Message joinRequestA{communication::messages::request::JoinRequest{"lobby", "a", ""}};
    communication::messages::Message joinRequestB{communication::messages::request::JoinRequest{"lobby", "b", ""}};
    communication::messages::Message joinResponse{communication::messages::unicast::JoinResponse{"Welcome to the Lobby"}};
    communication::messages::Message loginGreetingA{communication::messages::broadcast::LoginGreeting{"a"}};
    communication::messages::Message loginGreetingB{communication::messages::broadcast::LoginGreeting{"b"}};
    communication::messages::Message teamConfigs{communication::messages::request::TeamConfig{}};
    communication::messages::Message matchStart{
        communication::messages::broadcast::MatchStart{{}, {}, {}, "a", "b"}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingA, 1)).Times(1);

    EXPECT_CALL(messageHandler, send(joinResponse, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(matchStart, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(matchStart, 2)).Times(1);

    communication::CommunicatorTest communicator{messageHandler, log, {}};
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigs, 1));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigs, 2));
}

TEST(CommunicationLobby, TeamConfigThreePlayerError) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    communication::messages::Message joinRequestA{communication::messages::request::JoinRequest{"lobby", "a", ""}};
    communication::messages::Message joinRequestB{communication::messages::request::JoinRequest{"lobby", "b", ""}};
    communication::messages::Message joinRequestC{communication::messages::request::JoinRequest{"lobby", "c", ""}};
    communication::messages::Message joinResponse{communication::messages::unicast::JoinResponse{"Welcome to the Lobby"}};
    communication::messages::Message loginGreetingA{communication::messages::broadcast::LoginGreeting{"a"}};
    communication::messages::Message loginGreetingB{communication::messages::broadcast::LoginGreeting{"b"}};
    communication::messages::Message loginGreetingC{communication::messages::broadcast::LoginGreeting{"c"}};
    communication::messages::Message teamConfigs{communication::messages::request::TeamConfig{}};
    communication::messages::Message matchStart{
            communication::messages::broadcast::MatchStart{{}, {}, {}, "a", "b"}};
    communication::messages::Message matchFinishError{communication::messages::broadcast::MatchFinish{0,0,0,"",
                                                 communication::messages::types::VictoryReason::VIOLATION_OF_PROTOCOL}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingA, 1)).Times(1);

    EXPECT_CALL(messageHandler, send(joinResponse, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(joinResponse, 3)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingC, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingC, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingC, 3)).Times(1);

    EXPECT_CALL(messageHandler, send(matchStart, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(matchStart, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(matchStart, 3)).Times(1);
    EXPECT_CALL(messageHandler, send(matchFinishError, 3)).Times(1);
    EXPECT_CALL(messageHandler, send(matchFinishError, 1)).Times(0);
    EXPECT_CALL(messageHandler, send(matchFinishError, 2)).Times(0);

    communication::CommunicatorTest communicator{messageHandler, log, {}};
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestC, 3));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigs, 1));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigs, 2));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigs, 3));
}

TEST(CommunicationLobby, ChatSingle) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    communication::messages::Message joinRequestA{communication::messages::request::JoinRequest{"lobby", "a", ""}};
    communication::messages::Message joinResponse{communication::messages::unicast::JoinResponse{"Welcome to the Lobby"}};
    communication::messages::Message loginGreetingA{communication::messages::broadcast::LoginGreeting{"a"}};
    communication::messages::Message chatMessage{communication::messages::mods::request::SendChat{"Abc"}};
    communication::messages::Message chatReply{communication::messages::mods::broadcast::GlobalChat{"a", "Abc"}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingA, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(chatReply, 1)).Times(1);

    communication::CommunicatorTest communicator{messageHandler, log, {}};
    EXPECT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    EXPECT_NO_THROW(communicator.receiveTest(chatMessage, 1));
}

TEST(CommunicationLobby, ChatTwoSendA) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    communication::messages::Message joinRequestA{communication::messages::request::JoinRequest{"lobby", "a", ""}};
    communication::messages::Message joinRequestB{communication::messages::request::JoinRequest{"lobby", "b", ""}};
    communication::messages::Message joinResponse{communication::messages::unicast::JoinResponse{"Welcome to the Lobby"}};
    communication::messages::Message loginGreetingA{communication::messages::broadcast::LoginGreeting{"a"}};
    communication::messages::Message loginGreetingB{communication::messages::broadcast::LoginGreeting{"b"}};
    communication::messages::Message chatMessage{communication::messages::mods::request::SendChat{"Abc"}};
    communication::messages::Message chatReply{communication::messages::mods::broadcast::GlobalChat{"a", "Abc"}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingA, 1)).Times(1);

    EXPECT_CALL(messageHandler, send(joinResponse, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(chatReply, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(chatReply, 2)).Times(1);

    communication::CommunicatorTest communicator{messageHandler, log, {}};
    EXPECT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    EXPECT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
    EXPECT_NO_THROW(communicator.receiveTest(chatMessage, 1));
}

TEST(CommunicationLobby, ChatTwoSendB) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    communication::messages::Message joinRequestA{communication::messages::request::JoinRequest{"lobby", "a", ""}};
    communication::messages::Message joinRequestB{communication::messages::request::JoinRequest{"lobby", "b", ""}};
    communication::messages::Message joinResponse{communication::messages::unicast::JoinResponse{"Welcome to the Lobby"}};
    communication::messages::Message loginGreetingA{communication::messages::broadcast::LoginGreeting{"a"}};
    communication::messages::Message loginGreetingB{communication::messages::broadcast::LoginGreeting{"b"}};
    communication::messages::Message chatMessage{communication::messages::mods::request::SendChat{"Abc"}};
    communication::messages::Message chatReply{communication::messages::mods::broadcast::GlobalChat{"b", "Abc"}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingA, 1)).Times(1);

    EXPECT_CALL(messageHandler, send(joinResponse, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(chatReply, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(chatReply, 2)).Times(1);

    communication::CommunicatorTest communicator{messageHandler, log, {}};
    EXPECT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    EXPECT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
    EXPECT_NO_THROW(communicator.receiveTest(chatMessage, 2));
}

TEST(CommunicationLobby, ChatThreeSendA) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    communication::messages::Message joinRequestA{communication::messages::request::JoinRequest{"lobby", "a", ""}};
    communication::messages::Message joinRequestB{communication::messages::request::JoinRequest{"lobby", "b", ""}};
    communication::messages::Message joinRequestC{communication::messages::request::JoinRequest{"lobby", "c", ""}};
    communication::messages::Message joinResponse{communication::messages::unicast::JoinResponse{"Welcome to the Lobby"}};
    communication::messages::Message loginGreetingA{communication::messages::broadcast::LoginGreeting{"a"}};
    communication::messages::Message loginGreetingB{communication::messages::broadcast::LoginGreeting{"b"}};
    communication::messages::Message loginGreetingC{communication::messages::broadcast::LoginGreeting{"c"}};
    communication::messages::Message chatMessage{communication::messages::mods::request::SendChat{"Abc"}};
    communication::messages::Message chatReply{communication::messages::mods::broadcast::GlobalChat{"a", "Abc"}};

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
    EXPECT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    EXPECT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
    EXPECT_NO_THROW(communicator.receiveTest(joinRequestC, 3));
    EXPECT_NO_THROW(communicator.receiveTest(chatMessage, 1));
}

TEST(CommunicationLobby, ChatThreeSendB) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    communication::messages::Message joinRequestA{communication::messages::request::JoinRequest{"lobby", "a", ""}};
    communication::messages::Message joinRequestB{communication::messages::request::JoinRequest{"lobby", "b", ""}};
    communication::messages::Message joinRequestC{communication::messages::request::JoinRequest{"lobby", "c", ""}};
    communication::messages::Message joinResponse{communication::messages::unicast::JoinResponse{"Welcome to the Lobby"}};
    communication::messages::Message loginGreetingA{communication::messages::broadcast::LoginGreeting{"a"}};
    communication::messages::Message loginGreetingB{communication::messages::broadcast::LoginGreeting{"b"}};
    communication::messages::Message loginGreetingC{communication::messages::broadcast::LoginGreeting{"c"}};
    communication::messages::Message chatMessage{communication::messages::mods::request::SendChat{"Abc"}};
    communication::messages::Message chatReply{communication::messages::mods::broadcast::GlobalChat{"b", "Abc"}};

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
    EXPECT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    EXPECT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
    EXPECT_NO_THROW(communicator.receiveTest(joinRequestC, 3));
    EXPECT_NO_THROW(communicator.receiveTest(chatMessage, 2));
}

TEST(CommunicationLobby, ChatThreeSendC) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    communication::messages::Message joinRequestA{communication::messages::request::JoinRequest{"lobby", "a", ""}};
    communication::messages::Message joinRequestB{communication::messages::request::JoinRequest{"lobby", "b", ""}};
    communication::messages::Message joinRequestC{communication::messages::request::JoinRequest{"lobby", "c", ""}};
    communication::messages::Message joinResponse{communication::messages::unicast::JoinResponse{"Welcome to the Lobby"}};
    communication::messages::Message loginGreetingA{communication::messages::broadcast::LoginGreeting{"a"}};
    communication::messages::Message loginGreetingB{communication::messages::broadcast::LoginGreeting{"b"}};
    communication::messages::Message loginGreetingC{communication::messages::broadcast::LoginGreeting{"c"}};
    communication::messages::Message chatMessage{communication::messages::mods::request::SendChat{"Abc"}};
    communication::messages::Message chatReply{communication::messages::mods::broadcast::GlobalChat{"c", "Abc"}};

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
    EXPECT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    EXPECT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
    EXPECT_NO_THROW(communicator.receiveTest(joinRequestC, 3));
    EXPECT_NO_THROW(communicator.receiveTest(chatMessage, 3));
}
