/**
 * @file GetReplay.cpp
 * @author paul
 * @date 19.03.19
 * @brief GetReplay @TODO
 */

#include "GetReplay.hpp"

namespace communication::messages::request {

    auto GetReplay::getName() -> std::string {
        return "getReplay";
    }

    void to_json(nlohmann::json &j, const GetReplay&) {
        j = nlohmann::json::object();
    }

    void from_json(const nlohmann::json&, GetReplay&) {

    }
}
