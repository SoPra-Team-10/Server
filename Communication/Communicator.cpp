/**
 * @file Communicator.cpp
 * @author paul
 * @date 28.04.19
 * @brief Definition of the communicator class
 */

#include "Communicator.hpp"

namespace communication {
    Communicator::Communicator(MessageHandler &messageHandler, util::Logging &log,
                               const messages::broadcast::MatchConfig &matchConfig)
                               : messageHandler{messageHandler}, matchConfig{matchConfig}, log{log} {
        messageHandler.onReceive(
                std::bind(&Communicator::receive, this, std::placeholders::_1, std::placeholders::_2));
        messageHandler.onClose(
                std::bind(&Communicator::closeEvent, this, std::placeholders::_1));
    }

    void Communicator::receive(messages::Message message, int client) {
        if (std::holds_alternative<messages::request::JoinRequest>(message.getPayload())) {
            if (clientMapping.find(client) == clientMapping.end()) {
                auto joinRequest = std::get<messages::request::JoinRequest>(message.getPayload());
                std::set<messages::types::Mods> mods{joinRequest.getMods().begin(), joinRequest.getMods().end()};
                Client newClient{joinRequest.getUserName(), joinRequest.getPassword(),
                                 joinRequest.getIsAi(), mods};
                if (lobbyMapping.find(joinRequest.getLobby()) != lobbyMapping.end()) {
                    lobbyMapping.at(joinRequest.getLobby())->addSpectator(newClient, client);
                    clientMapping.emplace(client, lobbyMapping.at(joinRequest.getLobby()));
                    log.info("New client joins existing lobby");
                } else {
                    auto game = std::make_shared<Lobby>(joinRequest.getLobby(), message.getTimeStamp(),
                            *this, newClient, client, log, matchConfig);
                    lobbyMapping.emplace(joinRequest.getLobby(), game);
                    clientMapping.emplace(client, game);
                    log.info("New lobby");
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
        if (clientMapping.find(id) != clientMapping.end()) {
            clientMapping.at(id)->onLeave(id);
            clientMapping.erase(clientMapping.find(id));
        }
    }

    void Communicator::removeClient(int id) {
        if (clientMapping.find(id) != clientMapping.end()) {
            clientMapping.erase(clientMapping.find(id));
        }
    }
}