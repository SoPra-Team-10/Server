/**
 * @file Game.cpp
 * @author paul
 * @date 28.04.19
 * @brief Game @TODO
 */

#include "Game.hpp"
#include "Communicator.hpp"

namespace communication {

    Game::Game(Communicator &communicator, Client client, int id) : communicator{communicator} {
        this->clients.emplace(id, std::move(client));
    }

    void Game::addSpectator(Client client, int id) {
        this->clients.emplace(id, std::move(client));
    }

    template<>
    void Game::onPayload<messages::request::SendDebug>(const messages::request::SendDebug &, int) {
        // Nothing to do...
    }

    template<>
    void Game::onPayload(const messages::request::TeamConfig&, int id) {
        if (!players.first.has_value()) {
            players.first = id;
        } else if (!players.second.has_value()) {
            players.second = id;
            //@TODO match start
        } else {
            //@TODO only two players, match end
        }
    }

    template<>
    void Game::onPayload(const messages::request::GetReplay&, int ) {
        // @TODO send replay if game is finished
    }

    template<>
    void Game::onPayload(const messages::request::TeamFormation&, int id) {
        if (players.first == id) {
            //@TODO create a new GameControllerInstance
            //@TODO send snapshot
        } else if (players.second == id) {
            //@TODO create a new GameControllerInstance
            //@TODO send snapshot
        } else {
            //@TODO some spectator sent a TeamFormation
        }
    }

    template<>
    void Game::onPayload(const messages::request::PauseRequest &pauseRequest, int id) {
        messages::broadcast::PauseResponse pauseResponse{
            pauseRequest.getMessage(), this->clients.at(id).userName, true};
        this->sendAll(pauseResponse);
    }

    template<>
    void Game::onPayload(const messages::request::ContinueRequest &continueRequest, int id) {
        messages::broadcast::PauseResponse pauseResponse{
                continueRequest.getMessage(), this->clients.at(id).userName, false};
        this->sendAll(pauseResponse);
    }

    template<>
    void Game::onPayload(const messages::request::DeltaRequest &, int) {
        // @TODO wait for GameController
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

    void Game::sendRight(const messages::Payload &payload) {
        if (players.second.has_value()) {
            this->sendSingle(payload, players.second.value());
        }
    }

    void Game::sendAll(const messages::Payload &payload) {
        for (const auto &c : this->clients) {
            this->sendSingle(payload, c.first);
        }
    }

    void Game::sendSingle(const messages::Payload &payload, int id) {
        this->communicator.send(messages::Message{payload}, id);
    }

    void Game::sendLeft(const messages::Payload &payload) {
        if (players.first.has_value()) {
            this->sendSingle(payload, players.first.value());
        }
    }

}
