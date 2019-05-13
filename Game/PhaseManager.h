//
// Created by timluchterhand on 06.05.19.
//

#ifndef SERVER_PHASEMANAGER_H
#define SERVER_PHASEMANAGER_H

#include <SopraMessages/types.hpp>
#include <SopraGameLogic/GameModel.h>
#include <SopraGameLogic/GameController.h>
#include <SopraMessages/Next.hpp>
#include <queue>
#include "GameTypes.h"
#include "MemberSelector.h"

namespace gameHandling{

    enum class PlayerTurnState{
        Move,
        ExtraMove,
        PossibleAction
    };

    enum class TeamState{
        OneEmpty,
        BothEmpty,
        BothAvailable
    };

    class PhaseManager {
    public:
        PhaseManager(const std::shared_ptr<gameModel::Team> &teamLeft,
                     const std::shared_ptr<gameModel::Team> &teamRight,
                     std::shared_ptr<const gameModel::Environment> env);

        /**
         * Returns the next action required by a client
         * @param env
         * @throws std::runtime_error when player phase is over
         * @return
         */
        auto nextPlayer() -> communication::messages::broadcast::Next;

        auto nextInterference() -> communication::messages::broadcast::Next;

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

        bool playerUsed(communication::messages::types::EntityId id) const;

        int interferencesUsedLeft(communication::messages::types::FanType type) const;
        int interferencesUsedRight(communication::messages::types::FanType type) const;

    private:
        MemberSelector teamLeft, teamRight;
        const std::shared_ptr<const gameModel::Environment> env;
        TeamSide currentSidePlayers, currentSideInter;
        bool oneTeamEmptyPlayers = false;
        bool oneTeamEmptyInters = false;

        bool currentTurnFinished = true;
        std::shared_ptr<gameModel::Player> currentPlayer;
        PlayerTurnState playerTurnState = PlayerTurnState::Move;
        TeamState teamState = TeamState::BothAvailable;

        void chooseTeam(TeamSide &side) const;

        auto getNextPlayer() -> std::optional<std::shared_ptr<gameModel::Player>>;

        auto getTeam(TeamSide side) -> MemberSelector&;

        auto getTeam(TeamSide side) const -> const MemberSelector&;

        void switchSide(TeamSide &side);
    };
}


#endif //SERVER_PHASEMANAGER_H
