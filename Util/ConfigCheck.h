//
// Created by jonas on 07.05.19.
//

#ifndef SERVER_CONFIGCHECK_H
#define SERVER_CONFIGCHECK_H

#include <Game/GameTypes.h>
#include <SopraMessages/TeamFormation.hpp>

namespace configCheck {

    static bool checkTeamConfig(const communication::messages::request::TeamConfig &config, const gameHandling::TeamSide &side);

    bool checkTeamFormation(const communication::messages::request::TeamFormation &formation, const gameHandling::TeamSide &side);

    bool checkPlayerOnField(int x, int y);

}

#endif //SERVER_CONFIGCHECK_H
