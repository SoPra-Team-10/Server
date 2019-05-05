#include "Tests/Communication/MessageHandlerMock.h"
#include "Tests/Communication/CommunicatorTest.h"

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
