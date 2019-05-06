//
// Created by timluchterhand on 06.05.19.
//

#ifndef SERVER_TEAMMANAGER_H
#define SERVER_TEAMMANAGER_H

#include <SopraMessages/types.hpp>
#include <SopraGameLogic/GameModel.h>
#include <SopraGameLogic/GameController.h>
#include <SopraMessages/Next.hpp>
#include "GameTypes.h"
#include "MemberSelector.h"

namespace gameHandling{
    class TeamManager {
    public:
        TeamManager(const std::shared_ptr<gameModel::Team> &teamLeft, const std::shared_ptr<gameModel::Team> &teamRight);

        /**
         * Returns the next action required by a client
         * @param env
         * @throws std::runtime_error when player phase is over
         * @return
         */
        auto nextPlayer(const std::shared_ptr<const gameModel::Environment> &env) -> communication::messages::broadcast::Next;

        auto nextInterference(const std::shared_ptr<const gameModel::Environment> &env) -> communication::messages::broadcast::Next;

        /**
         *
         * @return true if at least one player of any Team is available and not knocked out
         */
        bool hasNextPlayer() const;

        bool hasNextInterference() const;

        /**
         * Resets the internal states so that players can be selected again
         */
        void resetPlayers();

        /**
         * Resets the internal states so that interferences can be selected again
         */
        void resetInterferences();

        /**
         * Resets all internal states so that players and interferences can be selected again
         */
        void reset();

    private:
        MemberSelector teamLeft, teamRight;
        TeamSide currentSidePlayers, currentSideInter;
        bool oneTeamEmptyPlayers = false;
        bool oneTeamEmptyInters = false;

        /**
         * Determines the next action a player may take
         * @param player player to calculate the action for
         * @param env environment to operate on
         * @return
         */
        auto getNextPlayerAction(const std::shared_ptr<const gameModel::Player> &player, const std::shared_ptr<const gameModel::Environment> &env) const ->
        communication::messages::types::TurnType;
        void chooseTeam(TeamSide &side);

        auto getNextPlayer() -> std::optional<std::shared_ptr<gameModel::Player>>;

        auto getTeam(TeamSide side) -> MemberSelector&;
    };
}


#endif //SERVER_TEAMMANAGER_H
