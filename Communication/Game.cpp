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
            this->kickUser(id);
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
            this->kickUser(id);
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
    void Game::onPayload(const T &, int client) {
        // We really shouldn't be here
        this->kickUser(client);
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

    void Game::kickUser(int id) {
        if (id == players.first || id == players.second) {
            std::string winner;
            if (id == players.first) {
                if (players.second.has_value()) {
                    winner = clients.at(players.second.value()).userName;
                }
            } else {
                if (players.first.has_value()) {
                    winner = clients.at(players.first.value()).userName;
                }
            }
            // @TODO fill endRound, leftPoints, rightPoints
            messages::broadcast::MatchFinish matchFinish{0,0,0,winner,
                                                         messages::types::VictoryReason::VIOLATION_OF_PROTOCOL};
        } else {
            // Kick a spectator by sending a MatchFinish message without a winner
            messages::broadcast::MatchFinish matchFinish{0,0,0,"",
                                                         messages::types::VictoryReason::VIOLATION_OF_PROTOCOL};
        }
    }

}
