/**
 * @file Communicator.cpp
 * @author paul
 * @date 15.03.19
 * @brief Communicator @TODO
 */

#include "Communicator.hpp"

namespace communication {

    Communicator::Communicator(uint16_t port) :
        webSocketServer{port, "test"} {}

    void Communicator::send(const messages::Message &message) {
        nlohmann::json json = message;
        webSocketServer.broadcast(json.dump(4));
    }
}