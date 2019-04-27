/**
 * @file Communicator.hpp
 * @author paul
 * @date 15.03.19
 * @brief Communicator @TODO
 */

#ifndef SERVER_COMMUNICATOR_HPP
#define SERVER_COMMUNICATOR_HPP

#include <set>
#include <sstream>
#include <iomanip>

#include <SopraNetwork/WebSocketServer.hpp>
#include <Communication/Messages/Message.h>

#include "../Lib/json.hpp"

namespace communication {
    class Communicator {
    public:
        explicit Communicator(uint16_t port);

        void sendAll(const messages::Message &message);
        void send(const messages::Message &message, int client);
        const util::Listener<messages::Message,int> onReceive;
    private:
        void connectionListener(std::shared_ptr<network::Connection> connection);
        void receiveListener(int client, std::string string);
        void closeListener(std::shared_ptr<network::Connection> connection);
        int connectionCount;

        network::WebSocketServer webSocketServer;
        std::map<int, std::shared_ptr<network::Connection>> activeConnections;
    };
}


#endif //SERVER_COMMUNICATOR_HPP
