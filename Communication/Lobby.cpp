/**
 * @file Game.cpp
 * @author paul
 * @date 28.04.19
 * @brief Definition of the game class
 */

#include <filesystem>
#include <fstream>
#include "Lobby.hpp"
#include "Communicator.hpp"

namespace communication {

    Lobby::Lobby(const std::string &name, const std::string &startTime, Communicator &communicator,
            const Client& client, int id, util::Logging &log,
            const messages::broadcast::MatchConfig &matchConfig)
        : communicator{communicator}, state{LobbyState::INITIAL}, name{name},
        replay{name, startTime,matchConfig},
        matchConfig{matchConfig}, log{log} {
        this->clients.emplace(id, client);
        this->sendSingle(messages::unicast::JoinResponse{"Welcome to the Lobby"}, id);
        this->sendAll(messages::broadcast::LoginGreeting{client.userName});
        replay.addSpectator(client.userName);
    }

    void Lobby::addSpectator(Client client, int id) {
        this->clients.emplace(id, client);
        replay.addSpectator(client.userName);
        this->sendSingle(messages::unicast::JoinResponse{"Welcome to the Lobby"}, id);
        this->sendAll(messages::broadcast::LoginGreeting{client.userName});
        if (state == LobbyState::GAME) {
            sendSingle(communication::messages::unicast::Reconnect{
                    messages::broadcast::MatchStart{
                        matchConfig,teamConfigs.first.value(),teamConfigs.second.value(),
                        clients.at(players.first.value()).userName,
                        clients.at(players.second.value()).userName},
                    game.value().getSnapshot()
            }, id);
        }
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
            replay.setLeftTeamConfig(teamConfigs.first.value());
            replay.setLeftTeamUserName(clients.at(players.first.value()).userName);
            replay.setRightTeamConfig(teamConfigs.second.value());
            replay.setRightTeamUserName(clients.at(players.second.value()).userName);
        } else {
            this->kickUser(id);
            log.warn("Got more than two teamConfigs, kicking user");
        }
    }

    template<>
    void Lobby::onPayload(const messages::request::GetReplay&, int id) {
        if (state == LobbyState::FINISHED) {
            this->sendSingle(replay, id);
        } else if (state == LobbyState::INITIAL) {
            std::stringstream sstream;
            sstream << this->name << ".json";
            auto fname = sstream.str();
            if (std::filesystem::exists(fname)) {
                std::ifstream ifstream{fname};
                nlohmann::json j;
                ifstream >> j;
                auto fileReplay = j.get<messages::broadcast::Replay>();
                this->sendSingle(fileReplay, id);
            } else {
                this->sendSingle(messages::unicast::PrivateDebug{"Game hasn't started yet and not old games are available"}, id);
            }
        } else {
            this->sendSingle(messages::unicast::PrivateDebug{"Game is currently running!"}, id);
        }
    }

    template<>
    void Lobby::onPayload(const messages::request::TeamFormation &teamFormation, int id) {
        if (this->state == LobbyState::WAITING_FORMATION) {
            if (players.first == id || players.second == id) {
                log.debug("Got teamFormation for left team");

                if (players.first == id) {
                    if (teamFormations.first.has_value()) {
                        kickUser(id);
                        log.warn("Player 1 sent two teamFormations");
                    } else {
                        teamFormations.first = teamFormation;
                    }
                } else if (players.second == id) {
                    if (teamFormations.second.has_value()) {
                        kickUser(id);
                        log.warn("Player 2 sent two teamFormations");
                    } else {
                        teamFormations.second = teamFormation;
                    }
                }

                if (teamFormations.first.has_value() && teamFormations.second.has_value()) {
                    game.emplace(Game{matchConfig, teamConfigs.first.value(), teamConfigs.second.value(),
                                      teamFormations.first.value(), teamFormations.second.value()});
                    game.value().timeoutListener(std::bind(&Lobby::onTimeout, this, std::placeholders::_1));
                    game.value().winListener(std::bind(&Lobby::onWin, this, std::placeholders::_1,
                                                       std::placeholders::_2));
                    state = LobbyState::GAME;
                    log.info("Starting game");
                    auto snapshot = game.value().getSnapshot();
                    this->sendAll(snapshot);
                    replay.setFirstSnapshot(snapshot);
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
                    auto snapshot = game.value().getSnapshot();
                    this->sendAll(snapshot);
                    replay.addLog(communication::messages::Message{snapshot.getLastDeltaBroadcast()});
                    auto next = game.value().getNextActor();
                    lastNext = next;
                    this->sendAll(next);
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

    void Lobby::kickUser(int id) {
        if (id != players.first && id != players.second) {
            // Kick a spectator by sending a MatchFinish message without a winner
            messages::broadcast::MatchFinish matchFinish{0, 0, 0, "",
                                                         messages::types::VictoryReason::VIOLATION_OF_PROTOCOL};
            this->sendSingle(matchFinish, id);
        }
        onLeave(id);
    }

    void Lobby::onLeave(int id) {
        if (id == players.first || id == players.second) {
            if (id == players.first) {
                if (players.second.has_value()) {
                    onWin(TeamSide::RIGHT, messages::types::VictoryReason::VIOLATION_OF_PROTOCOL);
                }
            } else {
                if (players.first.has_value()) {
                    onWin(TeamSide::LEFT, messages::types::VictoryReason::VIOLATION_OF_PROTOCOL);
                }
            }
        }
        clients.erase(clients.find(id));
        communicator.removeClient(id);
    }

    void Lobby::onTimeout(TeamSide teamSide) {
        int id;
        if (teamSide == TeamSide::LEFT) {
            id = players.first.value();
        } else {
            id = players.second.value();
        }
        this->sendSingle(messages::unicast::PrivateDebug{"Timeout"},id);
        this->kickUser(id);
    }

    void Lobby::onWin(TeamSide teamSide, communication::messages::types::VictoryReason victoryReason) {
        int winnerId;
        if (teamSide == TeamSide::LEFT) {
            winnerId = players.first.value();
        } else {
            winnerId = players.second.value();
        }
        std::string winner = clients.at(winnerId).userName;

        messages::broadcast::MatchFinish matchFinish{game.value().getEndRound(),
                                                     game.value().getLeftPoints(),
                                                     game.value().getRightPoints(),winner, victoryReason};
        this->sendAll(matchFinish);
        state = LobbyState::FINISHED;
    }

    void Lobby::sendAll(const messages::Payload &payload) {
        for (const auto &c : clients) {
            this->sendSingle(payload,c.first);
        }
    }

    void Lobby::sendSingle(const messages::Payload &payload, int id) {
        this->communicator.send(messages::Message{payload}, id);
    }

    void Lobby::sendSingle(const messages::broadcast::Replay &payload, int id) {
        this->communicator.send(messages::ReplayMessage{payload}, id);
    }

}
