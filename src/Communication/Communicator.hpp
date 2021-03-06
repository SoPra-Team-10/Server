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
#include <SopraUtil/Logging.hpp>
#include "Lobby.hpp"
#include "MessageHandler.hpp"

namespace communication {
    /**
     * The communicator is responsible for managing player sessions and lobbys.
     */
    class Communicator {
    public:
        /**
         * CTor: Start the communicator with a MessageHandler as a backend.
         * @param messageHandler the message handler to use to send and receive messages
         * @param log the log object which to use
         * @param matchConfig the matchConfig to use for all lobbies instantiated from this communicator
         */
        Communicator(MessageHandler &messageHandler, util::Logging &log,
                const messages::broadcast::MatchConfig &matchConfig);

        /**
         * Send an abstract message to the communicator
         * @tparam T type of the message payload
         * @param message the message to send
         * @param id the client to which the message should be sent
         */
        template <typename T>
        void send(const messages::AbstractMessage<T> &message, int id);

        /**
         * Remove a client from the lobby, this is used by a lobby to kick a user.
         */
        void removeClient(int id, const std::string& name);

        /**
         * Removes a client after a timeout and closes the lobby if empty
         * @param lobbyEmpty true if the player that left was the last player
         * @param userName the name of the player
         * @param lobbyName the name of the lobby
         */
        void removeFromLobbyAfterLeft(bool lobbyEmpty, const std::string &userName, const std::string &lobbyName);
    protected:
        void receive(const messages::Message& message, int client);

    private:
        void onConnection(int id);
        void closeEvent(int id);
        void sendLobbyModMessage(int id);

        MessageHandler &messageHandler;
        std::map<int, std::shared_ptr<Lobby>> clientMapping;
        std::map<std::string, std::shared_ptr<Lobby>> lobbyMapping;
        std::map<std::string, std::shared_ptr<Lobby>> userNameMapping;
        const messages::broadcast::MatchConfig matchConfig;
        util::Logging log;
    };

    template<typename T>
    void Communicator::send(const messages::AbstractMessage<T> &message, int id) {
        this->messageHandler.send(message, id);
    }
}

#endif //SERVER_COMMUNICATOR_HPP
