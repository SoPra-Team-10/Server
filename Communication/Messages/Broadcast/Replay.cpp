/**
 * @file Replay.cpp
 * @author paul
 * @date 19.03.19
 * @brief Definition of the Replay class
 */

#include "Replay.hpp"

#include <utility>

namespace communication::messages::broadcast {

    auto Replay::getName() -> std::string {
        return "replay";
    }

    Replay::Replay(std::string lobby, std::string startTime, const MatchConfig &matchConfig, request::TeamConfig leftTeamConfig,
                   request::TeamConfig rightTeamConfig, std::string leftTeamUserName,
                   std::string rightTeamUserName, std::vector<std::string> spectatrUserNames,
                   Snapshot firstSnapshot, std::vector<Message> log) : lobby(std::move(lobby)),
                                                                    startTime{std::move(startTime)},
                                                                     matchConfig(matchConfig),
                                                                     leftTeamConfig(std::move(leftTeamConfig)),
                                                                     rightTeamConfig(std::move(rightTeamConfig)),
                                                                     leftTeamUserName(std::move(leftTeamUserName)),
                                                                     rightTeamUserName(std::move(
                                                                             rightTeamUserName)),
                                                                     spectatrUserNames(std::move(
                                                                             spectatrUserNames)),
                                                                     firstSnapshot(std::move(firstSnapshot)),
                                                                     log(std::move(log)) {}

    std::string Replay::getLobby() const {
        return lobby;
    }

    MatchConfig Replay::getMatchConfig() const {
        return matchConfig;
    }

    request::TeamConfig Replay::getLeftTeamConfig() const {
        return leftTeamConfig;
    }

    request::TeamConfig Replay::getRightTeamConfig() const {
        return rightTeamConfig;
    }

    std::string Replay::getLeftTeamUserName() const {
        return leftTeamUserName;
    }

    std::string Replay::getRightTeamUserName() const {
        return rightTeamUserName;
    }

    std::vector<std::string> Replay::getSpectatrUserNames() const {
        return spectatrUserNames;
    }

    Snapshot Replay::getFirstSnapshot() const {
        return firstSnapshot;
    }

    std::vector<Message> Replay::getLog() const {
        return log;
    }

    bool Replay::operator==(const Replay &rhs) const {
        return lobby == rhs.lobby &&
               matchConfig == rhs.matchConfig &&
               leftTeamConfig == rhs.leftTeamConfig &&
               rightTeamConfig == rhs.rightTeamConfig &&
               leftTeamUserName == rhs.leftTeamUserName &&
               rightTeamUserName == rhs.rightTeamUserName &&
               spectatrUserNames == rhs.spectatrUserNames &&
               firstSnapshot == rhs.firstSnapshot &&
               log == rhs.log;
    }

    bool Replay::operator!=(const Replay &rhs) const {
        return !(rhs == *this);
    }

    std::string Replay::getStartTime() const {
        return this->startTime;
    }

    void to_json(nlohmann::json &j, const Replay &replay) {
        j["lobby"] = replay.getLobby();
        j["startTimestamp"] = replay.getStartTime();
        j["matchConfig"] = replay.getMatchConfig();
        j["leftTeamConfig"] = replay.getLeftTeamConfig();
        j["rightTeamConfig"] = replay.getRightTeamConfig();
        j["leftTeamUserName"] = replay.getLeftTeamUserName();
        j["rightTeamUserName"] = replay.getRightTeamUserName();
        j["spectatorUserName"] = replay.getSpectatrUserNames();
        j["firstSnapshot"] = replay.getFirstSnapshot();
        j["log"] = replay.getLog();
    }

    void from_json(const nlohmann::json &j, Replay &replay) {
        replay = Replay{
            j.at("lobby").get<std::string>(),
            j.at("startTimestamp").get<std::string>(),
            j.at("matchConfig").get<MatchConfig>(),
            j.at("leftTeamConfig").get<request::TeamConfig>(),
            j.at("rightTeamConfig").get<request::TeamConfig>(),
            j.at("leftTeamUserName").get<std::string>(),
            j.at("rightTeamUserName").get<std::string>(),
            j.at("spectatorUserName").get<std::vector<std::string>>(),
            j.at("firstSnapshot").get<Snapshot>(),
            j.at("log").get<std::vector<Message>>()
        };
    }
}
