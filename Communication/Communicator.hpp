/**
 * @file Communicator.hpp
 * @author paul
 * @date 28.04.19
 * @brief Communicator @TODO
 */

#ifndef SERVER_COMMUNICATOR_HPP
#define SERVER_COMMUNICATOR_HPP

#include <cstdint>
#include <unordered_set>
#include "Game.hpp"
#include "MessageHandler.hpp"

namespace communication {
    class Communicator {
    public:
        explicit Communicator(uint16_t port);

    private:
        void receive(messages::Message message, int client);

        MessageHandler messageHandler;
        std::list<Game> activeGames;
    };
}

#endif //SERVER_COMMUNICATOR_HPP
