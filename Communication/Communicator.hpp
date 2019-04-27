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

        void send(const messages::Message &message);

    private:
        network::WebSocketServer webSocketServer;
        std::set<std::shared_ptr<network::Connection>> activeConnections;
    };
}


#endif //SERVER_COMMUNICATOR_HPP
