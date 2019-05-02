/**
 * @file Communicator.cpp
 * @author paul
 * @date 28.04.19
 * @brief Definition of the communicator class
 */

#include "Communicator.hpp"

namespace communication {
    Communicator::Communicator(uint16_t port, util::Logging &log, const messages::broadcast::MatchConfig &matchConfig)
        : messageHandler{port, log}, matchConfig{matchConfig}, log{log} {
        messageHandler.onReceive(
                std::bind(&Communicator::receive, this, std::placeholders::_1, std::placeholders::_2));
    }

    void Communicator::receive(messages::Message message, int client) {
        if (std::holds_alternative<messages::request::JoinRequest>(message.getPayload())) {
            if (clientMapping.find(client) == clientMapping.end()) {
                auto joinRequest = std::get<messages::request::JoinRequest>(message.getPayload());
                Client newClient{joinRequest.getUserName(), joinRequest.getPassword(),
                                 joinRequest.getIsAi(), joinRequest.getMods()};
                if (lobbyMapping.find(joinRequest.getLobby()) != lobbyMapping.end()) {
                    lobbyMapping.at(joinRequest.getLobby())->addSpectator(newClient, client);
                    log.info("New client joins existing lobby");
                } else {
                    auto game = std::make_shared<Lobby>(*this, newClient, client, log, matchConfig);
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

    void Communicator::send(const messages::Message &message, int id) {
        this->messageHandler.send(message, id);
    }
}