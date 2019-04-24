/**
 * @file TeamFormation.cpp
 * @author paul
 * @date 19.03.19
 * @brief TeamFormation @TODO
 */

#include "TeamFormation.hpp"

namespace communication::messages::request {

    auto TeamFormation::getName() -> std::string {
        return "teamFormation";
    }

    TeamFormation::TeamFormation(int seekerX, int seekerY, int keeperX, int keeperY, int chaser1X, int chaser1Y,
                                 int chaser2X, int chaser2Y, int chaser3X, int chaser3Y, int beater1X, int beater1Y,
                                 int beater2X, int beater2Y) : seekerX(seekerX), seekerY(seekerY), keeperX(keeperX),
                                                               keeperY(keeperY), chaser1X(chaser1X), chaser1Y(chaser1Y),
                                                               chaser2X(chaser2X), chaser2Y(chaser2Y),
                                                               chaser3X(chaser3X), chaser3Y(chaser3Y),
                                                               beater1X(beater1X), beater1Y(beater1Y),
                                                               beater2X(beater2X), beater2Y(beater2Y) {}

    int TeamFormation::getSeekerX() const {
        return seekerX;
    }

    int TeamFormation::getSeekerY() const {
        return seekerY;
    }

    int TeamFormation::getKeeperX() const {
        return keeperX;
    }

    int TeamFormation::getKeeperY() const {
        return keeperY;
    }

    int TeamFormation::getChaser1X() const {
        return chaser1X;
    }

    int TeamFormation::getChaser1Y() const {
        return chaser1Y;
    }

    int TeamFormation::getChaser2X() const {
        return chaser2X;
    }

    int TeamFormation::getChaser2Y() const {
        return chaser2Y;
    }

    int TeamFormation::getChaser3X() const {
        return chaser3X;
    }

    int TeamFormation::getChaser3Y() const {
        return chaser3Y;
    }

    int TeamFormation::getBeater1X() const {
        return beater1X;
    }

    int TeamFormation::getBeater1Y() const {
        return beater1Y;
    }

    int TeamFormation::getBeater2X() const {
        return beater2X;
    }

    int TeamFormation::getBeater2Y() const {
        return beater2Y;
    }

    void to_json(nlohmann::json &j, const TeamFormation &teamFormation) {
        j["players"]["seeker"]["xPos"] = teamFormation.getSeekerX();
        j["players"]["seeker"]["yPos"] = teamFormation.getSeekerY();
        j["players"]["keeper"]["xPos"] = teamFormation.getKeeperX();
        j["players"]["keeper"]["yPos"] = teamFormation.getKeeperY();
        j["players"]["chaser1"]["xPos"] = teamFormation.getChaser1X();
        j["players"]["chaser1"]["yPos"] = teamFormation.getChaser1Y();
        j["players"]["chaser2"]["xPos"] = teamFormation.getChaser2X();
        j["players"]["chaser2"]["yPos"] = teamFormation.getChaser2Y();
        j["players"]["chaser3"]["xPos"] = teamFormation.getChaser3X();
        j["players"]["chaser3"]["yPos"] = teamFormation.getChaser3Y();
        j["players"]["beater1"]["xPos"] = teamFormation.getBeater1X();
        j["players"]["beater1"]["yPos"] = teamFormation.getBeater1Y();
        j["players"]["beater2"]["xPos"] = teamFormation.getBeater2X();
        j["players"]["beater2"]["yPos"] = teamFormation.getBeater2Y();
    }

    void from_json(const nlohmann::json &j, TeamFormation &teamFormation) {
        teamFormation = TeamFormation{
            j.at("players").at("seeker").at("xPos").get<int>(),
            j.at("players").at("seeker").at("yPos").get<int>(),
            j.at("players").at("keeper").at("xPos").get<int>(),
            j.at("players").at("keeper").at("yPos").get<int>(),
            j.at("players").at("chaser1").at("xPos").get<int>(),
            j.at("players").at("chaser1").at("yPos").get<int>(),
            j.at("players").at("chaser2").at("xPos").get<int>(),
            j.at("players").at("chaser2").at("yPos").get<int>(),
            j.at("players").at("chaser3").at("xPos").get<int>(),
            j.at("players").at("chaser3").at("yPos").get<int>(),
            j.at("players").at("beater1").at("xPos").get<int>(),
            j.at("players").at("beater1").at("yPos").get<int>(),
            j.at("players").at("beater2").at("xPos").get<int>(),
            j.at("players").at("beater2").at("yPos").get<int>()
        };
    }
}
