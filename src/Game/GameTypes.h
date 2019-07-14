//
// Created by timluchterhand on 06.05.19.
//

#ifndef SERVER_GAMETYPES_H
#define SERVER_GAMETYPES_H

#include <SopraMessages/types.hpp>

namespace gameHandling {
    /**
     * Different game phases
     */
    enum class GameState {
        BallPhase,
        PlayerPhase,
        InterferencePhase
    };

    /**
     * Different timeouts for player turns
     */
    struct Timeouts{
        const int playerTurn, fanTurn, unbanTurn;
    };
}

#endif //SERVER_GAMETYPES_H
