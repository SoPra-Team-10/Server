/**
 * @file Game.cpp
 * @author paul
 * @date 28.04.19
 * @brief Game @TODO
 */

#include "Game.hpp"

namespace communication {

    Game::Game(std::string lobbyName, Client client) : lobbyName{std::move(lobbyName)} {
        clients.first = std::move(client);
    }

    void Game::addSecond(Client ) {
        if (clients.second.has_value()) {
            throw std::runtime_error{"Already two clients in the lobby!"};
        }
    }

    void Game::onMessageLeft(const messages::Message &) {

    }

    void Game::onMessageRight(const messages::Message &) {

    }

    auto Game::getLobbyName() const -> std::string {
        return this->lobbyName;
    }

    auto Game::getLeftId() const -> int {
        return 0;
    }

    auto Game::getRightId() const -> int {
        return 0;
    }
}
