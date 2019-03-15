/**
 * @file Communicator.cpp
 * @author paul
 * @date 15.03.19
 * @brief Communicator @TODO
 */

#include "Communicator.hpp"

namespace communication {

    Communicator::Communicator(uint16_t port) :
        webSocketServer{port, "test"} {

    }

    template<typename T>
    void Communicator::sendAll(T &&t, std::string type) {
        nlohmann::json rootJson;
        rootJson["payloadType"] = type;
        rootJson["payload"] = t;
        rootJson["timestamp"] = "TODO"; //@TODO
        webSocketServer.broadcast(rootJson.dump(4));
    }
}