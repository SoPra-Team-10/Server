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
#include "Timer.h"

namespace gameHandling{
    enum class TeamSide : char {
        LEFT, RIGHT
    };

    class Game {
    public:
        std::shared_ptr<gameModel::Environment> environment;
        Game(communication::messages::broadcast::MatchConfig matchConfig,
             const communication::messages::request::TeamConfig& teamConfig1,
             const communication::messages::request::TeamConfig& teamConfig2,
             communication::messages::request::TeamFormation teamFormation1,
             communication::messages::request::TeamFormation teamFormation2);

        const util::Listener<TeamSide> timeoutListener;
        const util::Listener<TeamSide, communication::messages::types::VictoryReason> winListener;

        /**
         * Pauses the games timers
         */
        void pause();

        /**
         * Reactivates all timers
         */
        void resume();

        auto getNextActor() -> communication::messages::broadcast::Next;

        bool executeDelta(communication::messages::request::DeltaRequest);

        auto getSnapshot() const -> communication::messages::broadcast::Snapshot;

        /**
         * Returns the current round
         * @return
         */
        auto getEndRound() const -> int;

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
        Timer timer;
    };
}


#endif //SERVER_GAME_H
