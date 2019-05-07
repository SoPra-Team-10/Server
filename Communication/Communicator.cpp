/**
 * @file Communicator.cpp
 * @author paul
 * @date 28.04.19
 * @brief Definition of the communicator class
 */

#include "Communicator.hpp"
#include <SopraMessages/LobbyMod.hpp>

namespace communication {
    Communicator::Communicator(MessageHandler &messageHandler, util::Logging &log,
                               const messages::broadcast::MatchConfig &matchConfig)
                               : messageHandler{messageHandler}, matchConfig{matchConfig}, log{log} {
        messageHandler.onConnect(
                std::bind(&Communicator::onConnection, this, std::placeholders::_1));
        messageHandler.onReceive(
                std::bind(&Communicator::receive, this, std::placeholders::_1, std::placeholders::_2));
        messageHandler.onClose(
                std::bind(&Communicator::closeEvent, this, std::placeholders::_1));
    }

    void Communicator::onConnection(int id) {
        sendLobbyModMessage(id);
    }

    void Communicator::receive(messages::Message message, int client) {
        if (std::holds_alternative<messages::request::JoinRequest>(message.getPayload())) {
            if (clientMapping.find(client) == clientMapping.end()) {
                auto joinRequest = std::get<messages::request::JoinRequest>(message.getPayload());
                if (userNameMapping.find(joinRequest.getUserName()) == userNameMapping.end()) {
                    std::set<messages::types::Mods> mods{joinRequest.getMods().begin(), joinRequest.getMods().end()};
                    Client newClient{joinRequest.getUserName(), joinRequest.getPassword(),
                                     joinRequest.getIsAi(), mods};
                    if (lobbyMapping.find(joinRequest.getLobby()) != lobbyMapping.end()) {
                        lobbyMapping.at(joinRequest.getLobby())->addSpectator(newClient, client);
                        clientMapping.emplace(client, lobbyMapping.at(joinRequest.getLobby()));
                        userNameMapping.emplace(joinRequest.getUserName());
                        log.info("New client joins existing lobby");
                    } else {
                        auto game = std::make_shared<Lobby>(joinRequest.getLobby(), message.getTimeStamp(),
                                                            *this, newClient, client, log, matchConfig);
                        lobbyMapping.emplace(joinRequest.getLobby(), game);
                        clientMapping.emplace(client, game);
                        userNameMapping.emplace(joinRequest.getUserName());
                        log.info("New lobby");
                    }
                } else {
                    // Username taken
                    this->send(messages::Message{
                            messages::unicast::PrivateDebug{"Username is taken!"}}, client);
                    log.warn("Username taken");
                }
            } else {
                // User is already in a lobby
                this->send(messages::Message{
                        messages::unicast::PrivateDebug{"You are already in a Lobby!"}}, client);
                log.warn("User tried to join a second lobby");
            }
        } else {
            if (clientMapping.find(client) != clientMapping.end()) {
                clientMapping.at(client)->onMessage(message, client);
            } else {
                // Not a known user
                this->send(messages::Message{
                        messages::unicast::PrivateDebug{"You need to send a JoinRequest first!"}}, client);
                log.warn("User sent a message without joining");
            }
        }
    }

    void Communicator::closeEvent(int id) {
        auto cit = clientMapping.find(id);
        if (cit != clientMapping.end()) {
            auto name = cit->second->getName();
            auto [lobbyEmpty, userName] = cit->second->onLeave(id);
            userNameMapping.erase(userName);
            if(lobbyEmpty) {
                auto lit = lobbyMapping.find(name);
                if (lit != lobbyMapping.end()) {
                    lobbyMapping.erase(lit);
                }
            }
        }
    }

    void Communicator::removeClient(int id) {
        auto cmIt = clientMapping.find(id);
        if (cmIt != clientMapping.end()) {
            clientMapping.erase(cmIt);
        }
    }

    void Communicator::sendLobbyModMessage(int id) {
        messages::mods::other::LobbyMod lobbyMod;
        for (const auto lobby : lobbyMapping) {
            lobbyMod.addLobby({lobby.first, lobby.second->isMatchStarted(), lobby.second->getUserInLobby()});
        }
        messageHandler.send(lobbyMod, id);
    }
}