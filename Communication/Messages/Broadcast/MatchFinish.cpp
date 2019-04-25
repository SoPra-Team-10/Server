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
                             MatchFinish::VictoryReason victoryReason) : endRound(endRound), leftPoints(leftPoints),
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

    MatchFinish::VictoryReason MatchFinish::getVictoryReason() const {
        return victoryReason;
    }
}
