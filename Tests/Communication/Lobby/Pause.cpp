#include "Tests/Communication/MessageHandlerMock.h"
#include "Tests/Communication/CommunicatorTest.h"

#include <gtest/gtest.h>

using namespace communication::messages;

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

TEST(CommunicationLobby, ContinueNotPauseA) {
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
    Message continueRequest{request::ContinueRequest{"p"}};
    Message matchFinish{broadcast::MatchFinish{0,0,0,"a",types::VictoryReason::VIOLATION_OF_PROTOCOL}};
    Message error{unicast::PrivateDebug{"Error in continueRequest:\tNot paused!"}};

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
    ASSERT_NO_THROW(communicator.receiveTest(continueRequest, 2));
}

TEST(CommunicationLobby, ContinueNotPauseB) {
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
    Message continueRequest{request::ContinueRequest{"p"}};
    Message matchFinish{broadcast::MatchFinish{0,0,0,"b",types::VictoryReason::VIOLATION_OF_PROTOCOL}};
    Message error{unicast::PrivateDebug{"Error in continueRequest:\tNot paused!"}};

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
    ASSERT_NO_THROW(communicator.receiveTest(continueRequest, 1));
}


TEST(CommunicationLobby, PauseStartAStopA) {
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
    Message teamFormations{request::TeamFormation{}};
    Message snapshot{broadcast::Snapshot{}};
    Message pauseRequest{request::PauseRequest{"p"}};
    Message pauseResponse{broadcast::PauseResponse{"p", "a", true}};
    Message continueRequest{request::ContinueRequest{"c"}};
    Message continueResponse{broadcast::PauseResponse{"c", "a", false}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingA, 1)).Times(1);

    EXPECT_CALL(messageHandler, send(joinResponse, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(matchStart, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(matchStart, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(snapshot, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(snapshot, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(pauseResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(pauseResponse, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(continueResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(continueResponse, 2)).Times(1);

    communication::CommunicatorTest communicator{messageHandler, log, {}};
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigs, 1));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigs, 2));
    ASSERT_NO_THROW(communicator.receiveTest(teamFormations, 1));
    ASSERT_NO_THROW(communicator.receiveTest(teamFormations, 2));
    ASSERT_NO_THROW(communicator.receiveTest(pauseRequest, 1));
    ASSERT_NO_THROW(communicator.receiveTest(continueRequest, 1));
}

TEST(CommunicationLobby, PauseStartAStopB) {
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
    Message teamFormations{request::TeamFormation{}};
    Message snapshot{broadcast::Snapshot{}};
    Message pauseRequest{request::PauseRequest{"p"}};
    Message pauseResponse{broadcast::PauseResponse{"p", "a", true}};
    Message continueRequest{request::ContinueRequest{"c"}};
    Message continueResponse{broadcast::PauseResponse{"c", "b", false}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingA, 1)).Times(1);

    EXPECT_CALL(messageHandler, send(joinResponse, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(matchStart, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(matchStart, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(snapshot, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(snapshot, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(pauseResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(pauseResponse, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(continueResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(continueResponse, 2)).Times(1);

    communication::CommunicatorTest communicator{messageHandler, log, {}};
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigs, 1));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigs, 2));
    ASSERT_NO_THROW(communicator.receiveTest(teamFormations, 1));
    ASSERT_NO_THROW(communicator.receiveTest(teamFormations, 2));
    ASSERT_NO_THROW(communicator.receiveTest(pauseRequest, 1));
    ASSERT_NO_THROW(communicator.receiveTest(continueRequest, 2));
}

TEST(CommunicationLobby, PauseStartBStopA) {
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
    Message teamFormations{request::TeamFormation{}};
    Message snapshot{broadcast::Snapshot{}};
    Message pauseRequest{request::PauseRequest{"p"}};
    Message pauseResponse{broadcast::PauseResponse{"p", "b", true}};
    Message continueRequest{request::ContinueRequest{"c"}};
    Message continueResponse{broadcast::PauseResponse{"c", "a", false}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingA, 1)).Times(1);

    EXPECT_CALL(messageHandler, send(joinResponse, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(matchStart, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(matchStart, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(snapshot, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(snapshot, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(pauseResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(pauseResponse, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(continueResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(continueResponse, 2)).Times(1);

    communication::CommunicatorTest communicator{messageHandler, log, {}};
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigs, 1));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigs, 2));
    ASSERT_NO_THROW(communicator.receiveTest(teamFormations, 1));
    ASSERT_NO_THROW(communicator.receiveTest(teamFormations, 2));
    ASSERT_NO_THROW(communicator.receiveTest(pauseRequest, 2));
    ASSERT_NO_THROW(communicator.receiveTest(continueRequest, 1));
}

TEST(CommunicationLobby, PauseStartBStopB) {
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
    Message teamFormations{request::TeamFormation{}};
    Message snapshot{broadcast::Snapshot{}};
    Message pauseRequest{request::PauseRequest{"p"}};
    Message pauseResponse{broadcast::PauseResponse{"p", "b", true}};
    Message continueRequest{request::ContinueRequest{"c"}};
    Message continueResponse{broadcast::PauseResponse{"c", "b", false}};

    EXPECT_CALL(messageHandler, send(joinResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingA, 1)).Times(1);

    EXPECT_CALL(messageHandler, send(joinResponse, 2)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(loginGreetingB, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(matchStart, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(matchStart, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(snapshot, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(snapshot, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(pauseResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(pauseResponse, 2)).Times(1);

    EXPECT_CALL(messageHandler, send(continueResponse, 1)).Times(1);
    EXPECT_CALL(messageHandler, send(continueResponse, 2)).Times(1);

    communication::CommunicatorTest communicator{messageHandler, log, {}};
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestA, 1));
    ASSERT_NO_THROW(communicator.receiveTest(joinRequestB, 2));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigs, 1));
    ASSERT_NO_THROW(communicator.receiveTest(teamConfigs, 2));
    ASSERT_NO_THROW(communicator.receiveTest(teamFormations, 1));
    ASSERT_NO_THROW(communicator.receiveTest(teamFormations, 2));
    ASSERT_NO_THROW(communicator.receiveTest(pauseRequest, 2));
    ASSERT_NO_THROW(communicator.receiveTest(continueRequest, 2));
}
