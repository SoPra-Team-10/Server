/**
 * @file Communicator.cpp
 * @author paul
 * @date 28.04.19
 * @brief Definition of the communicator class
 */

#include "Communicator.hpp"

namespace communication {
    Communicator::Communicator(uint16_t port)
        : messageHandler{port} {

    }

    void Communicator::receive(messages::Message message, int client) {
        if (std::holds_alternative<messages::request::JoinRequest>(message.getPayload())) {
            auto joinRequest = std::get<messages::request::JoinRequest>(message.getPayload());
            Client newClient{joinRequest.getUserName(), joinRequest.getPassword(),
                             joinRequest.getIsAi(), joinRequest.getMods()};
            if (lobbyMapping.find(joinRequest.getLobby()) != lobbyMapping.end()) {
                lobbyMapping.at(joinRequest.getLobby())->addSpectator(newClient, client);
            } else {
                auto game = std::make_shared<Game>(*this, newClient, client);
                lobbyMapping.emplace(joinRequest.getLobby(), game);
                clientMapping.emplace(client, game);
            }
        } else {
            if (clientMapping.find(client) != clientMapping.end()) {
                clientMapping.at(client)->onMessage(message, client);
            }
            // If the user is not registered we simply ignore the message as there is no match to finish
        }
    }

    void Communicator::send(const messages::Message &message, int id) {
        this->messageHandler.send(message, id);
    }
}