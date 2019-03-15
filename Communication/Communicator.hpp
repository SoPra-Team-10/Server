/**
 * @file Communicator.hpp
 * @author paul
 * @date 15.03.19
 * @brief Communicator @TODO
 */

#ifndef SERVER_COMMUNICATOR_HPP
#define SERVER_COMMUNICATOR_HPP

#include <set>

#include <SopraNetwork/WebSocketServer.hpp>

#include "../Lib/json.hpp"

namespace communication {
    class Communicator {
    public:
        explicit Communicator(uint16_t port);


    private:
        template <typename T>
        void sendAll(T&& t, std::string type);

        network::WebSocketServer webSocketServer;
        std::set<std::shared_ptr<network::Connection>> activeConnections;
    };
}


#endif //SERVER_COMMUNICATOR_HPP
