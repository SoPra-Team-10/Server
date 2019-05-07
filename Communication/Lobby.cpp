#include <memory>

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
        replay{{name, startTime,matchConfig}, {name, startTime, matchConfig}},
        matchConfig{matchConfig}, log{log} {
        this->clients.emplace(id, client);
        this->sendSingle(messages::unicast::JoinResponse{"Welcome to the Lobby"}, id);
        this->sendAll(messages::broadcast::LoginGreeting{client.userName});
        replay.first.addSpectator(client.userName);
        replay.second.addSpectator(client.userName);
    }

    void Lobby::addSpectator(Client client, int id) {
        this->clients.emplace(id, client);
        replay.first.addSpectator(client.userName);
        replay.second.addSpectator(client.userName);
        this->sendSingle(messages::unicast::JoinResponse{"Welcome to the Lobby"}, id);
        this->sendAll(messages::broadcast::LoginGreeting{client.userName});
        if (state == LobbyState::GAME) {
            sendSingle(communication::messages::unicast::Reconnect{
                    messages::broadcast::MatchStart{
                        matchConfig,teamConfigs.first.value(),teamConfigs.second.value(),
                        clients.at(players.first.value()).userName,
                        clients.at(players.second.value()).userName},
                        game->getSnapshot(), lastNext.value()
            }, id);
        }
    }

    template<>
    void Lobby::onPayload<messages::request::SendDebug>(const messages::request::SendDebug &, int) {
        // Nothing to do...
        log.debug("Got a debug message");
    }

    template<>
    void Lobby::onPayload(const messages::request::GetReplay&, int id) {
        if (state == LobbyState::FINISHED) {
            this->sendSingle(replay.first, id);
        } else if (state == LobbyState::INITIAL) {
            std::stringstream sstream;
            sstream << this->name << "_replay.json";
            auto fname = sstream.str();
            if (std::filesystem::exists(fname)) {
                std::ifstream ifstream{fname};
                nlohmann::json j;
                ifstream >> j;
                auto fileReplay = j.get<messages::broadcast::Replay>();
                this->sendSingle(fileReplay, id);
            } else {
                sendWarn(messages::request::GetReplay::getName(), "Game hasn't started yet and no old games are available", id);
            }
        } else {
            sendWarn(messages::request::GetReplay::getName(), "Game is currently running!", id);
        }
    }

    template <>
    void Lobby::onPayload(const messages::mods::request::GetReplayWithSnapshot&, int id) {
        if (state == LobbyState::FINISHED) {
            this->sendSingle(replay.second, id);
        } else if (state == LobbyState::INITIAL) {
            std::stringstream sstream;
            sstream << this->name << "_replaySnapshot.json";
            auto fname = sstream.str();
            if (std::filesystem::exists(fname)) {
                std::ifstream ifstream{fname};
                nlohmann::json j;
                ifstream >> j;
                auto fileReplay = j.get<messages::mods::unicast::ReplayWithSnapshot>();
                this->sendSingle(fileReplay, id);
            } else {
                sendWarn(messages::request::GetReplay::getName(), "Game hasn't started yet and no old games are available", id);
            }
        } else {
            sendWarn(messages::request::GetReplay::getName(), "Game is currently running!", id);
        }
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
            replay.first.setLeftTeamConfig(teamConfigs.first.value());
            replay.first.setLeftTeamUserName(clients.at(players.first.value()).userName);
            replay.first.setRightTeamConfig(teamConfigs.second.value());
            replay.first.setRightTeamUserName(clients.at(players.second.value()).userName);
            replay.second.setLeftTeamConfig(teamConfigs.first.value());
            replay.second.setLeftTeamUserName(clients.at(players.first.value()).userName);
            replay.second.setRightTeamConfig(teamConfigs.second.value());
            replay.second.setRightTeamUserName(clients.at(players.second.value()).userName);
            teamFormationTimer.setTimeout(
                    std::bind(&Lobby::onTeamFormationTimeout, this), matchConfig.getTeamFormationTimeout());
        } else {
            this->kickUser(id);
            log.warn("Got more than two teamConfigs, kicking user");
        }
    }

    template<>
    void Lobby::onPayload(const messages::request::TeamFormation &teamFormation, int id) {
        if (this->state == LobbyState::WAITING_FORMATION) {
            if (players.first == id || players.second == id) {
                if (players.first == id) {
                    log.debug("Got teamFormation for left team");
                    if (teamFormations.first.has_value()) {
                        kickUser(id);
                        log.warn("Player 1 sent two teamFormations");
                    } else {
                        teamFormations.first = teamFormation;
                    }
                } else if (players.second == id) {
                    log.debug("Got teamFormation for right team");
                    if (teamFormations.second.has_value()) {
                        kickUser(id);
                        log.warn("Player 2 sent two teamFormations");
                    } else {
                        teamFormations.second = teamFormation;
                    }
                }

                if (teamFormations.first.has_value() && teamFormations.second.has_value()) {
                    teamFormationTimer.stop();
                    state = LobbyState::GAME;
                    log.info("Starting game");

                    game.emplace(matchConfig, teamConfigs.first.value(), teamConfigs.second.value(),
                            teamFormations.first.value(), teamFormations.second.value());
                    game->timeoutListener(std::bind(&Lobby::onTimeout, this, std::placeholders::_1));
                    game->winListener(std::bind(&Lobby::onWin, this, std::placeholders::_1,
                                                       std::placeholders::_2));
                    auto snapshot = game->getSnapshot();
                    this->sendAll(snapshot);
                    replay.first.setFirstSnapshot(snapshot);
                    replay.second.setFirstSnapshot(snapshot);
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
    void Lobby::onPayload(const messages::request::DeltaRequest &deltaRequest, int clientId) {
        if (clientId == players.first || clientId == players.second) {
            if (state == LobbyState::GAME) {
                if (game->executeDelta(deltaRequest)) {
                    this->sendAll(deltaRequest);
                    auto snapshot = game->getSnapshot();
                    this->sendAll(snapshot);
                    auto next = game->getNextActor();
                    lastNext = next;
                    this->sendAll(next);
                    if (next.getEntityId() == messages::types::EntityId::SNITCH
                            || next.getEntityId() == messages::types::EntityId::BLUDGER1
                            || next.getEntityId() == messages::types::EntityId::BLUDGER2
                            || next.getEntityId() == messages::types::EntityId::QUAFFLE) {
                        this->sendAll(game->executeBallDelta(next.getEntityId()));
                    }
                    replay.first.addLog(communication::messages::Message{snapshot.getLastDeltaBroadcast()});
                    replay.second.addLog(communication::messages::Message{snapshot});
                    replay.second.addLog(communication::messages::Message{next});
                } else {
                    // According to the spec the user needs to get kicked
                    sendError(messages::request::DeltaRequest::getName(),
                              "Invalid delta request!", clientId);
                    this->kickUser(clientId);
                    log.warn("Client sent an invalid deltaRequest");
                }
            } else {
                sendError(messages::request::DeltaRequest::getName(),
                          "Not in game", clientId);
                this->kickUser(clientId);
                log.warn("Client sent a DeltaRequest while not in game");
            }
        } else {
            this->kickUser(clientId);
            log.warn("Spectator send a DeltaRequest, kicking user");
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
            sendError(messages::request::PauseRequest::getName(),
                      "Invalid pause request: either AI or Game not started", id);
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
            sendError(messages::request::ContinueRequest::getName(),
                      "Not paused!", id);
            this->kickUser(id);
            log.warn("Client sent a continue while not paused");
        }
    }

    template <>
    void Lobby::onPayload(const messages::mods::request::SendChat &sendChat, int id) {
        this->sendAll(messages::mods::broadcast::GlobalChat{clients.at(id).userName, sendChat.getInformation()});
    }

    template<typename T>
    void Lobby::onPayload(const T &, int client) {
        sendError(T::getName(),
                  "The message is not a request!", client);
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

    void Lobby::onTeamFormationTimeout() {
        if (!teamFormations.first.has_value() && !teamFormations.second.has_value()) {
            auto formerPlayers = players;
            players.first.reset();
            players.second.reset();
            sendAll(messages::broadcast::MatchFinish{0, 0, 0, "",messages::types::VictoryReason::VIOLATION_OF_PROTOCOL});
            kickUser(formerPlayers.first.value());
            kickUser(formerPlayers.second.value());
        } else if (!teamFormations.first.has_value()) {
            kickUser(players.first.value());
        } else if (!teamFormations.second.has_value()) {
            kickUser(players.second.value());
        }
    }

    void Lobby::onTimeout(gameHandling::TeamSide teamSide) {
        int id;
        if (teamSide == gameHandling::TeamSide::LEFT) {
            id = players.first.value();
        } else {
            id = players.second.value();
        }
        sendError("None", "Timeout", id);
        this->kickUser(id);
    }

    void Lobby::onWin(gameHandling::TeamSide teamSide, communication::messages::types::VictoryReason victoryReason) {
        int winnerId;
        if (teamSide == gameHandling::TeamSide::LEFT) {
            winnerId = players.first.value();
        } else {
            winnerId = players.second.value();
        }
        std::string winner = clients.at(winnerId).userName;

        messages::broadcast::MatchFinish matchFinish;
        if (game) {
            matchFinish = {game->getRound(),
                           game->getLeftPoints(),
                           game->getRightPoints(), winner, victoryReason};
        } else {
            matchFinish = {0,0,0, winner, victoryReason};
        }
        replay.second.addLog(messages::Message{matchFinish});
        replay.first.addLog(messages::Message{matchFinish});
        this->sendAll(matchFinish);
        state = LobbyState::FINISHED;
        this->game.reset();

        std::stringstream sstream;
        sstream << this->name << "_replay.json";
        auto fname = sstream.str();
        std::ofstream ofstream{fname};
        if (ofstream.good()) {
            nlohmann::json j = this->replay.first;
            ofstream << j.dump();
        } else {
            log.warn("Can not write replay to file!");
        }

        std::stringstream sstreamSnapshot;
        sstream << this->name << "_replaySnapshot.json";
        auto fnameSnapshot = sstreamSnapshot.str();
        std::ofstream ofstreamSnapshot{fnameSnapshot};
        if (ofstreamSnapshot.good()) {
            nlohmann::json jSnapshot = this->replay.second;
            ofstreamSnapshot << jSnapshot.dump();
        } else {
            log.warn("Can not write replay to file!");
        }
    }

    bool Lobby::onLeave(int id) {
        if (id == players.first || id == players.second) {
            if (id == players.first) {
                if (players.second.has_value()) {
                    onWin(gameHandling::TeamSide::RIGHT, messages::types::VictoryReason::VIOLATION_OF_PROTOCOL);
                }
            } else {
                if (players.first.has_value()) {
                    onWin(gameHandling::TeamSide::LEFT, messages::types::VictoryReason::VIOLATION_OF_PROTOCOL);
                }
            }
        }
        clients.erase(clients.find(id));
        communicator.removeClient(id);
        return getUserInLobby() <= 0;
    }


    void Lobby::sendAll(const messages::Payload &payload) {
        for (const auto &c : clients) {
            this->sendSingle(payload,c.first);
        }
    }

    void Lobby::sendSingle(const messages::Payload &payload, int id) {
        this->communicator.send(messages::Message{payload}, id);
    }

    void Lobby::sendSingle(const messages::ReplayPayload &payload, int id) {
        this->communicator.send(messages::ReplayMessage{payload}, id);
    }

    void Lobby::sendError(const std::string &payloadReason, const std::string &msg, int id) {
        if (clients.at(id).mods.count(messages::types::Mods::ERROR) > 0) {
            this->sendSingle(communication::messages::mods::unicast::PrivateError{payloadReason, msg}, id);
        } else {
            std::stringstream sstream;
            sstream << "Error in " << payloadReason << ":\t" << msg;
            this->sendSingle(communication::messages::unicast::PrivateDebug{sstream.str()}, id);
        }
    }

    void Lobby::sendWarn(const std::string &payloadReason, const std::string &msg, int id) {
        if (clients.at(id).mods.count(messages::types::Mods::WARNING) > 0) {
            this->sendSingle(communication::messages::mods::unicast::PrivateWarning{payloadReason, msg}, id);
        } else {
            std::stringstream sstream;
            sstream << "Warning in " << payloadReason << ":\t" << msg;
            this->sendSingle(communication::messages::unicast::PrivateDebug{sstream.str()}, id);
        }
    }

    auto Lobby::getUserInLobby() const -> int {
        return clients.size();
    }

    auto Lobby::isMatchStarted() const -> bool {
        return state == LobbyState::GAME;
    }

    auto Lobby::getName() const -> std::string {
        return name;
    }

}
