/**
 * @file Communicator.cpp
 * @author paul
 * @date 28.04.19
 * @brief Communicator @TODO
 */

#include "Communicator.hpp"

namespace communication {
    Communicator::Communicator(uint16_t port)
        : messageHandler{port} {

    }

    void Communicator::receive(messages::Message message, int client) {
        if (!std::holds_alternative<messages::request::JoinRequest>(message.getPayload()) &&
            !std::holds_alternative<messages::request::SendDebug>(message.getPayload()) &&
            !std::holds_alternative<messages::request::TeamConfig>(message.getPayload()) &&
            !std::holds_alternative<messages::request::TeamFormation>(message.getPayload()) &&
            !std::holds_alternative<messages::request::PauseRequest>(message.getPayload()) &&
            !std::holds_alternative<messages::request::ContinueRequest>(message.getPayload()) &&
            !std::holds_alternative<messages::request::DeltaRequest>(message.getPayload()) &&
            !std::holds_alternative<messages::request::GetReplay>(message.getPayload())) {
            //@TODO invalid message
            return;
        }

        if (std::holds_alternative<messages::request::JoinRequest>(message.getPayload())) {
            auto joinRequest = std::get<messages::request::JoinRequest>(message.getPayload());
            Client newClient{joinRequest.getUserName(), joinRequest.getPassword(),
                             joinRequest.getIsAi(), joinRequest.getMods(), client};
            if (lobbyMapping.find(joinRequest.getLobby()) != lobbyMapping.end()) {
                lobbyMapping.at(joinRequest.getLobby())->addSpectator(newClient);
            } else {
                auto game = std::make_shared<Game>(newClient);
                lobbyMapping.emplace(joinRequest.getLobby(), game);
                clientMapping.emplace(client, game);
            }
        } else {
            if (clientMapping.find(client) != clientMapping.end()) {
                clientMapping.at(client)->onMessage(message, client);
            } else {
                // Got a message from an unregistered user
                // @TODO kick them
            }
        }
    }
}