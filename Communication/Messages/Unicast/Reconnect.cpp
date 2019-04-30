#include <utility>

/**
 * @file Reconnect.cpp
 * @author paul
 * @date 19.03.19
 * @brief Reconnect @TODO
 */

#include "Reconnect.hpp"

namespace communication::messages::unicast {

    auto Reconnect::getName() -> std::string {
        return "reconnect";
    }

    Reconnect::Reconnect(broadcast::MatchStart matchStart, broadcast::Snapshot snapshot) : matchStart(std::move(
            matchStart)), snapshot(std::move(snapshot)) {}

    broadcast::MatchStart Reconnect::getMatchStart() const {
        return matchStart;
    }

    broadcast::Snapshot Reconnect::getSnapshot() const {
        return snapshot;
    }

    bool Reconnect::operator==(const Reconnect &rhs) const {
        return matchStart == rhs.matchStart &&
               snapshot == rhs.snapshot;
    }

    bool Reconnect::operator!=(const Reconnect &rhs) const {
        return !(rhs == *this);
    }

    void to_json(nlohmann::json &j, const Reconnect &reconnect) {
        j["matchStart"] = reconnect.getMatchStart();
        j["snapshot"] = reconnect.getSnapshot();
    }

    void from_json(const nlohmann::json &j, Reconnect &reconnect) {
        reconnect = Reconnect{
            j.at("matchStart").get<broadcast::MatchStart>(),
            j.at("snapshot").get<broadcast::Snapshot>()
        };
    }
}
