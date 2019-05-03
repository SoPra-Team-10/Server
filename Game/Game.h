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

enum class TeamSide : char {
    LEFT, RIGHT
};

class Game {
public:
    Game(communication::messages::broadcast::MatchConfig matchConfig, communication::messages::request::TeamConfig teamConfig1, communication::messages::request::TeamConfig teamConfig2, communication::messages::request::TeamFormation teamFormation1, communication::messages::request::TeamFormation teamFormation2);
    const util::Listener<TeamSide> timeoutListener;
    const util::Listener<TeamSide, communication::messages::types::VictoryReason> winListener;
    void pause();
    void resume();
    communication::messages::broadcast::Next getNextActor();
    bool executeDelta(communication::messages::request::DeltaRequest);
    auto getSnapshot() const -> communication::messages::broadcast::Snapshot;
    auto getEndRound() const -> int;
    auto getLeftPoints() const -> int;
    auto getRightPoints() const -> int;
};


#endif //SERVER_GAME_H
