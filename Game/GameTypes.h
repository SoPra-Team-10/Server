//
// Created by timluchterhand on 06.05.19.
//

#ifndef SERVER_GAMETYPES_H
#define SERVER_GAMETYPES_H
namespace gameHandling{
    enum class GameState {
        BallPhase,
        PlayerPhase,
        InterferencePhase
    };

    enum class TeamSide : char {
        LEFT, RIGHT
    };
}

#endif //SERVER_GAMETYPES_H
