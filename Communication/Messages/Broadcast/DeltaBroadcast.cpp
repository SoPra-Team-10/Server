/**
 * @file DeltaBroadcast.cpp
 * @author paul
 * @date 19.03.19
 * @brief DeltaBroadcast @TODO
 */

#include "DeltaBroadcast.hpp"

namespace communication::messages::broadcast {
    auto DeltaBroadcast::getName() -> std::string {
        return "deltaBroadcast";
    }

    void to_json(nlohmann::json &j, const DeltaBroadcast &deltaBroadcast) {
        const Delta& delta = deltaBroadcast;
        to_json(j, delta);
    }

    void from_json(const nlohmann::json &j, DeltaBroadcast &deltaBroadcast) {
        Delta& delta = deltaBroadcast;
        from_json(j, delta);
    }
}
