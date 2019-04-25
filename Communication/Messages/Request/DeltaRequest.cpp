/**
 * @file DeltaRequest.cpp
 * @author paul
 * @date 19.03.19
 * @brief DeltaRequest @TODO
 */

#include "DeltaRequest.hpp"
#include <Util/OptionalSerialization.hpp>

namespace communication::messages::request {
    auto DeltaRequest::getName() -> std::string {
        return "deltaRequest";
    }

    void to_json(nlohmann::json &j, const DeltaRequest &deltaRequest) {
        const Delta& delta = deltaRequest;
        to_json(j, delta);
    }

    void from_json(const nlohmann::json &j, DeltaRequest &deltaRequest) {
        Delta& delta = deltaRequest;
        from_json(j, delta);
    }
}
