/**
 * @file Game.cpp
 * @author paul
 * @date 28.04.19
 * @brief Game @TODO
 */

#include "Game.hpp"

namespace communication {

    Game::Game(Client client) {
        this->spectators.emplace_back(std::move(client));
    }

    void Game::addSpectator(Client client) {
        this->spectators.emplace_back(std::move(client));
    }

    template<>
    void Game::onPayload<messages::request::SendDebug>(const messages::request::SendDebug &, int) {

    }

    template<typename T>
    void Game::onPayload(const T &, int) {
        //@TODO we shouldn't be here
    }

    void Game::onMessage(const messages::Message &message, int id) {
        std::visit([id, this](const auto &payload){
            this->onPayload(payload, id);
        }, message.getPayload());
    }

}
