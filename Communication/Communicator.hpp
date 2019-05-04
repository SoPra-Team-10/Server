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
        Communicator(MessageHandler &messageHandler, util::Logging &log, const messages::broadcast::MatchConfig &matchConfig);

        template <typename T>
        void send(const messages::AbstractMessage<T> &message, int id);

        void removeClient(int id);

    protected:
        void receive(messages::Message message, int client);
        void closeEvent(int id);

        MessageHandler &messageHandler;
        std::map<int, std::shared_ptr<Lobby>> clientMapping;
        std::map<std::string, std::shared_ptr<Lobby>> lobbyMapping;
        const messages::broadcast::MatchConfig &matchConfig;
        util::Logging &log;
    };

    template<typename T>
    void Communicator::send(const messages::AbstractMessage<T> &message, int id) {
        this->messageHandler.send(message, id);
    }
}

#endif //SERVER_COMMUNICATOR_HPP
