/**
 * @file MatchFinish.hpp
 * @author paul
 * @date 19.03.19
 * @brief MatchFinish @TODO
 */

#ifndef SERVER_MATCHFINISH_HPP
#define SERVER_MATCHFINISH_HPP

#include <string>
#include <Lib/json.hpp>

namespace communication::messages::broadcast {
    class MatchFinish {
    public:
        enum class VictoryReason {
            DISQUALIFICATION, BOTH_DISQUALIFICATION_MOST_POINTS,
            BOTH_DISQUALIFICATION_POINTS_EQUAL_SNITCH_CATCH,
            BOTH_DISQUALIFICATION_POINTS_EQUAL_LAST_DISQUALIFICATION,
            MOST_POINTS,
            POINTS_EQUAL_SNITCH_CATCH,
            VIOLATION_OF_PROTOCOL
        };

        MatchFinish(int endRound, int leftPoints, int rightPoints, std::string winnerUserName,
                    VictoryReason victoryReason);

        int getEndRound() const;

        int getLeftPoints() const;

        int getRightPoints() const;

        std::string getWinnerUserName() const;

        VictoryReason getVictoryReason() const;

        static auto getName() -> std::string;
    private:
        int endRound, leftPoints, rightPoints;
        std::string winnerUserName;
        VictoryReason victoryReason;
    };
}

#endif //SERVER_MATCHFINISH_HPP
