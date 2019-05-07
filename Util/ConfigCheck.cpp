//
// Created by jonas on 07.05.19.
//

#include "ConfigCheck.h"
#include <SopraGameLogic/GameModel.h>
#include <SopraMessages/types.hpp>
/*
bool configCheck::checkTeamFormation(const communication::messages::request::TeamFormation &formation,
                                     const gameHandling::TeamSide &side) {
    if(gameModel::Environment::getCell(formation.getBeater1X(),formation.getBeater1Y()) == gameModel::Cell::OutOfBounds){
        return false;
    }
}
*/
bool configCheck::checkTeamConfig(const communication::messages::request::TeamConfig &config) {
    gameModel::Team team(config, {}, true);

    int TINDERBLAST = 0;
    int CLEANSWEEP11 = 0;
    int COMET260 = 0;
    int NIMBUS2001 = 0;
    int FIREBOLT = 0;

    auto players = team.getAllPlayers();
    for (const auto &player : players) {
        if (player->broom == communication::messages::types::Broom::TINDERBLAST) {
            TINDERBLAST++;
        }
        else if (player->broom == communication::messages::types::Broom::CLEANSWEEP11) {
            CLEANSWEEP11++;
        }
        else if (player->broom == communication::messages::types::Broom::COMET260) {
            COMET260++;
        }
        else if (player->broom == communication::messages::types::Broom::NIMBUS2001) {
            NIMBUS2001++;
        }
        else if (player->broom == communication::messages::types::Broom::FIREBOLT) {
            FIREBOLT++;
        }
        else {
            return false;
        }
    }

    // check fanblock
    if (team.fanblock.getUses(gameModel::InterferenceType::Impulse) +
        team.fanblock.getUses(gameModel::InterferenceType::SnitchPush) +
        team.fanblock.getUses(gameModel::InterferenceType::RangedAttack) +
        team.fanblock.getUses(gameModel::InterferenceType::Teleport) != 7) {
        return false;
    }
    if (team.fanblock.getUses(gameModel::InterferenceType::Impulse) < 1 ||
        team.fanblock.getUses(gameModel::InterferenceType::SnitchPush) < 1 ||
        team.fanblock.getUses(gameModel::InterferenceType::RangedAttack) < 1 ||
        team.fanblock.getUses(gameModel::InterferenceType::Teleport) < 1) {
        return false;
    }

    // broom usage check
    if (TINDERBLAST < 1 || CLEANSWEEP11 < 1 || COMET260 < 1 || NIMBUS2001 < 1 || FIREBOLT < 1) {
        return false;
    }

    return true;
}
