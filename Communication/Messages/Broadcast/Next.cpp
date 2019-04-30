/**
 * @file Next.cpp
 * @author paul
 * @date 19.03.19
 * @brief Next @TODO
 */

#include "Next.hpp"

#include <utility>

namespace communication::messages::broadcast {

    auto Next::getName() -> std::string {
        return "next";
    }

    Next::Next(types::EntityId entityId, types::TurnType turnType, int timout) : entityId(entityId),
                                                                                        turnType(turnType),
                                                                                        timeout(timout) {}

    types::EntityId Next::getEntityId() const {
        return entityId;
    }

    types::TurnType Next::getTurnType() const {
        return turnType;
    }

    int Next::getTimout() const {
        return timeout;
    }

    bool Next::operator==(const Next &rhs) const {
        return entityId == rhs.entityId &&
               turnType == rhs.turnType &&
               timeout == rhs.timeout;
    }

    bool Next::operator!=(const Next &rhs) const {
        return !(rhs == *this);
    }

    void to_json(nlohmann::json &j, const Next &next) {
        j["turn"] = types::toString(next.getEntityId());
        j["type"] = types::toString(next.getTurnType());
        j["timeout"] = next.getTimout();
    }

    void from_json(const nlohmann::json &j, Next &next) {
        const auto &entityIdS = j.at("turn").get<std::string>();
        const auto &turnS = j.at("type").get<std::string>();

        next = Next{types::fromStringEntityId(entityIdS),
                    types::fromStringTurnType(turnS), j.at("timeout").get<int>()};
    }
}
