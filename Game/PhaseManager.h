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
         * @return Next action to be taken, or nothing if player phase is over
         */
        auto nextPlayer() -> std::optional<communication::messages::broadcast::Next>;

        auto nextInterference() -> std::optional<communication::messages::broadcast::Next>;

        bool hasInterference() const;

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

        int interferencesUsedLeft(communication::messages::types::FanType type) const;
        int interferencesUsedRight(communication::messages::types::FanType type) const;

        TeamSide getPlayerSide() const;

        TeamSide getInterferencesSide() const;

        bool playerUsed(const std::shared_ptr<const gameModel::Player> &player) const;

    private:
        MemberSelector teamLeft, teamRight;
        const std::shared_ptr<const gameModel::Environment> env;
        TeamSide currentSidePlayers, currentSideInter;
        std::shared_ptr<gameModel::Player> currentPlayer;
        PlayerTurnState playerTurnState = PlayerTurnState::Move;
        TeamState teamStatePlayers = TeamState::BothAvailable;
        TeamState teamStateInterferences = TeamState::BothAvailable;

        void chooseSide(TeamSide &side) const;

        auto getTeam(TeamSide side) -> MemberSelector&;

        auto getTeam(TeamSide side) const -> const MemberSelector&;

        void switchSide(TeamSide &side);
    };
}


#endif //SERVER_PHASEMANAGER_H
