/**
 * @file Lobby.cpp
 * @author paul
 * @date 28.04.19
 * @brief Definition of the lobby class
 */


#include <memory>
#include <filesystem>
#include <fstream>
#include <Game/ConfigCheck.h>
#include <SopraGameLogic/GameModel.h>
#include <SopraGameLogic/conversions.h>
#include "Lobby.hpp"
#include "Communicator.hpp"

namespace communication {
    constexpr auto RECONNECT_TIMEOUT = 30000;

    Lobby::Lobby(const std::string &name, const std::string &startTime, Communicator &communicator,
            const Client& client, int id, util::Logging &log,
            const messages::broadcast::MatchConfig &matchConfig)
             : log{log, name}, animationQueue{communicator}, state{LobbyState::INITIAL},
                communicator{communicator}, matchConfig{matchConfig}, name{name},
                replay{{name, startTime,matchConfig}, {name, startTime, matchConfig}} {
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
        if (state == LobbyState::GAME && lastSnapshot.has_value() && lastNext.has_value()) {
            sendSingle(communication::messages::unicast::Reconnect{
                    messages::broadcast::MatchStart{
                        matchConfig,teamConfigs.first.value(),teamConfigs.second.value(),
                        clients.at(players.first.value()).userName,
                        clients.at(players.second.value()).userName},
                        lastSnapshot.value(), lastNext.value()
            }, id);

            if (client.mods.count(messages::types::Mods::CHAT) > 0) {
                std::vector<std::pair<std::string, std::string>> messages{lastTenMessages.begin(), lastTenMessages.end()};
                sendSingle(communication::messages::mods::unicast::ReconnectChat{messages}, id);
            }
        }
    }

    auto Lobby::reAddUser(const Client& client, int id) -> std::optional<int> {
        std::optional<int> oldId;
        for (auto & currentClient : clients) {
            if (client == currentClient.second) {
                oldId = currentClient.first;
                if (players.first.has_value() && oldId == players.first.value()) {
                    players.first.emplace(id);
                } else if (players.second.has_value() && oldId == players.second.value()) {
                    players.second.emplace(id);
                }
            }
        }

        if (oldId.has_value()) {
            leaveTimers.erase(oldId.value());
            clients.erase(oldId.value());
            clients.emplace(id, client);
            sendSingle(communication::messages::unicast::Reconnect{
                    messages::broadcast::MatchStart{
                            matchConfig,teamConfigs.first.value(),teamConfigs.second.value(),
                            clients.at(players.first.value()).userName,
                            clients.at(players.second.value()).userName},
                    lastSnapshot.value(), lastNext.value()
            }, id);
            if (client.mods.count(messages::types::Mods::CHAT) > 0) {
                std::vector<std::pair<std::string, std::string>> messages{lastTenMessages.begin(), lastTenMessages.end()};
                sendSingle(communication::messages::mods::unicast::ReconnectChat{messages}, id);
            }
            return oldId;
        } else {
            return std::nullopt;
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
            if (!configCheck::checkTeamConfig(teamConfig)) {
                sendError(messages::request::TeamConfig::getName(), "Invalid Team Config", id);
                log.warn("Got invalid team config");
                kickUser(id);
            } else {
                players.first = id;
                log.debug("Got first teamConfig");
                teamConfigs.first = teamConfig;
            }
        } else if (!players.second.has_value()) {
            if (!configCheck::checkTeamConfig(teamConfig)) {
                sendError(messages::request::TeamConfig::getName(), "Invalid Team Config", id);
                log.warn("Got invalid team config");
                kickUser(id);
            } else if (clients.find(players.first.value()) == clients.end()) {
                log.warn("First client left after sending TeamConfig");
                players.first.reset();
                players.second.reset();
                for (const auto &client : clients) {
                    sendWarn("teamConfig", "Player 1 left, restarting game", client.first);
                }
            } else {
                players.second = id;
                log.info("Got second teamConfig, startingMatch");
                teamConfigs.second = teamConfig;
                this->sendAll(messages::broadcast::MatchStart{
                        matchConfig, teamConfigs.first.value(), teamConfigs.second.value(),
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
            }
        } else {
            log.warn("Got more than two teamConfigs, kicking user");
            this->kickUser(id);
        }
    }

    template<>
    void Lobby::onPayload(const messages::request::TeamFormation &teamFormation, int id) {
        if (this->state == LobbyState::WAITING_FORMATION) {
            if (players.first == id || players.second == id) {
                if (players.first == id) {
                    log.debug("Got teamFormation for left team");
                    if (teamFormations.first.has_value()) {
                        sendError(messages::request::TeamFormation::getName(), "You sent two teamformations", id);
                        log.warn("Player 1 sent two teamFormations");
                        kickUser(id);
                    } else if (!configCheck::checkTeamFormation(teamFormation, gameModel::TeamSide::LEFT)) {
                        sendError(messages::request::TeamFormation::getName(), "TeamFormation invalid", id);
                        log.warn("Got invalid team formation");
                        kickUser(id);
                        players.first.reset();
                    } else {
                        teamFormations.first = teamFormation;
                    }
                } else if (players.second == id) {
                    log.debug("Got teamFormation for right team");
                    if (teamFormations.second.has_value()) {
                        sendError(messages::request::TeamFormation::getName(), "You sent two teamformations", id);
                        log.warn("Player 2 sent two teamFormations");
                        kickUser(id);
                    } else if (!configCheck::checkTeamFormation(teamFormation, gameModel::TeamSide::RIGHT)) {
                        sendError(messages::request::TeamFormation::getName(), "TeamFormation invalid", id);
                        log.warn("Got invalid team formation");
                        kickUser(id);
                        players.second.reset();
                    } else {
                        teamFormations.second = teamFormation;
                    }
                }

                if (teamFormations.first.has_value() && teamFormations.second.has_value()) {
                    teamFormationTimer.stop();
                    state = LobbyState::GAME;
                    log.info("Starting game");

                    game.emplace(matchConfig, teamConfigs.first.value(), teamConfigs.second.value(),
                            teamFormations.first.value(), teamFormations.second.value(), log);
                    game->timeoutListener(std::bind(&Lobby::onTimeout, this, std::placeholders::_1,
                            std::placeholders::_2));
                    modifySnapshotsAddToLogAndSend(game->getSnapshot());
                    if (game->fatalErrorEvent.has_value()) {
                        onFatalError(game->fatalErrorEvent.value());
                        return;
                    }
                    auto next = game->getNextAction();
                    modifySnapshotsAddToLogAndSend(game->getSnapshot());
                    lastNext = next;
                    this->sendAll(next);
                    replay.second.addLog(communication::messages::Message{next});
                    while (next.getEntityId() == messages::types::EntityId::SNITCH
                           || next.getEntityId() == messages::types::EntityId::BLUDGER1
                           || next.getEntityId() == messages::types::EntityId::BLUDGER2
                           || next.getEntityId() == messages::types::EntityId::QUAFFLE) {
                        game->executeBallDelta(next.getEntityId());
                        if (game->fatalErrorEvent.has_value()) {
                            onFatalError(game->fatalErrorEvent.value());
                            return;
                        } else if (game->winEvent.has_value()) {
                            onWin(std::get<0>(game->winEvent.value()), std::get<1>(game->winEvent.value()));
                            return;
                        }
                        modifySnapshotsAddToLogAndSend(game->getSnapshot());
                        if (game->fatalErrorEvent.has_value()) {
                            onFatalError(game->fatalErrorEvent.value());
                            return;
                        }
                        next = game->getNextAction();
                        modifySnapshotsAddToLogAndSend(game->getSnapshot());
                        lastNext = next;
                        sendAll(next);
                        replay.second.addLog(communication::messages::Message{next});
                    }
                }
            } else {
                log.warn("Got teamFormation from a spectator, kicking user");
                this->kickUser(id);
            }
        } else {
            log.warn("Got teamFormation in wrong state");
            this->kickUser(id);
        }
    }

    template<>
    void Lobby::onPayload(const messages::request::DeltaRequest &deltaRequest, int clientId) {
        if (clientId == players.first || clientId == players.second) {
            if (state == LobbyState::GAME) {
                gameModel::TeamSide teamSide =
                        (clientId == players.first ? gameModel::TeamSide::LEFT : gameModel::TeamSide::RIGHT);
                auto res = game->executeDelta(deltaRequest, teamSide);

                if (game->fatalErrorEvent.has_value()) {
                    onFatalError(game->fatalErrorEvent.value());
                    return;
                } else if (game->winEvent.has_value()) {
                    onWin(std::get<0>(game->winEvent.value()), std::get<1>(game->winEvent.value()));
                    return;
                }
                if (res) {
                    modifySnapshotsAddToLogAndSend(game->getSnapshot());
                    if (game->fatalErrorEvent.has_value()) {
                        onFatalError(game->fatalErrorEvent.value());
                        return;
                    }
                    auto next = game->getNextAction();
                    modifySnapshotsAddToLogAndSend(game->getSnapshot());
                    lastNext = next;
                    this->sendAll(next);
                    replay.second.addLog(communication::messages::Message{next});
                    while (next.getEntityId() == messages::types::EntityId::SNITCH
                            || next.getEntityId() == messages::types::EntityId::BLUDGER1
                            || next.getEntityId() == messages::types::EntityId::BLUDGER2
                            || next.getEntityId() == messages::types::EntityId::QUAFFLE) {
                        game->executeBallDelta(next.getEntityId());
                        if (game->fatalErrorEvent.has_value()) {
                            onFatalError(game->fatalErrorEvent.value());
                            return;
                        } else if (game->winEvent.has_value()) {
                            onWin(std::get<0>(game->winEvent.value()), std::get<1>(game->winEvent.value()));
                            return;
                        }
                        modifySnapshotsAddToLogAndSend(game->getSnapshot());
                        if (game->fatalErrorEvent.has_value()) {
                            onFatalError(game->fatalErrorEvent.value());
                            return;
                        }
                        next = game->getNextAction();
                        modifySnapshotsAddToLogAndSend(game->getSnapshot());
                        lastNext = next;
                        sendAll(next);
                        replay.second.addLog(communication::messages::Message{next});
                    }
                } else {
                    // According to the spec the user needs to get kicked
                    sendError(messages::request::DeltaRequest::getName(),
                              "Invalid delta request!", clientId);
                    log.warn("Client sent an invalid deltaRequest");
                    this->kickUser(clientId);
                }
            } else {
                sendError(messages::request::DeltaRequest::getName(),
                          "Not in game", clientId);
                log.warn("Client sent a DeltaRequest while not in game");
                this->kickUser(clientId);
            }
        } else {
            log.warn("Spectator send a DeltaRequest, kicking user");
            this->kickUser(clientId);
        }
    }

    template<>
    void Lobby::onPayload(const messages::request::PauseRequest &pauseRequest, int id) {
        if (!clients.at(id).isAi && (state == LobbyState::GAME || state == LobbyState::PAUSE)) {
            messages::broadcast::PauseResponse pauseResponse{
                    pauseRequest.getMessage(), this->clients.at(id).userName, true};
            this->sendAll(pauseResponse);
            log.info("Pause");
            game->pause();
            state = LobbyState::PAUSE;
        } else {
            sendError(messages::request::PauseRequest::getName(),
                      "Invalid pause request: either AI or Game not started", id);
            log.warn("Invalid pause request");
            this->kickUser(id);
        }
    }

    template<>
    void Lobby::onPayload(const messages::request::ContinueRequest &continueRequest, int id) {
        if (state == LobbyState::PAUSE) {
            messages::broadcast::PauseResponse pauseResponse{
                    continueRequest.getMessage(), this->clients.at(id).userName, false};
            this->sendAll(pauseResponse);
            log.info("Continue");
            game->resume();
            state = LobbyState::GAME;
        } else {
            sendError(messages::request::ContinueRequest::getName(),
                      "Not paused!", id);
            log.warn("Client sent a continue while not paused");
            this->kickUser(id);
        }
    }

    template <>
    void Lobby::onPayload(const messages::mods::request::SendChat &sendChat, int id) {
        lastTenMessages.emplace_back(clients.at(id).userName, sendChat.getInformation());
        if (lastTenMessages.size() > 10) {
            lastTenMessages.pop_front();
        }
        this->sendAll(messages::mods::broadcast::GlobalChat{clients.at(id).userName, sendChat.getInformation()});
    }

    template<typename T>
    void Lobby::onPayload(const T &, int client) {
        sendError(T::getName(),
                  "The message is not a request!", client);
        log.warn("Received message is not a request, kicking user");
        this->kickUser(client);
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
        onLeaveAfterTimeout(id);
    }

    void Lobby::onTeamFormationTimeout() {
        log.warn("Team formation timeout");
        if (!teamFormations.first.has_value() && !teamFormations.second.has_value()) {
            auto formerPlayers = players;
            players.first.reset();
            players.second.reset();
            sendAll(messages::broadcast::MatchFinish{0, 0, 0, "",messages::types::VictoryReason::VIOLATION_OF_PROTOCOL});
            sendError("teamFormation", "Timeout", formerPlayers.first.value());
            sendError("teamFormation", "Timeout", formerPlayers.second.value());
            kickUser(formerPlayers.first.value());
            kickUser(formerPlayers.second.value());
        } else if (!teamFormations.first.has_value()) {
            sendError("teamFormation", "Timeout", players.first.value());
            kickUser(players.first.value());
        } else if (!teamFormations.second.has_value()) {
            sendError("teamFormation", "Timeout", players.second.value());
            kickUser(players.second.value());
        }
    }

    void Lobby::onTimeout(communication::messages::types::EntityId entityId,
            communication::messages::types::PhaseType phaseType) {
        communication::messages::request::DeltaRequest deltaRequest{
            messages::types::DeltaType::SKIP,
            std::nullopt,
            std::nullopt,std::nullopt,std::nullopt,std::nullopt,
            entityId,
            std::nullopt,
            phaseType,
            std::nullopt,
            std::nullopt,
            std::nullopt,
            std::nullopt
        };
        game->executeDelta(deltaRequest, gameLogic::conversions::idToSide(entityId));
        if (game->fatalErrorEvent.has_value()) {
            onFatalError(game->fatalErrorEvent.value());
            return;
        } else if (game->winEvent.has_value()) {
            onWin(std::get<0>(game->winEvent.value()), std::get<1>(game->winEvent.value()));
            return;
        }

        modifySnapshotsAddToLogAndSend(game->getSnapshot());
        if (game->fatalErrorEvent.has_value()) {
            onFatalError(game->fatalErrorEvent.value());
            return;
        }
        auto next = game->getNextAction();
        modifySnapshotsAddToLogAndSend(game->getSnapshot());
        lastNext = next;
        this->sendAll(next);
        if (next.getEntityId() == messages::types::EntityId::SNITCH
            || next.getEntityId() == messages::types::EntityId::BLUDGER1
            || next.getEntityId() == messages::types::EntityId::BLUDGER2
            || next.getEntityId() == messages::types::EntityId::QUAFFLE) {
            game->executeBallDelta(next.getEntityId());
            modifySnapshotsAddToLogAndSend(game->getSnapshot());
            if (game->fatalErrorEvent.has_value()) {
                onFatalError(game->fatalErrorEvent.value());
                return;
            } else if (game->winEvent.has_value()) {
                onWin(std::get<0>(game->winEvent.value()), std::get<1>(game->winEvent.value()));
                return;
            }
        }
        replay.second.addLog(communication::messages::Message{next});
    }

    void Lobby::onWin(gameModel::TeamSide teamSide, communication::messages::types::VictoryReason victoryReason) {
        int winnerId;
        if (teamSide == gameModel::TeamSide::LEFT) {
            winnerId = players.first.value();
        } else {
            winnerId = players.second.value();
        }
        std::string winner;
        if (clients.find(winnerId) != clients.end()) {
            winner = clients.at(winnerId).userName;
        }

        messages::broadcast::MatchFinish matchFinish;
        if (game.has_value()) {
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
            ofstream << j.dump(4);
        } else {
            log.warn("Can not write replay to file!");
        }

        std::stringstream sstreamSnapshot;
        sstreamSnapshot << this->name << "_replaySnapshot.json";
        auto fnameSnapshot = sstreamSnapshot.str();
        std::ofstream ofstreamSnapshot{fnameSnapshot};
        if (ofstreamSnapshot.good()) {
            nlohmann::json jSnapshot = this->replay.second;
            ofstreamSnapshot << jSnapshot.dump(4);
        } else {
            log.warn("Can not write replay to file!");
        }
    }

    void Lobby::onLeave(int id) {
        auto leaveTimer = std::make_shared<util::Timer>();
        leaveTimer->setTimeout(std::bind(&Lobby::onLeaveAfterTimeout, this, id, leaveTimer), RECONNECT_TIMEOUT);
        this->leaveTimers.emplace(std::make_pair(id, leaveTimer));
        log.info("Client left, starting timeout");
    }

    void Lobby::onLeaveAfterTimeout(int id, std::optional<std::shared_ptr<util::Timer>> timer) {
        if (timer.has_value()) {
            timer.value()->stop();
            this->leaveTimers.erase(id);
        }

        if (id == players.first || id == players.second) {
            teamFormationTimer.stop();
            if (id == players.first) {
                if (players.second.has_value()) {
                    onWin(gameModel::TeamSide::RIGHT, messages::types::VictoryReason::VIOLATION_OF_PROTOCOL);
                    players.second.reset();
                }
            } else {
                if (players.first.has_value()) {
                    onWin(gameModel::TeamSide::LEFT, messages::types::VictoryReason::VIOLATION_OF_PROTOCOL);
                    players.first.reset();
                }
            }
        }
        auto userName = clients.find(id)->second.userName;
        communicator.removeClient(id, userName);
        clients.erase(clients.find(id));
        log.info("User left");
        communicator.removeFromLobbyAfterLeft(getUserInLobby() <= 0, userName, getName());
    }

    void Lobby::onFatalError(const std::string& error) {
        log.error("Fatal error!");
        for (const auto &c : clients) {
            sendWarn("Internal Server error, the game gets reseted", error, c.first);
        }
        log.error(error);
        state = LobbyState::INITIAL;
        game.reset();
        teamConfigs.first.reset();
        teamConfigs.second.reset();
        teamFormations.first.reset();
        teamFormations.second.reset();
        players.first.reset();
        players.second.reset();
    }

    void Lobby::sendAll(const messages::Payload &payload) {
        for (const auto &c : clients) {
            this->sendSingle(payload,c.first);
        }
    }

    void Lobby::sendSingle(const messages::Payload &payload, int id) {
        if (std::holds_alternative<messages::broadcast::Snapshot>(payload)) {
            auto &snapshot = std::get<messages::broadcast::Snapshot>(payload);
            int offset = 0;
            switch (snapshot.getPhase()) {
                case messages::types::PhaseType::BALL_PHASE:
                    offset = matchConfig.getBallPhaseTime();
                    break;
                case messages::types::PhaseType::FAN_PHASE:
                    offset = matchConfig.getFanPhaseTime();
                    break;
                case messages::types::PhaseType::PLAYER_PHASE:
                    offset = matchConfig.getPlayerPhaseTime();
                    break;
                case messages::types::PhaseType::UNBAN_PHASE:
                    offset = matchConfig.getUnbanPhaseTime();
                    break;
                default:break;
            }
            animationQueue.add(payload, {id}, std::chrono::milliseconds{offset});
        } else {
            animationQueue.add(payload, {id});
        }
    }

    void Lobby::sendSingle(const messages::ReplayPayload &payload, int id) {
        this->communicator.send(messages::ReplayMessage{payload}, id);
    }

    void Lobby::sendError(const std::string &payloadReason, const std::string &msg, int id) {
        auto cit = clients.find(id);
        if (cit != clients.end() && cit->second.mods.count(messages::types::Mods::ERROR) > 0) {
            this->sendSingle(communication::messages::mods::unicast::PrivateError{payloadReason, msg}, id);
        } else {
            std::stringstream sstream;
            sstream << "Error in " << payloadReason << ":" << msg;
            this->sendSingle(communication::messages::unicast::PrivateDebug{sstream.str()}, id);
        }
    }

    void Lobby::sendWarn(const std::string &payloadReason, const std::string &msg, int id) {
        auto cit = clients.find(id);
        if (cit != clients.end() && cit->second.mods.count(messages::types::Mods::ERROR) > 0) {
            this->sendSingle(communication::messages::mods::unicast::PrivateWarning{payloadReason, msg}, id);
        } else {
            std::stringstream sstream;
            sstream << "Warning in " << payloadReason << ":" << msg;
            this->sendSingle(communication::messages::unicast::PrivateDebug{sstream.str()}, id);
        }
    }

    void Lobby::modifySnapshotsAddToLogAndSend(std::queue<communication::messages::broadcast::Snapshot> snapshots) {
        while (!snapshots.empty()) {
            auto snapshot = snapshots.front();
            snapshots.pop();
            snapshot.setSpectators(getSpectators());
            this->sendAll(snapshot);
            replay.first.setFirstSnapshot(snapshot);
            replay.second.setFirstSnapshot(snapshot);
            lastSnapshot = snapshot;
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

    auto Lobby::getSpectators() const -> std::vector<std::string> {
        std::vector<std::string> ret;
        ret.reserve(clients.size() - 2);
        for (const auto &client : clients) {
            if (client.first != players.first && client.first != players.second) {
                ret.emplace_back(client.second.userName);
            }
        }
        return ret;
    }

    bool Client::operator==(const Client &rhs) const {
        return userName == rhs.userName &&
               password == rhs.password &&
               isAi == rhs.isAi &&
               mods == rhs.mods;
    }

    bool Client::operator!=(const Client &rhs) const {
        return !(rhs == *this);
    }
}
