//
// Created by jonas on 07.05.19.
//

#ifndef SERVER_CONFIGCHECK_H
#define SERVER_CONFIGCHECK_H

#include <Game/GameTypes.h>
#include <SopraMessages/TeamFormation.hpp>
#include <SopraMessages/TeamConfig.hpp>
#include <SopraGameLogic/GameModel.h>

namespace configCheck {

    /**
     * Checks if the given TeamConfig complies with the game rules
     * @param config the config to be checked
     * @return true if ok, false if rule violation
     */
    bool checkTeamConfig(const communication::messages::request::TeamConfig &config);

    /**
     * Checks if the given TeamFormation complies with the game rules
     * @param formation The formation to be checked
     * @param side The side of the team
     * @return true if ok, false if rule violation
     */
    bool checkTeamFormation(const communication::messages::request::TeamFormation &formation,
            const gameModel::TeamSide &side);

}

#endif //SERVER_CONFIGCHECK_H
