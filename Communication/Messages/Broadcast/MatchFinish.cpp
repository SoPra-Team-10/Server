/**
 * @file MatchFinish.cpp
 * @author paul
 * @date 19.03.19
 * @brief MatchFinish @TODO
 */

#include "MatchFinish.hpp"
#include <utility>

namespace communication::messages::broadcast {

    auto MatchFinish::getName() -> std::string {
        return "matchFinish";
    }

    MatchFinish::MatchFinish(int endRound, int leftPoints, int rightPoints, std::string winnerUserName,
                             types::VictoryReason victoryReason) : endRound(endRound), leftPoints(leftPoints),
                                                                         rightPoints(rightPoints),
                                                                         winnerUserName(std::move(winnerUserName)),
                                                                         victoryReason(victoryReason) {}

    int MatchFinish::getEndRound() const {
        return endRound;
    }

    int MatchFinish::getLeftPoints() const {
        return leftPoints;
    }

    int MatchFinish::getRightPoints() const {
        return rightPoints;
    }

    std::string MatchFinish::getWinnerUserName() const {
        return winnerUserName;
    }

    types::VictoryReason MatchFinish::getVictoryReason() const {
        return victoryReason;
    }

    void to_json(nlohmann::json &j, const MatchFinish &matchFinish) {
        j["endRound"] = matchFinish.getEndRound();
        j["leftPoints"] = matchFinish.getLeftPoints();
        j["rightPoints"] = matchFinish.getRightPoints();
        j["winnerUserName"] = matchFinish.getWinnerUserName();
        j["victoryReason"] =
                types::toString(matchFinish.getVictoryReason());
    }

    void from_json(const nlohmann::json &j, MatchFinish &matchFinish) {
        matchFinish = MatchFinish{
            j.at("endRound").get<int>(),
            j.at("leftPoints").get<int>(),
            j.at("rightPoints").get<int>(),
            j.at("winnerUserName").get<std::string>(),
            types::fromStringVictoryReason(
                    j.at("victoryReason").get<std::string>())
        };
    }
}
