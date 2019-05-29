#include "Communication/MessageHandlerMock.h"
#include "Communication/CommunicatorTest.h"
#include "CommonMessages.hpp"

#include <gtest/gtest.h>

using namespace communication::messages;

TEST(CommunicationLobby, DeltaRequestBeforeTeamFormationA) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};


    Message joinRequestA{request::JoinRequest{"lobby", "a", "", true}};
    Message joinRequestB{request::JoinRequest{"lobby", "b", ""}};
    Message joinResponse{unicast::JoinResponse{"Welcome to the Lobby"}};
    Message loginGreetingA{broadcast::LoginGreeting{"a"}};
    Message loginGreetingB{broadcast::LoginGreeting{"b"}};
    Message teamConfigLeft{getTeamConfig(true)};
    Message teamConfigRight{getTeamConfig(false)};
    Message matchStart{
            broadcast::MatchStart{getMatchConfig(), getTeamConfig(true), getTeamConfig(false), "a", "b"}};
    Message deltaRequest{request::DeltaRequest{}};

    Message matchFinish{broadcast::MatchFinish{0,0,0,"b",types::VictoryReason::VIOLATION_OF_PROTOCOL}};
    Message error{unicast::PrivateDebug{"Error in deltaRequest:Not in game"}};

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


    communication::CommunicatorTest communicator{messageHandler, log, getMatchConfig()};
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigLeft, 1));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigRight, 2));
    ASSERT_NO_THROW(communicator.receiveTest(deltaRequest, 1));
}

TEST(CommunicationLobby, DeltaRequestBeforeTeamFormationB) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    Message joinRequestA{request::JoinRequest{"lobby", "a", "", true}};
    Message joinRequestB{request::JoinRequest{"lobby", "b", ""}};
    Message joinResponse{unicast::JoinResponse{"Welcome to the Lobby"}};
    Message loginGreetingA{broadcast::LoginGreeting{"a"}};
    Message loginGreetingB{broadcast::LoginGreeting{"b"}};
    Message teamConfigLeft{getTeamConfig(true)};
    Message teamConfigRight{getTeamConfig(false)};
    Message matchStart{
            broadcast::MatchStart{getMatchConfig(),getTeamConfig(true),getTeamConfig(false), "a", "b"}};
    Message deltaRequest{request::DeltaRequest{}};

    Message matchFinish{broadcast::MatchFinish{0,0,0,"a",types::VictoryReason::VIOLATION_OF_PROTOCOL}};
    Message error{unicast::PrivateDebug{"Error in deltaRequest:Not in game"}};

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

    communication::CommunicatorTest communicator{messageHandler, log, getMatchConfig()};
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigLeft, 1));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigRight, 2));
    ASSERT_NO_THROW(communicator.receiveTest(deltaRequest, 2));
}

TEST(CommunicationLobby, DeltaRequestSpectatorA) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    Message joinRequestA{request::JoinRequest{"lobby", "a", "", true}};
    Message joinRequestB{request::JoinRequest{"lobby", "b", ""}};
    Message joinResponse{unicast::JoinResponse{"Welcome to the Lobby"}};
    Message loginGreetingA{broadcast::LoginGreeting{"a"}};
    Message loginGreetingB{broadcast::LoginGreeting{"b"}};
    Message deltaRequest{request::DeltaRequest{}};

    Message matchFinish{broadcast::MatchFinish{0,0,0,"",types::VictoryReason::VIOLATION_OF_PROTOCOL}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingA, 1)).Times(1);

    EXPECT_CALL(messageHandler, send(joinResponse, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(matchFinish, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(matchFinish, 2)).Times(0);

    communication::CommunicatorTest communicator{messageHandler, log, {}};
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
    ASSERT_NO_THROW(communicator.receiveTest(deltaRequest, 1));
}

TEST(CommunicationLobby, DeltaRequestSpectatorB) {
    std::stringstream sstream;
    util::Logging log{sstream, 10};
    communication::MessageHandlerMock messageHandler{8080, log};

    Message joinRequestA{request::JoinRequest{"lobby", "a", "", true}};
    Message joinRequestB{request::JoinRequest{"lobby", "b", ""}};
    Message joinResponse{unicast::JoinResponse{"Welcome to the Lobby"}};
    Message loginGreetingA{broadcast::LoginGreeting{"a"}};
    Message loginGreetingB{broadcast::LoginGreeting{"b"}};
    Message deltaRequest{request::DeltaRequest{}};

    Message matchFinish{broadcast::MatchFinish{0,0,0,"",types::VictoryReason::VIOLATION_OF_PROTOCOL}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingA, 1)).Times(1);

    EXPECT_CALL(messageHandler, send(joinResponse, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(matchFinish, 1)).Times(0);
    EXPECT_CALL(messageHandler, send(matchFinish, 2)).Times(1);

    communication::CommunicatorTest communicator{messageHandler, log, {}};
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
    ASSERT_NO_THROW(communicator.receiveTest(deltaRequest, 2));
}
