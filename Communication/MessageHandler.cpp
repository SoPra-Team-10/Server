/**
 * @file Communicator.cpp
 * @author paul
 * @date 15.03.19
 * @brief Definition of the MessageHandler class
 */

#include "MessageHandler.hpp"

namespace communication {

    MessageHandler::MessageHandler(uint16_t port, util::Logging &log, const std::string &protocolName) :
        connectionCount{0},
        log{log} {
        webSocketServer.emplace(port, protocolName);
        webSocketServer->connectionListener(
                std::bind(&MessageHandler::connectionListener, this, std::placeholders::_1));
        webSocketServer->closeListener(
                std::bind(&MessageHandler::closeListener, this, std::placeholders::_1));
    }

    void MessageHandler::send(const messages::Message &message, int client) {
        nlohmann::json json = message;
        activeConnections.at(client)->send(json.dump(4));
    }

    void MessageHandler::send(const messages::ReplayMessage &message, int client) {
        nlohmann::json json = message;
        activeConnections.at(client)->send(json.dump(4));
    }

    void MessageHandler::send(const messages::ReplayWithSnapshotMessage &message, int client) {
        nlohmann::json json = message;
        activeConnections.at(client)->send(json.dump(4));
    }

    void MessageHandler::connectionListener(std::shared_ptr<network::Connection> connection) {
        activeConnections.emplace(this->connectionCount, connection);
        connection->receiveListener(std::bind(&MessageHandler::receiveListener,
                this, this->connectionCount, std::placeholders::_1));
        log.info("New connection");
    }

    void MessageHandler::receiveListener(int client, std::string string) {
        // For some strange reason js sends an empty string on connection breakup...
        if (!string.empty()) {
            try {
                nlohmann::json json = nlohmann::json::parse(string);
                auto message = json.get<messages::Message>();
                onReceive(message, client);
            } catch (nlohmann::json::exception &e) {
                this->send(messages::Message{messages::unicast::PrivateDebug{
                        e.what()
                }}, client);
                log.error("Got invalid json!");
            } catch (std::runtime_error &e) {
                this->send(messages::Message{messages::unicast::PrivateDebug{
                        e.what()
                }}, client);
                log.error("Got invalid json values!");
            }
        }
    }

    void MessageHandler::closeListener(std::shared_ptr<network::Connection> connection) {
        // Reverse lookup in O(n)
        for (auto it = this->activeConnections.begin(); it != this->activeConnections.end(); ++it) {
            if (it->second.get() == connection.get()) {
                onClose(it->first);
                this->activeConnections.erase(it);
                return;
            }
        }
    }


}