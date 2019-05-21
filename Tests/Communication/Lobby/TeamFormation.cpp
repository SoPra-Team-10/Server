#include "Tests/Communication/MessageHandlerMock.h"
#include "Tests/Communication/CommunicatorTest.h"
#include "CommonMessages.hpp"

#include <gtest/gtest.h>

using namespace communication::messages;


TEST(CommunicationLobby, DoubleTeamFormationA) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    Message joinRequestA{request::JoinRequest{"lobby", "a", ""}};
    Message joinRequestB{request::JoinRequest{"lobby", "b", ""}};
    Message joinResponse{unicast::JoinResponse{"Welcome to the Lobby"}};
    Message loginGreetingA{broadcast::LoginGreeting{"a"}};
    Message loginGreetingB{broadcast::LoginGreeting{"b"}};
    Message teamConfigLeft{getTeamConfig(true)};
    Message teamConfigRight{getTeamConfig(false)};
    Message matchStart{
            broadcast::MatchStart{getMatchConfig(), getTeamConfig(true), getTeamConfig(false), "a", "b"}};
    Message teamFormation{getTeamFormation(true)};
    Message matchFinish{broadcast::MatchFinish{0,0,0,"b",types::VictoryReason::VIOLATION_OF_PROTOCOL}};
    Message error{unicast::PrivateDebug{"Error in teamFormation:You sent two teamformations"}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingA, 1)).Times(1);

    EXPECT_CALL(messageHandler, send(joinResponse, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(matchStart, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(matchStart, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(error, 1)).Times(1);

    EXPECT_CALL(messageHandler, send(matchFinish,1)).Times(1);
    EXPECT_CALL(messageHandler, send(matchFinish,2)).Times(1);

    communication::CommunicatorTest communicator{messageHandler, log, getMatchConfig()};
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigLeft, 1));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigRight, 2));
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
    Message teamConfigLeft{getTeamConfig(true)};
    Message teamConfigRight{getTeamConfig(false)};
    Message matchStart{
            broadcast::MatchStart{getMatchConfig(), getTeamConfig(true), getTeamConfig(false), "a", "b"}};
    Message teamFormation{getTeamFormation(false)};
    Message matchFinish{broadcast::MatchFinish{0,0,0,"a",types::VictoryReason::VIOLATION_OF_PROTOCOL}};

    Message error{unicast::PrivateDebug{"Error in teamFormation:You sent two teamformations"}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingA, 1)).Times(1);

    EXPECT_CALL(messageHandler, send(joinResponse, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(matchStart, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(matchStart, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(error,2)).Times(1);

    EXPECT_CALL(messageHandler, send(matchFinish,1)).Times(1);
    EXPECT_CALL(messageHandler, send(matchFinish,2)).Times(1);

    communication::CommunicatorTest communicator{messageHandler, log, getMatchConfig()};
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigLeft, 1));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigRight, 2));
    ASSERT_NO_THROW(communicator.receiveTest(teamFormation, 2));
    ASSERT_NO_THROW(communicator.receiveTest(teamFormation, 2));
}

TEST(CommunicationLobby, TeamFormationSpectator) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    Message joinRequestA{request::JoinRequest{"lobby", "a", ""}};
    Message joinRequestB{request::JoinRequest{"lobby", "b", ""}};
    Message joinResponse{unicast::JoinResponse{"Welcome to the Lobby"}};
    Message loginGreetingA{broadcast::LoginGreeting{"a"}};
    Message loginGreetingB{broadcast::LoginGreeting{"b"}};
    Message teamConfigLeft{getTeamConfig(true)};
    Message teamConfigRight{getTeamConfig(false)};
    Message matchStart{
            broadcast::MatchStart{getMatchConfig(), getTeamConfig(true), getTeamConfig(false), "a", "b"}};

    Message joinRequestC{request::JoinRequest{"lobby", "c", ""}};
    Message loginGreetingC{broadcast::LoginGreeting{"c"}};
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

    communication::CommunicatorTest communicator{messageHandler, log, getMatchConfig()};
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestC, 3));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigLeft, 1));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigRight, 2));
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

