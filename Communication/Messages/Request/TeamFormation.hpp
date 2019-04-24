/**
 * @file TeamFormation.hpp
 * @author paul
 * @date 19.03.19
 * @brief TeamFormation @TODO
 */

#ifndef SERVER_TEAMFORMATION_HPP
#define SERVER_TEAMFORMATION_HPP

#include <string>
#include <Lib/json.hpp>

namespace communication::messages::request {
    class TeamFormation {
    public:
        TeamFormation(int seekerX, int seekerY, int keeperX, int keeperY, int chaser1X, int chaser1Y, int chaser2X,
                      int chaser2Y, int chaser3X, int chaser3Y, int beater1X, int beater1Y, int beater2X, int beater2Y);

        int getSeekerX() const;

        int getSeekerY() const;

        int getKeeperX() const;

        int getKeeperY() const;

        int getChaser1X() const;

        int getChaser1Y() const;

        int getChaser2X() const;

        int getChaser2Y() const;

        int getChaser3X() const;

        int getChaser3Y() const;

        int getBeater1X() const;

        int getBeater1Y() const;

        int getBeater2X() const;

        int getBeater2Y() const;

        static auto getName() -> std::string;
    private:
        int seekerX, seekerY;
        int keeperX, keeperY;
        int chaser1X, chaser1Y;
        int chaser2X, chaser2Y;
        int chaser3X, chaser3Y;
        int beater1X, beater1Y;
        int beater2X, beater2Y;
    };

    void to_json(nlohmann::json &j, const TeamFormation &teamFormation);
    void from_json(const nlohmann::json &j, TeamFormation &teamFormation);
}

#endif //SERVER_TEAMFORMATION_HPP
