//
// Created by jonas on 07.05.19.
//

#include "ConfigCheck.h"
#include "SopraGameLogic/GameModel.h"

bool configCheck::checkTeamFormation(const communication::messages::request::TeamFormation &formation,
                                     const gameHandling::TeamSide &side) {
    gameModel::Team team = {{}, formation, true};

    auto allPlayers = team.getAllPlayers();
    if(side == gameHandling::TeamSide::LEFT) {
        for (auto &player : allPlayers) {
            if (gameModel::Environment::getCell(player->position) == gameModel::Cell::OutOfBounds ||
                player->position.x > 7) {
                return false;
            }
        }
    }else{
        for (auto &player : allPlayers) {
            if (gameModel::Environment::getCell(player->position) == gameModel::Cell::OutOfBounds ||
                player->position.x < 9) {
                return false;
            }
        }
    }
    for (int i = 0; i < 7; i++){
        for (int j = i+1; j < 7; j++){
            if(allPlayers[i]->position == allPlayers[j]->position){
                return false;
            }
        }
    }
    return true;
}
