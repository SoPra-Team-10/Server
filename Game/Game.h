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
    Timer timer;
    communication::messages::broadcast::DeltaBroadcast deltaBroadcast;
    communication::messages::types::TurnType turnType;
    int playerCounter = 0;
    int phaseCounter = 0;
    int ballCounter = 0;
    int fanCounter = 0;
    int round = 0;
    int pointsLeft = 0;
    int pointsRight = 0;
    bool turnTeam1PlayerPhase = true;
    bool turnTeam1FanPhase = true;
    std::array<bool, 7> arrayTeam1PlayerTurnUsed {true};
    std::array<bool, 7> arrayTeam2PlayerTurnUsed {true};
    std::array<bool, 7> arrayTeam1PlayerKnockout {true};
    std::array<bool, 7> arrayTeam2PlayerKnockout {true};
    std::array<bool, 7> arrayTeam1Fan {true};
    std::array<bool, 7> arrayTeam2Fan {true};
    std::optional<std::shared_ptr<gameModel::Player>> quaffleHold;
    std::optional<std::shared_ptr<gameModel::Player>> bludgerHold;
    std::map<int ,communication::messages::types::EntityId> mapTeam1Player {
            {0, communication::messages::types::EntityId::LEFT_KEEPER},
            {1, communication::messages::types::EntityId::LEFT_SEEKER},
            {2, communication::messages::types::EntityId::LEFT_BEATER1},
            {3, communication::messages::types::EntityId::LEFT_BEATER2},
            {4, communication::messages::types::EntityId::LEFT_CHASER1},
            {5, communication::messages::types::EntityId::LEFT_CHASER2},
            {6, communication::messages::types::EntityId::LEFT_CHASER3}
    };
    std::map<int, communication::messages::types::EntityId> mapTeam2Player {
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
    std::map<int, communication::messages::types::EntityId > mapTeam1Fan {
            {0, communication::messages::types::EntityId::LEFT_ELF },
            {1, communication::messages::types::EntityId::LEFT_GOBLIN },
            {2, communication::messages::types::EntityId::LEFT_NIFFLER },
            {3, communication::messages::types::EntityId::LEFT_TROLL }
    };
    std::map<int, communication::messages::types::EntityId > mapTeam2Fan {
            {0, communication::messages::types::EntityId::RIGHT_ELF },
            {1, communication::messages::types::EntityId::RIGHT_GOBLIN },
            {2, communication::messages::types::EntityId::RIGHT_NIFFLER },
            {3, communication::messages::types::EntityId::RIGHT_TROLL }
    };
    std::vector<std::pair<communication::messages::types::FanType , bool>> fansTeam1 {};
    std::vector<std::pair<communication::messages::types::FanType , bool>> fansTeam2 {};

    auto getBallPhase(communication::messages::types::EntityId entityId) -> communication::messages::broadcast::Next;

};


#endif //SERVER_GAME_H
