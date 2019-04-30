#include <gtest/gtest.h>
#include <Communication/Messages/Common/types.hpp>

using namespace communication::messages::types;

TEST(CommunicationMessagesCommonTypes, EntityIdLeftSeeker) {
    EXPECT_EQ(fromStringEntityId(toString(EntityId::LEFT_SEEKER)), EntityId::LEFT_SEEKER);
}
TEST(CommunicationMessagesCommonTypes, EntityIdLeftKeeper) {
    EXPECT_EQ(fromStringEntityId(toString(EntityId::LEFT_KEEPER)), EntityId::LEFT_KEEPER);
}
TEST(CommunicationMessagesCommonTypes, EntityIdLeftChaser1) {
    EXPECT_EQ(fromStringEntityId(toString(EntityId::LEFT_CHASER1)), EntityId::LEFT_CHASER1);
}
TEST(CommunicationMessagesCommonTypes, EntityIdLeftChaser2) {
    EXPECT_EQ(fromStringEntityId(toString(EntityId::LEFT_CHASER2)), EntityId::LEFT_CHASER2);
}
TEST(CommunicationMessagesCommonTypes, EntityIdLeftChaser3) {
    EXPECT_EQ(fromStringEntityId(toString(EntityId::LEFT_CHASER3)), EntityId::LEFT_CHASER3);
}
TEST(CommunicationMessagesCommonTypes, EntityIdLeftBeater1) {
    EXPECT_EQ(fromStringEntityId(toString(EntityId::LEFT_BEATER1)), EntityId::LEFT_BEATER1);
}
TEST(CommunicationMessagesCommonTypes, EntityIdLeftBeater2) {
    EXPECT_EQ(fromStringEntityId(toString(EntityId::LEFT_BEATER2)), EntityId::LEFT_BEATER2);
}
TEST(CommunicationMessagesCommonTypes, EntityIdRightSeeker) {
    EXPECT_EQ(fromStringEntityId(toString(EntityId::RIGHT_SEEKER)), EntityId::RIGHT_SEEKER);
}
TEST(CommunicationMessagesCommonTypes, EntityIdRightKeeper) {
    EXPECT_EQ(fromStringEntityId(toString(EntityId::RIGHT_KEEPER)), EntityId::RIGHT_KEEPER);
}
TEST(CommunicationMessagesCommonTypes, EntityIdRightChaser1) {
    EXPECT_EQ(fromStringEntityId(toString(EntityId::RIGHT_CHASER1)), EntityId::RIGHT_CHASER1);
}
TEST(CommunicationMessagesCommonTypes, EntityIdRightChaser2) {
    EXPECT_EQ(fromStringEntityId(toString(EntityId::RIGHT_CHASER2)), EntityId::RIGHT_CHASER2);
}
TEST(CommunicationMessagesCommonTypes, EntityIdRightChaser3) {
    EXPECT_EQ(fromStringEntityId(toString(EntityId::RIGHT_CHASER3)), EntityId::RIGHT_CHASER3);
}
TEST(CommunicationMessagesCommonTypes, EntityIdRightBeater1) {
    EXPECT_EQ(fromStringEntityId(toString(EntityId::RIGHT_BEATER1)), EntityId::RIGHT_BEATER1);
}
TEST(CommunicationMessagesCommonTypes, EntityIdRightBeater2) {
    EXPECT_EQ(fromStringEntityId(toString(EntityId::RIGHT_BEATER2)), EntityId::RIGHT_BEATER2);
}
TEST(CommunicationMessagesCommonTypes, EntityIdSnitch) {
    EXPECT_EQ(fromStringEntityId(toString(EntityId::SNITCH)), EntityId::SNITCH);
}
TEST(CommunicationMessagesCommonTypes, EntityIdBludger1) {
    EXPECT_EQ(fromStringEntityId(toString(EntityId::BLUDGER1)), EntityId::BLUDGER1);
}
TEST(CommunicationMessagesCommonTypes, EntityIdBludger2) {
    EXPECT_EQ(fromStringEntityId(toString(EntityId::BLUDGER2)), EntityId::BLUDGER2);
}
TEST(CommunicationMessagesCommonTypes, EntityIdQuaffle) {
    EXPECT_EQ(fromStringEntityId(toString(EntityId::QUAFFLE)), EntityId::QUAFFLE);
}
TEST(CommunicationMessagesCommonTypes, EntityIdLeftGoblin) {
    EXPECT_EQ(fromStringEntityId(toString(EntityId::LEFT_GOBLIN)), EntityId::LEFT_GOBLIN);
}
TEST(CommunicationMessagesCommonTypes, EntityIdLeftTroll) {
    EXPECT_EQ(fromStringEntityId(toString(EntityId::LEFT_TROLL)), EntityId::LEFT_TROLL);
}
TEST(CommunicationMessagesCommonTypes, EntityIdLeftElf) {
    EXPECT_EQ(fromStringEntityId(toString(EntityId::LEFT_ELF)), EntityId::LEFT_ELF);
}
TEST(CommunicationMessagesCommonTypes, EntityIdLeftNiffler) {
    EXPECT_EQ(fromStringEntityId(toString(EntityId::LEFT_NIFFLER)), EntityId::LEFT_NIFFLER);
}
TEST(CommunicationMessagesCommonTypes, EntityIdRightGoblin) {
    EXPECT_EQ(fromStringEntityId(toString(EntityId::RIGHT_GOBLIN)), EntityId::RIGHT_GOBLIN);
}
TEST(CommunicationMessagesCommonTypes, EntityIdRightTroll) {
    EXPECT_EQ(fromStringEntityId(toString(EntityId::RIGHT_TROLL)), EntityId::RIGHT_TROLL);
}
TEST(CommunicationMessagesCommonTypes, EntityIdRightElf) {
    EXPECT_EQ(fromStringEntityId(toString(EntityId::RIGHT_ELF)), EntityId::RIGHT_ELF);
}
TEST(CommunicationMessagesCommonTypes, EntityIdRightNiffler) {
    EXPECT_EQ(fromStringEntityId(toString(EntityId::RIGHT_NIFFLER)), EntityId::RIGHT_NIFFLER);
}

TEST(CommunicationMessagesCommonTypes, TurnTypeMove) {
    EXPECT_EQ(fromStringTurnType(toString(TurnType::MOVE)), TurnType::MOVE);
}
TEST(CommunicationMessagesCommonTypes, TurnTypeAction) {
    EXPECT_EQ(fromStringTurnType(toString(TurnType::ACTION)), TurnType::ACTION);
}

TEST(CommunicationMessagesCommonTypes, VictoryReasonDisqualification) {
    EXPECT_EQ(fromStringVictoryReason(toString(VictoryReason::DISQUALIFICATION)), VictoryReason::DISQUALIFICATION);
}
TEST(CommunicationMessagesCommonTypes, VictoryReasonBothDisMostPoints) {
    EXPECT_EQ(fromStringVictoryReason(toString(VictoryReason::BOTH_DISQUALIFICATION_MOST_POINTS)), VictoryReason::BOTH_DISQUALIFICATION_MOST_POINTS);
}
TEST(CommunicationMessagesCommonTypes, VictoryReasonBothDisPointsEqualSnitch) {
    EXPECT_EQ(fromStringVictoryReason(toString(VictoryReason::BOTH_DISQUALIFICATION_POINTS_EQUAL_SNITCH_CATCH)), VictoryReason::BOTH_DISQUALIFICATION_POINTS_EQUAL_SNITCH_CATCH);
}
TEST(CommunicationMessagesCommonTypes, VictoryReasonBothDisPointsEqualLast) {
    EXPECT_EQ(fromStringVictoryReason(toString(VictoryReason::BOTH_DISQUALIFICATION_POINTS_EQUAL_LAST_DISQUALIFICATION)), VictoryReason::BOTH_DISQUALIFICATION_POINTS_EQUAL_LAST_DISQUALIFICATION);
}
TEST(CommunicationMessagesCommonTypes, VictoryReasonMostPoints) {
    EXPECT_EQ(fromStringVictoryReason(toString(VictoryReason::MOST_POINTS)), VictoryReason::MOST_POINTS);
}
TEST(CommunicationMessagesCommonTypes, VictoryReasonPointsEqualSnitch) {
    EXPECT_EQ(fromStringVictoryReason(toString(VictoryReason::POINTS_EQUAL_SNITCH_CATCH)), VictoryReason::POINTS_EQUAL_SNITCH_CATCH);
}
TEST(CommunicationMessagesCommonTypes, VictoryReasonViolationOfProt) {
    EXPECT_EQ(fromStringVictoryReason(toString(VictoryReason::VIOLATION_OF_PROTOCOL)), VictoryReason::VIOLATION_OF_PROTOCOL);
}

TEST(CommunicationMessagesCommonTypes, FanTypeGoblin) {
    EXPECT_EQ(fromStringFanType(toString(FanType::GOBLIN)), FanType::GOBLIN);
}
TEST(CommunicationMessagesCommonTypes, FanTypeTroll) {
    EXPECT_EQ(fromStringFanType(toString(FanType::TROLL)), FanType::TROLL);
}
TEST(CommunicationMessagesCommonTypes, FanTypeElf) {
    EXPECT_EQ(fromStringFanType(toString(FanType::ELF)), FanType::ELF);
}
TEST(CommunicationMessagesCommonTypes, FanTypeNiffler) {
    EXPECT_EQ(fromStringFanType(toString(FanType::NIFFLER)), FanType::NIFFLER);
}

TEST(CommunicationMessagesCommonTypes, BroomThinderblast) {
    EXPECT_EQ(fromStringBroom(toString(Broom::THINDERBLAST)), Broom::THINDERBLAST);
}
TEST(CommunicationMessagesCommonTypes, BroomCleansweep) {
    EXPECT_EQ(fromStringBroom(toString(Broom::CLEANSWEEP11)), Broom::CLEANSWEEP11);
}
TEST(CommunicationMessagesCommonTypes, BroomComet) {
    EXPECT_EQ(fromStringBroom(toString(Broom::COMET260)), Broom::COMET260);
}
TEST(CommunicationMessagesCommonTypes, BroomNimbus) {
    EXPECT_EQ(fromStringBroom(toString(Broom::NIMBUS2001)), Broom::NIMBUS2001);
}
TEST(CommunicationMessagesCommonTypes, BroomFirebolt) {
    EXPECT_EQ(fromStringBroom(toString(Broom::FIREBOLT)), Broom::FIREBOLT);
}

TEST(CommunicationMessagesCommonTypes, SexM) {
    EXPECT_EQ(fromStringSex(toString(Sex::M)), Sex::M);
}
TEST(CommunicationMessagesCommonTypes, SexF) {
    EXPECT_EQ(fromStringSex(toString(Sex::F)), Sex::F);
}

TEST(CommunicationMessagesCommonTypes, DeltaTypeSnitchCatch) {
    EXPECT_EQ(fromStringDeltaType(toString(DeltaType::SNITCH_CATCH)), DeltaType::SNITCH_CATCH);
}
TEST(CommunicationMessagesCommonTypes, DeltaTypeBludgerBeating) {
    EXPECT_EQ(fromStringDeltaType(toString(DeltaType::BLUDGER_BEATING)), DeltaType::BLUDGER_BEATING);
}
TEST(CommunicationMessagesCommonTypes, DeltaTypeQuaffleThrow) {
    EXPECT_EQ(fromStringDeltaType(toString(DeltaType::QUAFFLE_THROW)), DeltaType::QUAFFLE_THROW);
}
TEST(CommunicationMessagesCommonTypes, DeltaTypeSnitchSnatch) {
    EXPECT_EQ(fromStringDeltaType(toString(DeltaType::SNITCH_SNATCH)), DeltaType::SNITCH_SNATCH);
}
TEST(CommunicationMessagesCommonTypes, DeltaTypeTrollRoard) {
    EXPECT_EQ(fromStringDeltaType(toString(DeltaType::TROLL_ROAR)), DeltaType::TROLL_ROAR);
}
TEST(CommunicationMessagesCommonTypes, DeltaTypeElfTeleportation) {
    EXPECT_EQ(fromStringDeltaType(toString(DeltaType::ELF_TELEPORTATION)), DeltaType::ELF_TELEPORTATION);
}
TEST(CommunicationMessagesCommonTypes, DeltaTypeGoblinShock) {
    EXPECT_EQ(fromStringDeltaType(toString(DeltaType::GOBLIN_SHOCK)), DeltaType::GOBLIN_SHOCK);
}
TEST(CommunicationMessagesCommonTypes, DeltaTypeBan) {
    EXPECT_EQ(fromStringDeltaType(toString(DeltaType::BAN)), DeltaType::BAN);
}
TEST(CommunicationMessagesCommonTypes, DeltaTypeBludgerKnockout) {
    EXPECT_EQ(fromStringDeltaType(toString(DeltaType::BLUDGER_KNOCKOUT)), DeltaType::BLUDGER_KNOCKOUT);
}
TEST(CommunicationMessagesCommonTypes, DeltaTypeMove) {
    EXPECT_EQ(fromStringDeltaType(toString(DeltaType::MOVE)), DeltaType::MOVE);
}

TEST(CommunicationMessagesCommonTypes, PhaseTypeBallPhase) {
    EXPECT_EQ(fromStringPhaseType(toString(PhaseType::BALL_PHASE)), PhaseType::BALL_PHASE);
}
TEST(CommunicationMessagesCommonTypes, PhaseTypePlayerPhase) {
    EXPECT_EQ(fromStringPhaseType(toString(PhaseType::PLAYER_PHASE)), PhaseType::PLAYER_PHASE);
}
TEST(CommunicationMessagesCommonTypes, PhaseTypeActionPhase) {
    EXPECT_EQ(fromStringPhaseType(toString(PhaseType::ACTION_PHASE)), PhaseType::ACTION_PHASE);
}
TEST(CommunicationMessagesCommonTypes, PhaseTypeFanPhase) {
    EXPECT_EQ(fromStringPhaseType(toString(PhaseType::FAN_PHASE)), PhaseType::FAN_PHASE);
}
TEST(CommunicationMessagesCommonTypes, PhaseTypeGameFinish) {
    EXPECT_EQ(fromStringPhaseType(toString(PhaseType::GAME_FINISH)), PhaseType::GAME_FINISH);
}
