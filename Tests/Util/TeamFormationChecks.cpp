//
// Created by bjorn on 07.05.19.
//

#include "gtest/gtest.h"
#include "SopraGameLogic/GameModel.h"
#include "src/Game/ConfigCheck.h"

TEST(teamFormationCheck, valid_team_formation){
    EXPECT_TRUE(configCheck::checkTeamFormation(communication::messages::request::TeamFormation{0,4,1,4,2,4,3,4,4,4,5,4,6,4}, gameHandling::TeamSide::LEFT));
}

TEST(teamFormationCheck, invalid_team_formation0){
    EXPECT_FALSE(configCheck::checkTeamFormation(communication::messages::request::TeamFormation{0,0,1,4,2,4,3,4,4,4,5,4,6,4}, gameHandling::TeamSide::LEFT));
}

TEST(teamFormationCheck, invalid_team_formation1){
    EXPECT_FALSE(configCheck::checkTeamFormation(communication::messages::request::TeamFormation{10,4,1,4,2,4,3,4,4,4,5,4,6,4}, gameHandling::TeamSide::LEFT));
}

TEST(teamFormationCheck, invalid_team_formation2){
    EXPECT_FALSE(configCheck::checkTeamFormation(communication::messages::request::TeamFormation{0,4,0,4,2,4,3,4,4,4,5,4,6,4}, gameHandling::TeamSide::LEFT));
}

TEST(checkTeamConfig, valid_team_config0) {
    EXPECT_TRUE(configCheck::checkTeamConfig(communication::messages::request::TeamConfig{
        "","","","","",
        1,1,1,2,2,
        {"seeker", communication::messages::types::Broom::FIREBOLT, communication::messages::types::Sex::F},
        {"keeper", communication::messages::types::Broom::CLEANSWEEP11, communication::messages::types::Sex::F},
        {"chaser1", communication::messages::types::Broom::COMET260, communication::messages::types::Sex::F},
        {"chaser2", communication::messages::types::Broom::NIMBUS2001, communication::messages::types::Sex::F},
        {"chaser3", communication::messages::types::Broom::TINDERBLAST, communication::messages::types::Sex::F},
        {"beater1", communication::messages::types::Broom::FIREBOLT, communication::messages::types::Sex::F},
        {"beater2", communication::messages::types::Broom::FIREBOLT, communication::messages::types::Sex::F}
    }));
}

TEST(checkTeamConfig, invalid_team_config0) {
    EXPECT_FALSE(configCheck::checkTeamConfig(communication::messages::request::TeamConfig{
            "","","","","",
            1,1,1,2,2,
            {"seeker", communication::messages::types::Broom::FIREBOLT, communication::messages::types::Sex::F},
            {"keeper", communication::messages::types::Broom::CLEANSWEEP11, communication::messages::types::Sex::F},
            {"chaser1", communication::messages::types::Broom::COMET260, communication::messages::types::Sex::F},
            {"chaser2", communication::messages::types::Broom::NIMBUS2001, communication::messages::types::Sex::F},
            {"chaser3", communication::messages::types::Broom::FIREBOLT, communication::messages::types::Sex::F},
            {"beater1", communication::messages::types::Broom::FIREBOLT, communication::messages::types::Sex::F},
            {"beater2", communication::messages::types::Broom::FIREBOLT, communication::messages::types::Sex::F}
    }));
}