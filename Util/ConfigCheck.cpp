//
// Created by jonas on 07.05.19.
//

#include "ConfigCheck.h"
#include "SopraGameLogic/GameModel.h"

bool configCheck::checkTeamFormation(const communication::messages::request::TeamFormation &formation,
                                     const gameHandling::TeamSide &side) {
    if(gameModel::Environment::getCell(formation.getBeater1X(),formation.getBeater1Y()) == gameModel::Cell::OutOfBounds){
        return false;
    }
}
