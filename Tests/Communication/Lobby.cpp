#include "MessageHandlerMock.h"
#include "CommunicatorTest.h"

#include <gtest/gtest.h>

using namespace communication::messages;

TEST(CommunicationLobby, SendTeamConfigs) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    Message joinRequestA{request::JoinRequest{"lobby", "a", ""}};
    Message joinRequestB{request::JoinRequest{"lobby", "b", ""}};
    Message joinResponse{unicast::JoinResponse{"Welcome to the Lobby"}};
    Message loginGreetingA{broadcast::LoginGreeting{"a"}};
    Message loginGreetingB{broadcast::LoginGreeting{"b"}};
    Message teamConfigs{request::TeamConfig{}};
    Message matchStart{
        broadcast::MatchStart{{}, {}, {}, "a", "b"}};

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

    Message joinRequestA{request::JoinRequest{"lobby", "a", ""}};
    Message joinRequestB{request::JoinRequest{"lobby", "b", ""}};
    Message joinRequestC{request::JoinRequest{"lobby", "c", ""}};
    Message joinResponse{unicast::JoinResponse{"Welcome to the Lobby"}};
    Message loginGreetingA{broadcast::LoginGreeting{"a"}};
    Message loginGreetingB{broadcast::LoginGreeting{"b"}};
    Message loginGreetingC{broadcast::LoginGreeting{"c"}};
    Message teamConfigs{request::TeamConfig{}};
    Message matchStart{
            broadcast::MatchStart{{}, {}, {}, "a", "b"}};
    Message matchFinishError{broadcast::MatchFinish{0,0,0,"",types::VictoryReason::VIOLATION_OF_PROTOCOL}};

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

TEST(CommunicationLobby, DoubleTeamFormationA) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    Message joinRequestA{request::JoinRequest{"lobby", "a", ""}};
    Message joinRequestB{request::JoinRequest{"lobby", "b", ""}};
    Message joinResponse{unicast::JoinResponse{"Welcome to the Lobby"}};
    Message loginGreetingA{broadcast::LoginGreeting{"a"}};
    Message loginGreetingB{broadcast::LoginGreeting{"b"}};
    Message teamConfigs{request::TeamConfig{}};
    Message matchStart{
            broadcast::MatchStart{{}, {}, {}, "a", "b"}};
    Message teamFormation{request::TeamFormation{}};
    Message matchFinish{broadcast::MatchFinish{0,0,0,"b",types::VictoryReason::VIOLATION_OF_PROTOCOL}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingA, 1)).Times(1);

    EXPECT_CALL(messageHandler, send(joinResponse, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(matchStart, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(matchStart, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(matchFinish,1)).Times(1);
    EXPECT_CALL(messageHandler, send(matchFinish,2)).Times(1);

    communication::CommunicatorTest communicator{messageHandler, log, {}};
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigs, 1));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigs, 2));
    ASSERT_NO_THROW(communicator.receiveTest(teamFormation, 1));
    ASSERT_NO_THROW(communicator.receiveTest(teamFormation, 1));
}

TEST(CommunicationLobby, DoubleTeamFormationB) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    Message joinRequestA{request::JoinRequest{"lobby", "a", ""}};
    Message joinRequestB{request::JoinRequest{"lobby", "b", ""}};
    Message joinResponse{unicast::JoinResponse{"Welcome to the Lobby"}};
    Message loginGreetingA{broadcast::LoginGreeting{"a"}};
    Message loginGreetingB{broadcast::LoginGreeting{"b"}};
    Message teamConfigs{request::TeamConfig{}};
    Message matchStart{
            broadcast::MatchStart{{}, {}, {}, "a", "b"}};
    Message teamFormation{request::TeamFormation{}};
    Message matchFinish{broadcast::MatchFinish{0,0,0,"a",types::VictoryReason::VIOLATION_OF_PROTOCOL}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingA, 1)).Times(1);

    EXPECT_CALL(messageHandler, send(joinResponse, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(matchStart, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(matchStart, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(matchFinish,1)).Times(1);
    EXPECT_CALL(messageHandler, send(matchFinish,2)).Times(1);

    communication::CommunicatorTest communicator{messageHandler, log, {}};
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigs, 1));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigs, 2));
    ASSERT_NO_THROW(communicator.receiveTest(teamFormation, 2));
    ASSERT_NO_THROW(communicator.receiveTest(teamFormation, 2));
}

TEST(CommunicationLobby, TeamFormationSpectator) {
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
    Message teamConfigs{request::TeamConfig{}};
    Message matchStart{
            broadcast::MatchStart{{}, {}, {}, "a", "b"}};
    Message teamFormation{request::TeamFormation{}};
    Message matchFinish{broadcast::MatchFinish{0,0,0,"",types::VictoryReason::VIOLATION_OF_PROTOCOL}};

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

    EXPECT_CALL(messageHandler, send(matchFinish,1)).Times(0);
    EXPECT_CALL(messageHandler, send(matchFinish,2)).Times(0);
    EXPECT_CALL(messageHandler, send(matchFinish,3)).Times(1);

    communication::CommunicatorTest communicator{messageHandler, log, {}};
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestC, 3));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigs, 1));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigs, 2));
    ASSERT_NO_THROW(communicator.receiveTest(teamFormation, 3));
}

TEST(CommunicationLobby, TeamFormationBeforeMatchConfigA) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    Message joinRequestA{request::JoinRequest{"lobby", "a", ""}};
    Message joinRequestB{request::JoinRequest{"lobby", "b", ""}};
    Message joinResponse{unicast::JoinResponse{"Welcome to the Lobby"}};
    Message loginGreetingA{broadcast::LoginGreeting{"a"}};
    Message loginGreetingB{broadcast::LoginGreeting{"b"}};
    Message teamFormation{request::TeamFormation{}};
    Message matchFinish{broadcast::MatchFinish{0,0,0,"",types::VictoryReason::VIOLATION_OF_PROTOCOL}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingA, 1)).Times(1);

    EXPECT_CALL(messageHandler, send(joinResponse, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(matchFinish,1)).Times(1);
    EXPECT_CALL(messageHandler, send(matchFinish,2)).Times(0);

    communication::CommunicatorTest communicator{messageHandler, log, {}};
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
    ASSERT_NO_THROW(communicator.receiveTest(teamFormation, 1));
}

TEST(CommunicationLobby, TeamFormationBeforeMatchConfigB) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    Message joinRequestA{request::JoinRequest{"lobby", "a", ""}};
    Message joinRequestB{request::JoinRequest{"lobby", "b", ""}};
    Message joinResponse{unicast::JoinResponse{"Welcome to the Lobby"}};
    Message loginGreetingA{broadcast::LoginGreeting{"a"}};
    Message loginGreetingB{broadcast::LoginGreeting{"b"}};
    Message teamFormation{request::TeamFormation{}};
    Message matchFinish{broadcast::MatchFinish{0,0,0,"",types::VictoryReason::VIOLATION_OF_PROTOCOL}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingA, 1)).Times(1);

    EXPECT_CALL(messageHandler, send(joinResponse, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(matchFinish,1)).Times(0);
    EXPECT_CALL(messageHandler, send(matchFinish,2)).Times(1);

    communication::CommunicatorTest communicator{messageHandler, log, {}};
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
    ASSERT_NO_THROW(communicator.receiveTest(teamFormation, 2));
}

TEST(CommunicationLobby, PausRequestAiA) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    Message joinRequestA{request::JoinRequest{"lobby", "a", "", true}};
    Message joinRequestB{request::JoinRequest{"lobby", "b", ""}};
    Message joinResponse{unicast::JoinResponse{"Welcome to the Lobby"}};
    Message loginGreetingA{broadcast::LoginGreeting{"a"}};
    Message loginGreetingB{broadcast::LoginGreeting{"b"}};
    Message teamConfigs{request::TeamConfig{}};
    Message matchStart{
            broadcast::MatchStart{{}, {}, {}, "a", "b"}};
    Message pauseRequest{request::PauseRequest{"p"}};
    Message matchFinish{broadcast::MatchFinish{0,0,0,"b",types::VictoryReason::VIOLATION_OF_PROTOCOL}};
    Message error{unicast::PrivateDebug{"Error in pauseRequest:\tInvalid pause request: either AI or Game not started"}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingA, 1)).Times(1);

    EXPECT_CALL(messageHandler, send(joinResponse, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(matchStart, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(matchStart, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(error, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(matchFinish, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(matchFinish, 2)).Times(1);

    communication::CommunicatorTest communicator{messageHandler, log, {}};
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigs, 1));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigs, 2));
    ASSERT_NO_THROW(communicator.receiveTest(pauseRequest, 1));
}

TEST(CommunicationLobby, PausRequestAiB) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    Message joinRequestA{request::JoinRequest{"lobby", "a", ""}};
    Message joinRequestB{request::JoinRequest{"lobby", "b", "", true}};
    Message joinResponse{unicast::JoinResponse{"Welcome to the Lobby"}};
    Message loginGreetingA{broadcast::LoginGreeting{"a"}};
    Message loginGreetingB{broadcast::LoginGreeting{"b"}};
    Message teamConfigs{request::TeamConfig{}};
    Message matchStart{
            broadcast::MatchStart{{}, {}, {}, "a", "b"}};
    Message pauseRequest{request::PauseRequest{"p"}};
    Message matchFinish{broadcast::MatchFinish{0,0,0,"a",types::VictoryReason::VIOLATION_OF_PROTOCOL}};
    Message error{unicast::PrivateDebug{"Error in pauseRequest:\tInvalid pause request: either AI or Game not started"}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingA, 1)).Times(1);

    EXPECT_CALL(messageHandler, send(joinResponse, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(matchStart, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(matchStart, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(error, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(matchFinish, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(matchFinish, 2)).Times(1);

    communication::CommunicatorTest communicator{messageHandler, log, {}};
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigs, 1));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigs, 2));
    ASSERT_NO_THROW(communicator.receiveTest(pauseRequest, 2));
}

TEST(CommunicationLobby, PauseGameNotStarted) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    communication::messages::Message joinRequest{communication::messages::request::JoinRequest{}};
    communication::messages::Message joinResponse{communication::messages::unicast::JoinResponse{"Welcome to the Lobby"}};
    communication::messages::Message loginGreeting{communication::messages::broadcast::LoginGreeting{}};
    Message pauseRequest{request::PauseRequest{"p"}};
    Message error{unicast::PrivateDebug{"Error in pauseRequest:\tInvalid pause request: either AI or Game not started"}};
    Message matchFinish{broadcast::MatchFinish{0,0,0,"",types::VictoryReason::VIOLATION_OF_PROTOCOL}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreeting, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(error, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(matchFinish, 1)).Times(1);

    communication::CommunicatorTest communicator{messageHandler, log, {}};
    ASSERT_NO_THROW(communicator.receiveTest(joinRequest, 1));
    ASSERT_NO_THROW(communicator.receiveTest(pauseRequest, 1));
}
