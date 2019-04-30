/**
 * @file MatchStart.cpp
 * @author paul
 * @date 19.03.19
 * @brief Declaration of the MatchStart class
 */

#include "MatchStart.hpp"
#include <utility>

namespace communication::messages::broadcast {

    auto MatchStart::getName() -> std::string {
        return "matchStart";
    }

    MatchStart::MatchStart(const MatchConfig &matchConfig, request::TeamConfig leftTeamConfig,
                           request::TeamConfig rightTeamConfig, std::string leftTeamUserName,
                           std::string rightTeamUserName) : matchConfig(matchConfig),
                                                                   leftTeamConfig(std::move(leftTeamConfig)),
                                                                   rightTeamConfig(std::move(rightTeamConfig)),
                                                                   leftTeamUserName(std::move(leftTeamUserName)),
                                                                   rightTeamUserName(std::move(rightTeamUserName)) {}

    MatchConfig MatchStart::getMatchConfig() const {
        return matchConfig;
    }

    request::TeamConfig MatchStart::getLeftTeamConfig() const {
        return leftTeamConfig;
    }

    request::TeamConfig MatchStart::getRightTeamConfig() const {
        return rightTeamConfig;
    }

    std::string MatchStart::getLeftTeamUserName() const {
        return leftTeamUserName;
    }

    std::string MatchStart::getRightTeamUserName() const {
        return rightTeamUserName;
    }

    bool MatchStart::operator==(const MatchStart &rhs) const {
        return matchConfig == rhs.matchConfig &&
               leftTeamConfig == rhs.leftTeamConfig &&
               rightTeamConfig == rhs.rightTeamConfig &&
               leftTeamUserName == rhs.leftTeamUserName &&
               rightTeamUserName == rhs.rightTeamUserName;
    }

    bool MatchStart::operator!=(const MatchStart &rhs) const {
        return !(rhs == *this);
    }

    void from_json(const nlohmann::json &j, MatchStart &matchStart) {
        matchStart = MatchStart{
            j.at("matchConfig").get<MatchConfig>(),
            j.at("leftTeamConfig").get<request::TeamConfig>(),
            j.at("rightTeamConfig").get<request::TeamConfig>(),
            j.at("leftTeamUserName").get<std::string>(),
            j.at("rightTeamUserName").get<std::string>()
        };
    }

    void to_json(nlohmann::json &j, const MatchStart &matchStart) {
        j["matchConfig"] = matchStart.getMatchConfig();
        j["leftTeamConfig"] = matchStart.getLeftTeamConfig();
        j["rightTeamConfig"] = matchStart.getRightTeamConfig();
        j["leftTeamUserName"] = matchStart.getLeftTeamUserName();
        j["rightTeamUserName"] = matchStart.getRightTeamUserName();
    }
}
