#include <utility>

//
// Created by paulnykiel on 26.04.19.
//

#include "Message.h"
#include <sstream>
#include <iomanip>

namespace communication::messages {
    Message::Message() :
            timestamp{std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch())},
                    payload{broadcast::DeltaBroadcast{}}{
        auto globTime = std::time(nullptr);
        this->time = *std::localtime(&globTime);
    }

    Message::Message(Payload payload) :
        timestamp{std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch())},
        time{},
        payload{std::move(payload)} {}

    auto Message::getPayload() const -> Payload {
        return this->payload;
    }

    auto Message::getPayloadType() const -> std::string {
        std::string type;
        std::visit([&type](auto &&arg){
            type = arg.getName();
        }, this->payload);
        return type;
    }

    auto Message::getTimeStamp() const -> std::string {
        // Formats the timestamp according to the standard
        // (time has only second accuracy, that's why there is this
        // stream magic)
        std::stringstream sstream;
        sstream << std::put_time(&this->time,"%F %T:")
            << std::setfill('0') <<std::setw(3) << (this->timestamp.count() % 1000);

        return sstream.str();
    }

    void to_json(nlohmann::json &j, const Message &message) {
        j["timestamp"] = message.getTimeStamp();
        j["payloadType"] = message.getPayloadType();
        std::visit([&j](auto &&arg){
            j["payload"] = arg;
        }, message.getPayload());
    }

    void from_json(const nlohmann::json &j, Message &message) {
        message = Message{
            j["payload"]
        };
    }
}
