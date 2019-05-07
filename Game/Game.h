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
#include "MemberSelector.h"

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
        auto getNextActor() -> communication::messages::broadcast::Next;

        bool executeDelta(communication::messages::request::DeltaRequest);

        auto executeBallDelta(communication::messages::types::EntityId entityId)
                -> communication::messages::request::DeltaRequest;

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
        GameState roundState = GameState::BallPhase; ///< the basic game phases
        communication::messages::types::EntityId ballTurn =
                communication::messages::types::EntityId::SNITCH; ///< the Ball to make a move
        int roundNumber = 0;
        TeamSide activeTeam = TeamSide::LEFT;
        MemberSelector leftSelector, rightSelector;

    };
}


#endif //SERVER_GAME_H
