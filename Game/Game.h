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
#include <Util/Logging.hpp>
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
             communication::messages::request::TeamFormation teamFormation2,
             util::Logging &log);

        const util::Listener<communication::messages::types::EntityId, communication::messages::types::PhaseType> timeoutListener;
        const util::Listener<TeamSide, communication::messages::types::VictoryReason> winListener;
        const util::Listener<std::string> fatalErrorListener;

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

        /**
         * Executess the requested command if compliant with the game rules
         * @param command Command to be executed
         * @param teamSide Side which executes the command
         * @return true if successful, false if rule violation
         */
        bool executeDelta(communication::messages::request::DeltaRequest command, TeamSide teamSide);

        /**
         * Executes a ball turn
         * @param entityId the ball to make a move
         */
        void executeBallDelta(communication::messages::types::EntityId entityId);

        /**
         * Creates a bunch of redundant data for the clients
         * @return a queue of snapshots containing all DeltaBroadcasts since the last action
         */
        auto getSnapshot() -> std::queue<communication::messages::broadcast::Snapshot>;

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

        /**
         * Prepares the game state for the next round. Is called by the server after every call of executeDelta
         */
        void endRound();

    private:
        util::Timer timer;
        communication::messages::types::PhaseType currentPhase = communication::messages::types::PhaseType::BALL_PHASE; ///< the basic game phases
        communication::messages::types::EntityId ballTurn =
                communication::messages::types::EntityId::SNITCH; ///< the Ball to make a move
        unsigned int roundNumber = 0;
        PhaseManager phaseManager;
        std::queue<communication::messages::broadcast::DeltaBroadcast> lastDeltas {};
        communication::messages::broadcast::Next expectedRequestType{}; ///<Next-object containing information about the next expected request from a client
        TeamSide currentSide; ///<Current side to make a move
        util::Logging &log;
        gameController::ExcessLength overTimeState = gameController::ExcessLength::None;
        unsigned int overTimeCounter = 0;

        /**
         * Gets the team associated with the given side
         * @param side
         * @return
         */
        auto getTeam(TeamSide side) const -> std::shared_ptr<gameModel::Team>&;

        auto getVictoriousTeam(const std::shared_ptr<gameModel::Player> &winningPlayer) const -> std::pair<TeamSide, communication::messages::types::VictoryReason>;

        /**
         * Constructs a TeamSnapshot object from a Team
         * @param team
         * @param side
         * @return
         */
        auto teamToTeamSnapshot(const std::shared_ptr<const gameModel::Team> &team, TeamSide side) const
            -> communication::messages::broadcast::TeamSnapshot;

        /**
         * pushes a PHASE_CHANGE DeltaBroadcast on the lastDeltas queue if the game phase has changed
         */
        void changePhaseDelta();

        /**
         * Calls the timeoutListener
         */
        void onTimeout();
    };
}


#endif //SERVER_GAME_H
