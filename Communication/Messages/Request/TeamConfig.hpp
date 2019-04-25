/**
 * @file TeamConfig.hpp
 * @author paul
 * @date 19.03.19
 * @brief TeamConfig @TODO
 */

#ifndef SERVER_TEAMCONFIG_HPP
#define SERVER_TEAMCONFIG_HPP

#include <string>
#include <Lib/json.hpp>

#include "Communication/Messages/Common/types.hpp"

namespace communication::messages::request {
    class Player {
    public:
        Player() = default;
        Player(std::string name, types::Broom broom, types::Sex sex);
        std::string getName() const;
        types::Broom getBroom() const;
        types::Sex getSex() const;

    private:
        std::string name;
        types::Broom broom;
        types::Sex sex;
    };

    class TeamConfig {
    public:
        TeamConfig() = default;
        TeamConfig(std::string name, std::string motto, std::string colorPrimary, std::string colorSecondary,
                   std::string image, int goblins, int trolls, int elfs, int nifflers, Player seeker,
                   Player keeper, Player chaser1, Player chaser2, Player chaser3,
                   Player beater1, Player beater2);

        std::string getMotto() const;
        std::string getColorPrimary() const;
        std::string getColorSecondary() const;
        std::string getImage() const;
        int getGoblins() const;
        int getTrolls() const;
        int getElfs() const;
        int getNifflers() const;
        Player getSeeker() const;
        Player getKeeper() const;
        Player getChaser1() const;
        Player getChaser2() const;
        Player getChaser3() const;
        Player getBeater1() const;
        Player getBeater2() const;

        static auto getName() -> std::string;

    private:
        std::string name, motto;
        std::string colorPrimary, colorSecondary;
        std::string image;
        int goblins, trolls, elfs, nifflers;
        Player seeker, keeper, chaser1, chaser2, chaser3, beater1, beater2;

    };

    void to_json(nlohmann::json &j, const TeamConfig &teamConfig);
    void from_json(const nlohmann::json &j, TeamConfig &teamConfig);

    void to_json(nlohmann::json &j, const Player &player);
    void from_json(const nlohmann::json &j, Player &player);
}

#endif //SERVER_TEAMCONFIG_HPP
