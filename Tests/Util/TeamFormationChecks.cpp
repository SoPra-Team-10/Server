//
// Created by bjorn on 07.05.19.
//

#include "gtest/gtest.h"
#include "SopraGameLogic/GameModel.h"
#include "Util/ConfigCheck.h"

TEST(teamFormationCHheck, valid_team_formation){
    EXPECT_TRUE(configCheck::checkTeamFormation(communication::messages::request::TeamFormation{0,4,1,4,2,4,3,4,4,4,5,4,6,4}, gameHandling::TeamSide::LEFT));
}

TEST(teamFormationCHheck, invalid_team_formation0){
    EXPECT_FALSE(configCheck::checkTeamFormation(communication::messages::request::TeamFormation{0,0,1,4,2,4,3,4,4,4,5,4,6,4}, gameHandling::TeamSide::LEFT));
}

TEST(teamFormationCHheck, invalid_team_formation1){
    EXPECT_FALSE(configCheck::checkTeamFormation(communication::messages::request::TeamFormation{10,4,1,4,2,4,3,4,4,4,5,4,6,4}, gameHandling::TeamSide::LEFT));
}

TEST(teamFormationCHheck, invalid_team_formation2){
    EXPECT_FALSE(configCheck::checkTeamFormation(communication::messages::request::TeamFormation{0,4,0,4,2,4,3,4,4,4,5,4,6,4}, gameHandling::TeamSide::LEFT));
}