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
        if (std::holds_alternative<messages::request::JoinRequest>(message.getPayload())) {
            auto joinRequest = std::get<messages::request::JoinRequest>(message.getPayload());
            Client newClient{joinRequest.getUserName(), joinRequest.getPassword(),
                             joinRequest.getIsAi(), joinRequest.getMods(), client};
            for (auto &game : this->activeGames) {
                if (game.getLobbyName() == joinRequest.getLobby()) {
                    game.addSecond(newClient);
                    return;
                }
            }

            this->activeGames.emplace_back(joinRequest.getLobby(), newClient);
        } else {
            for (auto &game : this->activeGames) {
                if (game.getLeftId() == client) {
                    game.onMessageLeft(message);
                    return;
                } else if (game.getRightId() == client) {
                    game.onMessageRight(message);
                    return;
                }
            }
            // Got a message from an unregistered user
        }
    }
}