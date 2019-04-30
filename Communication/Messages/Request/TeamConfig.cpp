#include <utility>

/**
 * @file TeamConfig.cpp
 * @author paul
 * @date 19.03.19
 * @brief TeamConfig @TODO
 */

#include "TeamConfig.hpp"
#include <utility>
#include <sstream>

namespace communication::messages::request {

    auto TeamConfig::getName() -> std::string {
        return "teamConfig";
    }

    TeamConfig::TeamConfig(std::string name, std::string motto, std::string colorPrimary,
                           std::string colorSecondary, std::string image, int goblins, int trolls, int elfs,
                           int nifflers, Player seeker, Player keeper, Player chaser1,
                           Player chaser2, Player chaser3, Player beater1, Player beater2)
            : name(std::move(name)), motto(std::move(motto)), colorPrimary(std::move(colorPrimary)),
              colorSecondary(std::move(colorSecondary)), image(std::move(image)),
              goblins(goblins), trolls(trolls), elfs(elfs), nifflers(nifflers), seeker(std::move(seeker)),
              keeper(std::move(keeper)),
              chaser1(std::move(chaser1)), chaser2(std::move(chaser2)), chaser3(std::move(chaser3)),
              beater1(std::move(beater1)), beater2(std::move(beater2)) {}

    std::string TeamConfig::getTeamName() const {
        return this->name;
    }

    std::string TeamConfig::getMotto() const {
        return motto;
    }

    std::string TeamConfig::getColorPrimary() const {
        return colorPrimary;
    }

    std::string TeamConfig::getColorSecondary() const {
        return colorSecondary;
    }

    std::string TeamConfig::getImage() const {
        return image;
    }

    int TeamConfig::getGoblins() const {
        return goblins;
    }

    int TeamConfig::getTrolls() const {
        return trolls;
    }

    int TeamConfig::getElfs() const {
        return elfs;
    }

    int TeamConfig::getNifflers() const {
        return nifflers;
    }

    void to_json(nlohmann::json &j, const TeamConfig &teamConfig) {
        j["name"] = teamConfig.getTeamName();
        j["motto"] = teamConfig.getMotto();
        j["colors"]["primary"] = teamConfig.getColorPrimary();
        j["colors"]["secondary"] = teamConfig.getColorSecondary();
        j["image"] = teamConfig.getImage();
        j["fans"]["goblins"] = teamConfig.getGoblins();
        j["fans"]["trolls"] = teamConfig.getTrolls();
        j["fans"]["elfs"] = teamConfig.getElfs();
        j["fans"]["nifflers"] = teamConfig.getNifflers();
        j["players"]["seeker"] = teamConfig.getSeeker();
        j["players"]["keeper"] = teamConfig.getKeeper();
        j["players"]["chaser1"] = teamConfig.getChaser1();
        j["players"]["chaser2"] = teamConfig.getChaser2();
        j["players"]["chaser3"] = teamConfig.getChaser3();
        j["players"]["beater1"] = teamConfig.getBeater1();
        j["players"]["beater2"] = teamConfig.getBeater2();
    }

    void from_json(const nlohmann::json &j, TeamConfig &teamConfig) {
        teamConfig = TeamConfig{
            j.at("name").get<std::string>(),
            j.at("motto").get<std::string>(),
            j.at("colors").at("primary").get<std::string>(),
            j.at("colors").at("secondary").get<std::string>(),
            j.at("image").get<std::string>(),
            j.at("fans").at("goblins").get<int>(),
            j.at("fans").at("trolls").get<int>(),
            j.at("fans").at("elfs").get<int>(),
            j.at("fans").at("nifflers").get<int>(),
            j.at("players").at("seeker").get<Player>(),
            j.at("players").at("keeper").get<Player>(),
            j.at("players").at("chaser1").get<Player>(),
            j.at("players").at("chaser2").get<Player>(),
            j.at("players").at("chaser3").get<Player>(),
            j.at("players").at("beater1").get<Player>(),
            j.at("players").at("beater2").get<Player>()
        };
    }

    Player TeamConfig::getSeeker() const {
        return seeker;
    }

    Player TeamConfig::getKeeper() const {
        return keeper;
    }

    Player TeamConfig::getChaser1() const {
        return chaser1;
    }

    Player TeamConfig::getChaser2() const {
        return chaser2;
    }

    Player TeamConfig::getChaser3() const {
        return chaser3;
    }

    Player TeamConfig::getBeater1() const {
        return beater1;
    }

    Player TeamConfig::getBeater2() const {
        return beater2;
    }

    bool TeamConfig::operator==(const TeamConfig &rhs) const {
        return name == rhs.name &&
               motto == rhs.motto &&
               colorPrimary == rhs.colorPrimary &&
               colorSecondary == rhs.colorSecondary &&
               image == rhs.image &&
               goblins == rhs.goblins &&
               trolls == rhs.trolls &&
               elfs == rhs.elfs &&
               nifflers == rhs.nifflers &&
               seeker == rhs.seeker &&
               keeper == rhs.keeper &&
               chaser1 == rhs.chaser1 &&
               chaser2 == rhs.chaser2 &&
               chaser3 == rhs.chaser3 &&
               beater1 == rhs.beater1 &&
               beater2 == rhs.beater2;
    }

    bool TeamConfig::operator!=(const TeamConfig &rhs) const {
        return !(rhs == *this);
    }


    Player::Player(std::string name, types::Broom broom, types::Sex sex) : name(std::move(name)), broom(broom),
                                                                             sex(sex) {}

    std::string Player::getName() const {
        return name;
    }

    types::Broom Player::getBroom() const {
        return broom;
    }

    types::Sex Player::getSex() const {
        return sex;
    }

    bool Player::operator==(const Player &rhs) const {
        return name == rhs.name &&
               broom == rhs.broom &&
               sex == rhs.sex;
    }

    bool Player::operator!=(const Player &rhs) const {
        return !(rhs == *this);
    }

    void to_json(nlohmann::json &j, const Player &player) {
        j["name"] = player.getName();
        j["broom"] = types::toString(player.getBroom());
        j["sex"] = types::toString(player.getSex());
    }

    void from_json(const nlohmann::json &j, Player &player) {
        const auto &broomS = j.at("broom").get<std::string>();
        const auto &sexS = j.at("sex").get<std::string>();
        const auto &name = j.at("name").get<std::string>();

        types::Broom broom = types::fromStringBroom(broomS);
        types::Sex sex = types::fromStringSex(sexS);

        player = Player{name, broom, sex};
    }
}
