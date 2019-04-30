/**
 * @file MatchStart.hpp
 * @author paul
 * @date 19.03.19
 * @brief Declaration of the MatchStart class
 */

#ifndef SERVER_MATCHSTART_HPP
#define SERVER_MATCHSTART_HPP

#include <string>
#include <Lib/json.hpp>
#include <Communication/Messages/Request/TeamConfig.hpp>
#include "MatchConfig.hpp"

namespace communication::messages::broadcast {
    class MatchStart {
    public:
        MatchStart() = default;
        MatchStart(const MatchConfig &matchConfig, request::TeamConfig leftTeamConfig,
                   request::TeamConfig rightTeamConfig, std::string leftTeamUserName,
                   std::string rightTeamUserName);
        MatchConfig getMatchConfig() const;
        request::TeamConfig getLeftTeamConfig() const;
        request::TeamConfig getRightTeamConfig() const;
        std::string getLeftTeamUserName() const;
        std::string getRightTeamUserName() const;

        static auto getName() -> std::string;

        bool operator==(const MatchStart &rhs) const;

        bool operator!=(const MatchStart &rhs) const;

    private:
        MatchConfig matchConfig{};
        request::TeamConfig leftTeamConfig, rightTeamConfig;
        std::string leftTeamUserName, rightTeamUserName;
    };

    void to_json(nlohmann::json &j, const MatchStart &matchStart);
    void from_json(const nlohmann::json &j, MatchStart &matchStart);
}

#endif //SERVER_MATCHSTART_HPP
