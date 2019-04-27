#include <utility>

//
// Created by paulnykiel on 26.04.19.
//

#include "Message.h"
#include <sstream>

namespace communication::messages {
    Message::Message() :
            timestamp{std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch())},
                    payload{broadcast::DeltaBroadcast{}}
    {}

    Message::Message(Payload payload) :
        timestamp{std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch())},
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

    auto Message::getTimeStamp() const -> std::chrono::milliseconds {
        return this->timestamp;
    }

    void to_json(nlohmann::json &j, const Message &message) {
        j["timestamp"] = message.getTimeStamp().count(); // @TODO
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
