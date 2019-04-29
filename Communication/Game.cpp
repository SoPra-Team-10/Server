/**
 * @file Game.cpp
 * @author paul
 * @date 28.04.19
 * @brief Game @TODO
 */

#include "Game.hpp"

namespace communication {

    Game::Game(std::string lobbyName, Client client) : lobbyName{std::move(lobbyName)} {
        this->spectators.emplace_back(std::move(client));
    }


    auto Game::getLobbyName() const -> std::string {
        return this->lobbyName;
    }

    auto Game::getLeftId() const -> int {
        return players.first.clientId;
    }

    auto Game::getRightId() const -> int {
        if (players.second.has_value()) {
            return players.second.value().clientId;
        } else {
            return -1;
        }
    }

    void Game::addSpectator(Client client) {
        this->spectators.emplace_back(std::move(client));
    }

    void Game::onMessage(const messages::Message &message) {

    }
}
