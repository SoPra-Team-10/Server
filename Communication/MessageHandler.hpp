/**
 * @file Communicator.hpp
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
#include <SopraMessages/json.hpp>

#include <Util/Logging.hpp>


namespace communication {
    class MessageHandler {
    public:
        MessageHandler(uint16_t port, util::Logging &log);

        void sendAll(const messages::Message &message);
        void send(const messages::Message &message, int client);
        const util::Listener<messages::Message,int> onReceive;
        const util::Listener<int> onClose;
    private:
        void connectionListener(std::shared_ptr<network::Connection> connection);
        void receiveListener(int client, std::string string);
        void closeListener(std::shared_ptr<network::Connection> connection);
        int connectionCount;

        network::WebSocketServer webSocketServer;
        std::map<int, std::shared_ptr<network::Connection>> activeConnections;
        util::Logging &log;
    };
}


#endif //SERVER_MESSAGEHANDLER_HPP
