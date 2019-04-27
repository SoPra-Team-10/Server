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

        template <typename T>
        void sendAll(T&& t);
    private:

        network::WebSocketServer webSocketServer;
        std::set<std::shared_ptr<network::Connection>> activeConnections;
    };

    template<typename T>
    void Communicator::sendAll(T &&t) {
        auto time = std::time(nullptr);
        auto localTime = std::localtime(&time);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>
                (std::chrono::system_clock::now().time_since_epoch());

        // Formats the timestamp according to the standard
        // (time has only second accuracy, that's why there is this
        // stream magic)
        std::stringstream sstream;
        sstream << std::put_time(localTime,"%F %T:")
            << std::setfill('0') <<std::setw(3) << (ms.count() % 1000);

        nlohmann::json rootJson;
        rootJson["payloadType"] = t.getName();
        rootJson["payload"] = t;
        rootJson["timestamp"] = sstream.str();
        webSocketServer.broadcast(rootJson.dump(4));
    }
}


#endif //SERVER_COMMUNICATOR_HPP
