//
// Created by bjorn on 02.05.19.
//

#ifndef SERVER_GAME_H
#define SERVER_GAME_H

#include <SopraGameLogic/SopraGameModel.h>
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
    gameModel::Environment env;
    util::Listener<Side > TOListener;
    util::Listener<Side > GameOverEvent;
    gameModel::Config config;


    Game(communication::messages::broadcast::MatchConfig matchConfig, communication::messages::request::TeamConfig teamConfig1, communication::messages::request::TeamConfig teamConfig2, communication::messages::request::TeamFormation teamFormation1, communication::messages::request::TeamFormation teamFormation2);
    void pause();
    void resume();
    communication::messages::broadcast::Next getNextActor();
    bool executeDelta();

    static std::map<communication::messages::types::Broom, gameModel::Broom > broomMap;
};


#endif //SERVER_GAME_H
