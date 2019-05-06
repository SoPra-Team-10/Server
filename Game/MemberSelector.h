//
// Created by timluchterhand on 06.05.19.
//

#ifndef SERVER_MEMBERSELECTOR_H
#define SERVER_MEMBERSELECTOR_H

#include <SopraMessages/types.hpp>
#include <SopraGameLogic/GameModel.h>
#include <SopraGameLogic/GameController.h>
#include <deque>
#include "GameTypes.h"

namespace gameHandling{
    class MemberSelector {
    public:
        MemberSelector(const std::shared_ptr<gameModel::Team> &team, TeamSide side);

        /**
         *
         * @return the next randomly chosen player id from the team
         * @throws std::runtime_error when all players from team have been selected
         */
        auto getNextPlayer() -> communication::messages::types::EntityId;

        /**
         *
         * @return true if all players have been selected, false otherwise
         */
        bool playersEmpty() const;

        /**
         *
         * @return the next randomly chosen interference id from the team
         * @throws std::runtime_error when all interferences from team have been selected
         */
        auto getNextInterference() -> communication::messages::types::EntityId;

        /**
         *
         * @return true if all interferences have been selected, false otherwise
         */
        bool interferencesEmpty() const;

        /**
         * Restores the initial state of the player list just as after the construction
         * Resets the internal representation of the player list, so that it is possible to randomly choose players again.
         */
        void resetPlayers();

        /**
         * Restores the initial state of the interference list just as after the construction
         * Resets the internal representation of the interference list, so that it is possible to randomly choose interferences again.
         */
        void resteInterferences();

    private:
        std::shared_ptr<const gameModel::Team> team;
        const gameModel::Fanblock fanblock;
        const TeamSide side;
        std::deque<communication::messages::types::EntityId> playersLeft;
        std::deque<std::pair<gameModel::InterferenceType, int>> interferencesLeft;

        auto interferenceToID(gameModel::InterferenceType type) const -> communication::messages::types::EntityId;

        template <typename T>
        auto selectRandom(std::deque<T> &list) const -> typename std::deque<T>::iterator;
    };


    template<typename T>
    auto MemberSelector::selectRandom(std::deque<T> &list) const -> typename std::deque<T>::iterator {
        int index = gameController::rng(0, static_cast<int>(list.size()) - 1);
        return list.begin() + index;
    }
}

#endif