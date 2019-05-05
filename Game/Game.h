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

enum class TeamSide : char {
    LEFT, RIGHT
};

class Game {
public:
    gameModel::Environment environment;
    Timer timer;
    Game(communication::messages::broadcast::MatchConfig matchConfig,
            const communication::messages::request::TeamConfig& teamConfig1,
            const communication::messages::request::TeamConfig& teamConfig2,
            communication::messages::request::TeamFormation teamFormation1,
            communication::messages::request::TeamFormation teamFormation2);
    const util::Listener<TeamSide> timeoutListener;
    const util::Listener<TeamSide, communication::messages::types::VictoryReason> winListener;
    void pause();
    void resume();
    auto getNextActor() -> communication::messages::broadcast::Next;
    bool executeDelta(communication::messages::request::DeltaRequest);
    auto getSnapshot() const -> communication::messages::broadcast::Snapshot;
    auto getEndRound() const -> int;
    auto getLeftPoints() const -> int;
    auto getRightPoints() const -> int;

private:
    int playerCounter = 0;
    int phaseCounter = 0;
    int ballCounter = 0;
    bool turnTeam1 = true;
    std::array<bool, 7> arrayTeam1 {true};
    std::array<bool, 7> arrayTeam2 {true};
    std::map<int ,communication::messages::types::EntityId> mapTeam1 {
            {0, communication::messages::types::EntityId::LEFT_KEEPER},
            {1, communication::messages::types::EntityId::LEFT_SEEKER},
            {2, communication::messages::types::EntityId::LEFT_BEATER1},
            {3, communication::messages::types::EntityId::LEFT_BEATER2},
            {4, communication::messages::types::EntityId::LEFT_CHASER1},
            {5, communication::messages::types::EntityId::LEFT_CHASER2},
            {6, communication::messages::types::EntityId::LEFT_CHASER3}
    };
    std::map<int, communication::messages::types::EntityId> mapTeam2 {
            {0, communication::messages::types::EntityId::RIGHT_KEEPER},
            {1, communication::messages::types::EntityId::RIGHT_SEEKER},
            {2, communication::messages::types::EntityId::RIGHT_BEATER1},
            {3, communication::messages::types::EntityId::RIGHT_BEATER2},
            {4, communication::messages::types::EntityId::RIGHT_CHASER1},
            {5, communication::messages::types::EntityId::RIGHT_CHASER2},
            {6, communication::messages::types::EntityId::RIGHT_CHASER3}
    };
    std::map<int, communication::messages::types::PhaseType> gamePhase {
            {0, communication::messages::types::PhaseType::PLAYER_PHASE},
            {1, communication::messages::types::PhaseType::BALL_PHASE},
            {2, communication::messages::types::PhaseType::FAN_PHASE},
            {3, communication::messages::types::PhaseType::GAME_FINISH}
    };
    std::map<int, communication::messages::types::EntityId> mapBallPhase {
            {0, communication::messages::types::EntityId::SNITCH},
            {1, communication::messages::types::EntityId::BLUDGER1},
            {2, communication::messages::types::EntityId::BLUDGER2}
    };

    auto getBallPhase(communication::messages::types::EntityId entityId) -> communication::messages::broadcast::Next;

};


#endif //SERVER_GAME_H
