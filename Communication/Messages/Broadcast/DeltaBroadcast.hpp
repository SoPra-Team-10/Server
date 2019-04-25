/**
 * @file DeltaBroadcast.hpp
 * @author paul
 * @date 19.03.19
 * @brief DeltaBroadcast @TODO
 */

#ifndef SERVER_DELTABROADCAST_HPP
#define SERVER_DELTABROADCAST_HPP

#include <string>
#include <Lib/json.hpp>
#include <Communication/Messages/Common/Delta.hpp>

namespace communication::messages::broadcast {
    class DeltaBroadcast : public Delta {
    public:
        static auto getName() -> std::string;
    };

    void to_json(nlohmann::json &j, const DeltaBroadcast &deltaBroadcast);
    void from_json(const nlohmann::json &j, DeltaBroadcast &deltaBroadcast);
}

#endif //SERVER_DELTABROADCAST_HPP
