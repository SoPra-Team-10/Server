/**
 * @file Replay.hpp
 * @author paul
 * @date 19.03.19
 * @brief Replay @TODO
 */

#ifndef SERVER_REPLAY_HPP
#define SERVER_REPLAY_HPP

#include <string>
#include <Lib/json.hpp>
#include <Communication/Messages/Request/TeamConfig.hpp>
#include <Communication/Messages/Message.h>
#include "MatchConfig.hpp"
#include "Snapshot.hpp"

namespace communication::messages::broadcast {
    class Replay {
    public:
        Replay() = default;
        Replay(std::string lobby, const MatchConfig &matchConfig, request::TeamConfig leftTeamConfig,
               request::TeamConfig rightTeamConfig, std::string leftTeamUserName,
               std::string rightTeamUserName, std::vector<std::string> spectatrUserNames,
               Snapshot firstSnapshot, std::vector<Message> log);

        std::string getLobby() const;

        MatchConfig getMatchConfig() const;

        request::TeamConfig getLeftTeamConfig() const;

        request::TeamConfig getRightTeamConfig() const;

        std::string getLeftTeamUserName() const;

        std::string getRightTeamUserName() const;

        std::vector<std::string> getSpectatrUserNames() const;

        Snapshot getFirstSnapshot() const;

        std::vector<Message> getLog() const;

        static auto getName() -> std::string;

        bool operator==(const Replay &rhs) const;

        bool operator!=(const Replay &rhs) const;

    private:
        std::string lobby;
        MatchConfig matchConfig;
        request::TeamConfig leftTeamConfig, rightTeamConfig;
        std::string leftTeamUserName, rightTeamUserName;
        std::vector<std::string> spectatrUserNames;
        Snapshot firstSnapshot;
        std::vector<Message> log;
    };

    void to_json(nlohmann::json &j, const Replay &replay);
    void from_json(const nlohmann::json &j, Replay &replay);
}

#endif //SERVER_REPLAY_HPP
