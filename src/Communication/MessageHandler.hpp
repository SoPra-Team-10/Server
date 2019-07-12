/**
 * @file MessageHandler.hpp
 * @author paul
 * @date 15.03.19
 * @brief Declaration of the MessageHandler class
 */

#ifndef SERVER_MESSAGEHANDLER_HPP
#define SERVER_MESSAGEHANDLER_HPP

#include <set>
#include <sstream>
#include <iomanip>

#include <SopraNetwork/WebSocketServer.hpp>
#include <SopraMessages/Message.hpp>
#include <SopraMessages/ReplayMessage.h>
#include <SopraMessages/json.hpp>

#include <SopraUtil/Logging.hpp>
#include <SopraMessages/LobbyMod.hpp>


namespace communication {
    /**
     * The MessageHandler is responsible for the abstraction between the raw data from
     * the Websocket and Message-Objects. Furthermore it provides an abstraction for
     * connections.
     */
    class MessageHandler {
    public:
        /**
         * CTor, create a new message handler.
         * @param port the port used for the websocket
         * @param log a reference to the logger
         * @param protocolName the name of the websocket protocol, "http-only" by default
         * @see Logger for more information on the Logger
         */
        MessageHandler(uint16_t port, util::Logging &log, const std::string &protocolName = "");

        /**
         * Send a message to the client specified by id
         * @param message the message to send
         * @param client the client to which to send the message
         */
        virtual void send(const messages::Message &message, int client);

        /**
         * Send a replayMessage to the client specified by id
         * @param message the replayMessage to send
         * @param client the client to which to send the message
         */
        virtual void send(const messages::ReplayMessage &message, int client);

        /**
         * Listener that gets called on every new connection
         */
        const util::Listener<int> onConnect;

        /**
         * Listener, that gets called on every new Message
         */
        const util::Listener<messages::Message,int> onReceive;

        /**
         * Listener, that gets called when a client closes the connection
         */
        const util::Listener<int> onClose;

        /**
         * DTor, marks as virtual
         */
        virtual ~MessageHandler() = default;
    private:
        void connectionListener(std::shared_ptr<network::Connection> connection);
        void receiveListener(int client, const std::string& string);
        void closeListener(const std::shared_ptr<network::Connection>& connection);
        int connectionCount;

        std::optional<network::WebSocketServer> webSocketServer;
        std::map<int, std::shared_ptr<network::Connection>> activeConnections;
        util::Logging log;
    };
}


#endif //SERVER_MESSAGEHANDLER_HPP
