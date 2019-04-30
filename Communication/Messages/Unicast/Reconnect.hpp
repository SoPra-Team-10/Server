/**
 * @file Reconnect.hpp
 * @author paul
 * @date 19.03.19
 * @brief Reconnect @TODO
 */

#ifndef SERVER_RECONNECT_HPP
#define SERVER_RECONNECT_HPP

#include <string>
#include <Lib/json.hpp>
#include <Communication/Messages/Broadcast/MatchStart.hpp>
#include <Communication/Messages/Broadcast/Snapshot.hpp>

namespace communication::messages::unicast {
    class Reconnect {
    public:
        Reconnect() = default;
        Reconnect(broadcast::MatchStart matchStart, broadcast::Snapshot snapshot);

        broadcast::MatchStart getMatchStart() const;

        broadcast::Snapshot getSnapshot() const;

        static auto getName() -> std::string;

        bool operator==(const Reconnect &rhs) const;

        bool operator!=(const Reconnect &rhs) const;

    private:
        broadcast::MatchStart matchStart;
        broadcast::Snapshot snapshot;
    };

    void to_json(nlohmann::json &j, const Reconnect &reconnect);
    void from_json(const nlohmann::json &j, Reconnect &reconnect);
}

#endif //SERVER_RECONNECT_HPP
