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
#include <SopraUtil/Logging.hpp>
#include "GameTypes.h"
#include <SopraUtil/Timer.h>
#include "PhaseManager.h"

namespace gameHandling {
    constexpr auto SNITCH_SPAWN_ROUND = 10;
    constexpr auto OVERTIME_INTERVAL = 3;
    constexpr auto MAX_BAN_COUNT = 2;

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
        mutable std::optional<std::tuple<gameModel::TeamSide, communication::messages::types::VictoryReason>> winEvent;
        mutable std::optional<std::string> fatalErrorEvent;

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
        bool executeDelta(communication::messages::request::DeltaRequest command, gameModel::TeamSide teamSide);

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

    private:
        util::Timer timer;
        communication::messages::types::PhaseType currentPhase = communication::messages::types::PhaseType::BALL_PHASE; ///< the basic game phases
        communication::messages::types::EntityId ballTurn =
                communication::messages::types::EntityId::SNITCH; ///< the Ball to make a move
        unsigned int roundNumber = 1;
        Timeouts timeouts;
        PhaseManager phaseManager;
        std::queue<communication::messages::broadcast::DeltaBroadcast> lastDeltas {};
        communication::messages::broadcast::Next expectedRequestType{}; ///<Next-object containing information about the next expected request from a client
        gameModel::TeamSide currentSide; ///<Current side to make a move
        util::Logging &log;
        gameController::ExcessLength overTimeState = gameController::ExcessLength::None;
        unsigned int overTimeCounter = 0;
        bool goalScored = false;
        std::deque<std::shared_ptr<gameModel::Player>> bannedPlayers = {};
        std::optional<gameModel::TeamSide> firstSideDisqualified = std::nullopt;

        /**
         * Gets the side of the given Team
         * @param player
         * @return
         */
        auto getSide(const std::shared_ptr<const gameModel::Player> &player) const -> gameModel::TeamSide;

        /**
         * gets the winning Team and the reason for winning when the snitch has been caught.
         * @param winningPlayer the Player catching the snitch
         * @return the winning team according to the game rules and the reason they won
         */
        auto getVictoriousTeam(const std::shared_ptr<const gameModel::Player> &winningPlayer) const -> std::pair<gameModel::TeamSide, communication::messages::types::VictoryReason>;

        /**
         * Constructs a TeamSnapshot object from a Team
         * @param team
         * @return
         */
        auto teamToTeamSnapshot(const std::shared_ptr<const gameModel::Team> &team) const
            -> communication::messages::broadcast::TeamSnapshot;

        /**
         * pushes a PHASE_CHANGE DeltaBroadcast on the lastDeltas queue if the game phase has changed
         * and makes necessary changes to the environment for the next phase
         */
        void changePhase();

        /**
         * Calls the timeoutListener
         */
        void onTimeout();

        /**
         * Prepares the game state for the next round.
         */
        void endRound();
    };
}


#endif //SERVER_GAME_H
