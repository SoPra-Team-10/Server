/**
 * @file Communicator.cpp
 * @author paul
 * @date 15.03.19
 * @brief Communicator @TODO
 */

#include "Communicator.hpp"

namespace communication {

    Communicator::Communicator(uint16_t port) :
        connectionCount{0},
        webSocketServer{port, "test"} {
        webSocketServer.connectionListener(
                std::bind(&Communicator::connectionListener, this, std::placeholders::_1));
        webSocketServer.closeListener(
                std::bind(&Communicator::closeListener, this, std::placeholders::_1));
    }

    void Communicator::sendAll(const messages::Message &message) {
        nlohmann::json json = message;
        webSocketServer.broadcast(json.dump(4));
    }

    void Communicator::send(const messages::Message &message, int client) {
        nlohmann::json json = message;
        activeConnections.at(client)->send(json.dump(4));
    }

    void Communicator::connectionListener(std::shared_ptr<network::Connection> connection) {
        activeConnections.emplace(this->connectionCount, connection);
        connection->receiveListener(std::bind(&Communicator::receiveListener,
                this, this->connectionCount, std::placeholders::_1));
    }

    void Communicator::receiveListener(int client, std::string string) {
        nlohmann::json json = string;
        auto message = json.get<messages::Message>();
        onReceive(message, client);
    }

    void Communicator::closeListener(std::shared_ptr<network::Connection> connection) {
        // Reverse lookup in O(n)
        for (auto it = this->activeConnections.begin(); it != this->activeConnections.end(); ++it) {
            if (it->second.get() == connection.get()) {
                this->activeConnections.erase(it);
                return;
            }
        }
    }
}