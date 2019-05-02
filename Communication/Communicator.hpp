/**
 * @file Communicator.hpp
 * @author paul
 * @date 28.04.19
 * @brief Declaration of the Commuicator class
 */

#ifndef SERVER_COMMUNICATOR_HPP
#define SERVER_COMMUNICATOR_HPP

#include <cstdint>
#include <unordered_set>
#include <Util/Logging.hpp>
#include "Lobby.hpp"
#include "MessageHandler.hpp"

namespace communication {
    class Communicator {
    public:
        Communicator(uint16_t port, util::Logging &log, const messages::broadcast::MatchConfig &matchConfig);

        void send(const messages::Message &message, int id);
    private:
        void receive(messages::Message message, int client);

        MessageHandler messageHandler;
        std::map<int, std::shared_ptr<Lobby>> clientMapping;
        std::map<std::string, std::shared_ptr<Lobby>> lobbyMapping;
        const messages::broadcast::MatchConfig &matchConfig;
        util::Logging &log;
    };
}

#endif //SERVER_COMMUNICATOR_HPP
