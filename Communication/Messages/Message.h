//
// Created by paulnykiel on 26.04.19.
//

#ifndef SERVER_MESSAGE_H
#define SERVER_MESSAGE_H


#include "Broadcast/DeltaBroadcast.hpp"

#include <variant>
#include <Communication/Messages/Broadcast/GlobalDebug.hpp>
#include <Communication/Messages/Broadcast/LoginGreeting.hpp>
#include <Communication/Messages/Broadcast/MatchConfig.hpp>
#include <Communication/Messages/Broadcast/MatchFinish.hpp>
#include <Communication/Messages/Broadcast/MatchStart.hpp>
#include <Communication/Messages/Broadcast/Next.hpp>
#include <Communication/Messages/Broadcast/PauseResponse.hpp>
#include <Communication/Messages/Broadcast/Snapshot.hpp>
#include <Communication/Messages/Request/ContinueRequest.hpp>
#include <Communication/Messages/Request/DeltaRequest.hpp>
#include <Communication/Messages/Request/GetReplay.hpp>
#include <Communication/Messages/Request/JoinRequest.hpp>
#include <Communication/Messages/Request/PauseRequest.hpp>
#include <Communication/Messages/Request/SendDebug.hpp>
#include <Communication/Messages/Request/TeamFormation.hpp>
#include <Communication/Messages/Unicast/JoinResponse.hpp>
#include <Communication/Messages/Unicast/PrivateDebug.hpp>
#include <Communication/Messages/Unicast/Reconnect.hpp>
#include <chrono>

namespace communication::messages {
    using Payload = std::variant<
            broadcast::DeltaBroadcast,
            broadcast::GlobalDebug,
            broadcast::LoginGreeting,
            broadcast::MatchConfig,
            broadcast::MatchFinish,
            broadcast::MatchStart,
            broadcast::Next,
            broadcast::PauseResponse,
            broadcast::Snapshot,
            request::ContinueRequest,
            request::DeltaRequest,
            request::GetReplay,
            request::JoinRequest,
            request::PauseRequest,
            request::SendDebug,
            request::TeamConfig,
            request::TeamFormation,
            unicast::JoinResponse,
            unicast::PrivateDebug,
            unicast::Reconnect>;

    class Message {
    public:
        Message();
        explicit Message(Payload payload);
        auto getPayload() const -> Payload;
        auto getPayloadType() const -> std::string;
        auto getTimeStamp() const -> std::string;

        bool operator==(const Message &rhs) const;

        bool operator!=(const Message &rhs) const;

    private:
        std::chrono::milliseconds timestamp;
        std::tm time{};
        Payload payload;
    };

    void to_json(nlohmann::json &j, const Message &message);
    void from_json(const nlohmann::json &j, Message &message);
}

#endif //SERVER_MESSAGE_H
