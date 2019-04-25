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

#include "Communication/Messages/Common/types.hpp"

namespace communication::messages::broadcast {
    class MatchFinish {
    public:

        MatchFinish(int endRound, int leftPoints, int rightPoints, std::string winnerUserName,
                    types::VictoryReason victoryReason);

        int getEndRound() const;

        int getLeftPoints() const;

        int getRightPoints() const;

        std::string getWinnerUserName() const;

        types::VictoryReason getVictoryReason() const;

        static auto getName() -> std::string;
    private:
        int endRound, leftPoints, rightPoints;
        std::string winnerUserName;
        types::VictoryReason victoryReason;
    };
}

#endif //SERVER_MATCHFINISH_HPP
