//
// Created by jonas on 07.05.19.
//

#ifndef SERVER_CONFIGCHECK_H
#define SERVER_CONFIGCHECK_H

namespace configCheck {

    static bool checkTeamConfig(const communication::messages::request::TeamConfig &config, const gameHandling::TeamSide &side);

    static bool checkTeamFormation(const communication::messages::request::TeamFormation &formation, const gameHandling::TeamSide &side);

}

#endif //SERVER_CONFIGCHECK_H
