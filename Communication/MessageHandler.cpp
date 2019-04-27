/**
 * @file Communicator.cpp
 * @author paul
 * @date 15.03.19
 * @brief Communicator @TODO
 */

#include "MessageHandler.hpp"

namespace communication {

    MessageHandler::MessageHandler(uint16_t port) :
        connectionCount{0},
        webSocketServer{port, "http-only"} {
        webSocketServer.connectionListener(
                std::bind(&MessageHandler::connectionListener, this, std::placeholders::_1));
        webSocketServer.closeListener(
                std::bind(&MessageHandler::closeListener, this, std::placeholders::_1));
    }

    void MessageHandler::sendAll(const messages::Message &message) {
        nlohmann::json json = message;
        webSocketServer.broadcast(json.dump(4));
    }

    void MessageHandler::send(const messages::Message &message, int client) {
        nlohmann::json json = message;
        activeConnections.at(client)->send(json.dump(4));
    }

    void MessageHandler::connectionListener(std::shared_ptr<network::Connection> connection) {
        activeConnections.emplace(this->connectionCount, connection);
        connection->receiveListener(std::bind(&MessageHandler::receiveListener,
                this, this->connectionCount, std::placeholders::_1));
    }

    void MessageHandler::receiveListener(int client, std::string string) {
        nlohmann::json json = nlohmann::json::parse(string);
        auto message = json.get<messages::Message>();
        onReceive(message, client);
    }

    void MessageHandler::closeListener(std::shared_ptr<network::Connection> connection) {
        // Reverse lookup in O(n)
        for (auto it = this->activeConnections.begin(); it != this->activeConnections.end(); ++it) {
            if (it->second.get() == connection.get()) {
                this->activeConnections.erase(it);
                return;
            }
        }
    }
}