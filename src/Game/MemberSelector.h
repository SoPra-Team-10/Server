//
// Created by timluchterhand on 06.05.19.
//

#ifndef SERVER_MEMBERSELECTOR_H
#define SERVER_MEMBERSELECTOR_H

#include <SopraMessages/types.hpp>
#include <SopraGameLogic/GameModel.h>
#include <SopraGameLogic/GameController.h>
#include <deque>
#include <SopraGameLogic/Interference.h>
#include "GameTypes.h"

namespace gameHandling{

    /**
     * Implements a pseudo queue for selecting players and fans from a team
     */
    class MemberSelector {
    public:
        explicit MemberSelector(const std::shared_ptr<gameModel::Team> &team);

        /**
         * Get the next player from the queue
         * @throws std::runtime_error when all players from team have been selected
         * @return the next randomly chosen player id from the team
         */
        auto getNextPlayer() -> std::shared_ptr<gameModel::Player>;

        /**
         * Checks if there are still Players left in the queue
         * @return true if at least one Player is available
         */
        bool hasPlayers() const;

        /**
         * Checks if the player has been used
         * @param id
         * @return true if the specified PlayerID has already been selected
         */
        bool playerUsed(communication::messages::types::EntityId id) const;

        /**
         * Get the next Interference from the queue
         * @throws std::runtime_error when all interferences from team have been selected
         * @return the next randomly chosen interference id from the team
         */
        auto getNextInterference() -> communication::messages::types::EntityId;

        /**
         * Checks if there are still Interferences left in the queue
         * @return false if all interferences have been selected, true otherwise
         */
        bool hasInterference() const;

        /**
         * Gets the number of times the selected fan has been used
         * @param type The type of Interference as FanType
         * @return the number of times the selected fan has been used
         */
        int usedInterferences(communication::messages::types::FanType type) const;

        /**
         * Restores the initial state of the player list just as after the construction
         * Resets the internal representation of the player list, so that it is possible to randomly choose players again.
         */
        void resetPlayers();

        /**
         * Restores the initial state of the interference list just as after the construction
         * Resets the internal representation of the interference list, so that it is possible to randomly choose interferences again.
         */
        void resetInterferences();

        /**
         * Getter
         * @return the Teamside of the managed Team
         */
        auto getSide() const -> gameModel::TeamSide;

    private:
        std::shared_ptr<const gameModel::Team> team; ///<The immutable Team used to reset the internal state of the MemberSelector
        std::deque<std::shared_ptr<gameModel::Player>> playersLeft; ///<List with all currently available Players
        std::deque<std::pair<gameModel::InterferenceType, int>> interferencesLeft; ///<List with all currently available Interferences and their respective uses

        /**
         * Selects a random entry from a deque
         * @tparam T Type of deque
         * @param list
         * @return iterator to a randomly choosen element of the deque
         */
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