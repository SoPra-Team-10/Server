//
// Created by bjorn on 02.05.19.
//

#ifndef SERVER_GAME_H
#define SERVER_GAME_H

#include <SopraNetwork/Listener.hpp>
#include <SopraMessages/MatchConfig.hpp>
#include <SopraMessages/Next.hpp>
#include <SopraMessages/TeamConfig.hpp>
#include <SopraMessages/TeamFormation.hpp>
#include <SopraMessages/DeltaRequest.hpp>
#include <SopraMessages/Snapshot.hpp>
#include <SopraGameLogic/GameModel.h>
#include <chrono>
#include "GameTypes.h"
#include "Util/Timer.h"
#include "PhaseManager.h"

namespace gameHandling{
    class Game {
    public:
        std::shared_ptr<gameModel::Environment> environment;
        Game(communication::messages::broadcast::MatchConfig matchConfig,
             const communication::messages::request::TeamConfig& teamConfig1,
             const communication::messages::request::TeamConfig& teamConfig2,
             communication::messages::request::TeamFormation teamFormation1,
             communication::messages::request::TeamFormation teamFormation2);

        const util::Listener<communication::messages::types::EntityId, communication::messages::types::PhaseType> timeoutListener;
        const util::Listener<TeamSide, communication::messages::types::VictoryReason> winListener;
        const util::Listener<const char*> fatalErrorListener;

        /**
         * Pauses the games timers
         */
        void pause();

        /**
         * Reactivates all timers
         */
        void resume();

        /**
         * Gets the next actor to make a move. If the actor is a player, the timeout timer is started
         * @return
         */
        auto getNextAction() -> communication::messages::broadcast::Next;

        bool executeDelta(communication::messages::request::DeltaRequest command, TeamSide teamSide);

        void executeBallDelta(communication::messages::types::EntityId entityId);

        auto getSnapshot() const -> communication::messages::broadcast::Snapshot;

        /**
         * Returns the current round
         * @return
         */
        auto getRound() const -> int;

        /**
         * Returns the left team's score
         * @return
         */
        auto getLeftPoints() const -> int;

        /**
         * Returns the right team's score
         * @return
         */
        auto getRightPoints() const -> int;

    private:
        util::Timer timer;
        communication::messages::types::PhaseType roundState = communication::messages::types::PhaseType::BALL_PHASE; ///< the basic game phases
        communication::messages::types::EntityId ballTurn =
                communication::messages::types::EntityId::SNITCH; ///< the Ball to make a move
        int roundNumber = 0;
        PhaseManager phaseManager;
        communication::messages::broadcast::DeltaBroadcast lastDelta;

        void endRound();

        auto getTeam(TeamSide side) const -> std::shared_ptr<gameModel::Team>&;

        auto teamToTeamSnapshot(const std::shared_ptr<const gameModel::Team> &team, TeamSide side) const
            -> communication::messages::broadcast::TeamSnapshot;
    };
}


#endif //SERVER_GAME_H
