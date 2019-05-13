//
// Created by jonas on 07.05.19.
//

#ifndef SERVER_CONFIGCHECK_H
#define SERVER_CONFIGCHECK_H

#include <Game/GameTypes.h>
#include <SopraMessages/TeamFormation.hpp>
#include <SopraMessages/TeamConfig.hpp>

namespace configCheck {

    bool checkTeamConfig(const communication::messages::request::TeamConfig &config);

    bool checkTeamFormation(const communication::messages::request::TeamFormation &formation, const gameHandling::TeamSide &side);

}

#endif //SERVER_CONFIGCHECK_H
