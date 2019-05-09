//
// Created by timluchterhand on 06.05.19.
//

#ifndef SERVER_SETUP_H
#define SERVER_SETUP_H

#include <Game/Game.h>
namespace setup{
    auto createGame(util::Logging &log) -> gameHandling::Game;
    auto createEnv(const gameModel::Config &config) -> std::shared_ptr<gameModel::Environment>;
}

#endif //SERVER_SETUP_H
