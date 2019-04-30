/**
 * @file DeltaRequest.hpp
 * @author paul
 * @date 19.03.19
 * @brief Definition of the DeltaRequest class
 */

#ifndef SERVER_DELTAREQUEST_HPP
#define SERVER_DELTAREQUEST_HPP

#include <string>
#include <Lib/json.hpp>
#include <Communication/Messages/Common/types.hpp>
#include <Communication/Messages/Common/Delta.hpp>

namespace communication::messages::request {
    class DeltaRequest : public Delta {
    public:
        using Delta::Delta;
        static auto getName() -> std::string;
    };

    void to_json(nlohmann::json &j, const DeltaRequest &deltaRequest);
    void from_json(const nlohmann::json &j, DeltaRequest &deltaRequest);
}

#endif //SERVER_DELTAREQUEST_HPP
