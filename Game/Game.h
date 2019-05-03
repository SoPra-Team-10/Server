//
// Created by bjorn on 02.05.19.
//

#ifndef SERVER_GAME_H
#define SERVER_GAME_H

#include <SopraNetwork/Listener.hpp>
#include <SopraMessages/MatchConfig.hpp>
#include <SopraMessages/Next.hpp>
#include <SopraMessages/TeamConfig.hpp>
#include <SopraMessages/TeamFormation.hpp>

class Game {
public:
    Game(communication::messages::broadcast::MatchConfig matchConfig, communication::messages::request::TeamConfig teamConfig1, communication::messages::request::TeamConfig teamConfig2, communication::messages::request::TeamFormation teamFormation1, communication::messages::request::TeamFormation teamFormation2);
    void pause();
    void resume();
    communication::messages::broadcast::Next getNextActor();
    bool executeDelta();
};


#endif //SERVER_GAME_H
