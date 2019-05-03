/**
 * @file Game.cpp
 * @author paul
 * @date 28.04.19
 * @brief Definition of the game class
 */

#include "Lobby.hpp"
#include "Communicator.hpp"

namespace communication {

    Lobby::Lobby(Communicator &communicator, Client client, int id, util::Logging &log,
            const messages::broadcast::MatchConfig &matchConfig)
        : communicator{communicator}, state{LobbyState::INITIAL}, matchConfig{matchConfig}, log{log} {
        this->sendSingle(messages::unicast::JoinResponse{"Welcome to the Lobby"}, id);
        this->sendAll(messages::broadcast::LoginGreeting{client.userName});
        this->clients.emplace(id, std::move(client));
    }

    void Lobby::addSpectator(Client client, int id) {
        this->sendSingle(messages::unicast::JoinResponse{"Welcome to the Lobby"}, id);
        this->sendAll(messages::broadcast::LoginGreeting{client.userName});
        this->clients.emplace(id, std::move(client));
    }

    template<>
    void Lobby::onPayload<messages::request::SendDebug>(const messages::request::SendDebug &, int) {
        // Nothing to do...
        log.debug("Got a debug message");
    }

    template<>
    void Lobby::onPayload(const messages::request::TeamConfig &teamConfig, int id) {
        if (!players.first.has_value()) {
            players.first = id;
            log.debug("Got first teamConfig");
            teamConfigs.first = teamConfig;
        } else if (!players.second.has_value()) {
            players.second = id;
            log.info("Got second teamConfig, startingMatch");
            teamConfigs.second = teamConfig;
            this->sendAll(messages::broadcast::MatchStart{
                matchConfig,teamConfigs.first.value(),teamConfigs.second.value(),
                clients.at(players.first.value()).userName,
                clients.at(players.second.value()).userName});
            this->state = LobbyState::WAITING_FORMATION;
        } else {
            this->kickUser(id);
            log.warn("Got more than two teamConfigs, kicking user");
        }
    }

    template<>
    void Lobby::onPayload(const messages::request::GetReplay&, int id) {
        if (state == LobbyState::FINISHED) {
            // @TODO send replay if game is finished, optional
        } else {
            this->sendSingle(messages::unicast::PrivateDebug{"Replay not implemented!"}, id);
        }
    }

    template<>
    void Lobby::onPayload(const messages::request::TeamFormation &teamFormation, int id) {
        if (this->state == LobbyState::WAITING_FORMATION) {
            if (players.first == id) {
                log.debug("Got teamFormation for left team");
                if (firstTeamFormation.has_value()) {
                    game = Game{matchConfig, teamConfigs.first.value(), teamConfigs.second.value(),
                                teamFormation, firstTeamFormation.value()};
                    state = LobbyState::GAME;
                    log.info("Starting game");
                    this->sendAll(game.value().getSnapshot());
                } else {
                    firstTeamFormation = teamFormation;
                }
            } else if (players.second == id) {
                log.debug("Got teamFormation for right team");
                if (firstTeamFormation.has_value()) {
                    game = Game{matchConfig, teamConfigs.first.value(), teamConfigs.second.value(),
                                firstTeamFormation.value(), teamFormation};
                    state = LobbyState::GAME;
                    log.info("Starting game");
                    this->sendAll(game.value().getSnapshot());
                } else {
                    firstTeamFormation = teamFormation;
                }
            } else {
                this->kickUser(id);
                log.warn("Got teamFormation from a spectator, kicking user");
            }
        } else {
            this->kickUser(id);
            log.warn("Got teamFormation in wrong state");
        }
    }

    template<>
    void Lobby::onPayload(const messages::request::PauseRequest &pauseRequest, int id) {
        if (!clients.at(id).isAi && (state == LobbyState::GAME || state == LobbyState::PAUSE)) {
            messages::broadcast::PauseResponse pauseResponse{
                    pauseRequest.getMessage(), this->clients.at(id).userName, true};
            this->sendAll(pauseResponse);
            log.info("Pause");
            state = LobbyState::PAUSE;
        } else {
            this->sendSingle(messages::unicast::PrivateDebug{"Invalid pause request either AI or Game not started"}, id);
            this->kickUser(id);
            log.warn("Invalid pause request");
        }
    }

    template<>
    void Lobby::onPayload(const messages::request::ContinueRequest &continueRequest, int id) {
        if (state == LobbyState::PAUSE) {
            messages::broadcast::PauseResponse pauseResponse{
                    continueRequest.getMessage(), this->clients.at(id).userName, false};
            this->sendAll(pauseResponse);
            log.info("Continue");
            state = LobbyState::GAME;
        } else {
            this->sendSingle(messages::unicast::PrivateDebug{"Not paused"}, id);
            this->kickUser(id);
            log.warn("Client sent a continue while not paused");
        }
    }

    template<>
    void Lobby::onPayload(const messages::request::DeltaRequest &deltaRequest, int clientId) {
        if (clientId == players.first || clientId == players.second) {
            if (state == LobbyState::GAME) {
                if (game.value().executeDelta(deltaRequest)) {
                    this->sendAll(game.value().getSnapshot());
                } else {
                    // According to the spec the user needs to get kicked
                    this->kickUser(clientId);
                    this->sendSingle(messages::unicast::PrivateDebug{"Not in game"}, clientId);
                    log.warn("Client sent an invalid deltaRequest");
                }
            } else {
                this->kickUser(clientId);
                this->sendSingle(messages::unicast::PrivateDebug{"Not in game"}, clientId);
                log.warn("Client sent a DeltaRequest while not in game");
            }
        } else {
            this->kickUser(clientId);
            log.warn("Spectator send a DeltaRequest, kicking user");
        }
    }

    template<typename T>
    void Lobby::onPayload(const T &, int client) {
        this->kickUser(client);
        log.warn("Received message is not a request, kicking user");
    }

    void Lobby::onMessage(const messages::Message &message, int id) {
        std::visit([id, this](const auto &payload){
            this->onPayload(payload, id);
        }, message.getPayload());
    }

    void Lobby::sendAll(const messages::Payload &payload) {
        for (const auto &c : this->clients) {
            this->sendSingle(payload, c.first);
        }
    }

    void Lobby::sendSingle(const messages::Payload &payload, int id) {
        this->communicator.send(messages::Message{payload}, id);
    }

    void Lobby::kickUser(int id) {
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
            messages::broadcast::MatchFinish matchFinish{game.value().getEndRound(),
                                                         game.value().getLeftPoints(),
                                                         game.value().getRightPoints(),winner,
                                                         messages::types::VictoryReason::VIOLATION_OF_PROTOCOL};
            this->sendAll(matchFinish);
        } else {
            // Kick a spectator by sending a MatchFinish message without a winner
            messages::broadcast::MatchFinish matchFinish{0,0,0,"",
                                                         messages::types::VictoryReason::VIOLATION_OF_PROTOCOL};
            this->sendSingle(matchFinish, id);
        }
        clients.erase(clients.find(id));
        communicator.removeClient(id);
    }

    void Lobby::onLeave(int id) {
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
            messages::broadcast::MatchFinish matchFinish{game.value().getEndRound(),
                                                         game.value().getLeftPoints(),
                                                         game.value().getRightPoints(),winner,
                                                         messages::types::VictoryReason::VIOLATION_OF_PROTOCOL};
            this->sendAll(matchFinish);
        }
        clients.erase(clients.find(id));
        communicator.removeClient(id);
    }

}
