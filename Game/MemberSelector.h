//
// Created by timluchterhand on 06.05.19.
//

#ifndef SERVER_MEMBERSELECTOR_H
#define SERVER_MEMBERSELECTOR_H

#include <SopraMessages/types.hpp>
#include <SopraGameLogic/GameModel.h>
#include <deque>
#include "Game.h"

namespace gameHandling{
    class MemberSelector {
        MemberSelector(const std::shared_ptr<gameModel::Team> &team, TeamSide side);

        /**
         *
         * @return the next randomly chosen player id from the team
         * @throws std::runtime_error when all players from team have been selected
         */
        auto getNextPlayer() -> communication::messages::types::EntityId;

        /**
         *
         * @return the next randomly chosen interference id from the team
         * @throws std::runtime_error when all interferences from team have been selected
         */
        auto getNextInterference() -> communication::messages::types::EntityId;

    private:
        TeamSide side;
        std::deque<communication::messages::types::EntityId> playersLeft;
        std::deque<gameModel::InterferenceType > interferencesLeft;
        auto interferenceToID(gameModel::InterferenceType type) const -> communication::messages::types::EntityId;
    };
}

#endif