//
// Created by bjorn on 02.05.19.
//

#ifndef SERVER_GAME_H
#define SERVER_GAME_H

#include <SopraNetwork/Listener.hpp>
#include <Communication/Messages/Broadcast/MatchConfig.hpp>
#include <Communication/Messages/Broadcast/Next.hpp>
#include <Communication/Messages/Request/TeamConfig.hpp>
#include <Communication/Messages/Request/TeamFormation.hpp>

enum Side{
    left,right
};

class Game {
public:
    Game(communication::messages::broadcast::MatchConfig matchConfig, communication::messages::request::TeamConfig teamConfig1, communication::messages::request::TeamConfig teamConfig2, communication::messages::request::TeamFormation teamFormation1, communication::messages::request::TeamFormation teamFormation2);
    void pause();
    void resume();
    communication::messages::broadcast::Next getNextActor();
    bool executeDelta();
};


#endif //SERVER_GAME_H
