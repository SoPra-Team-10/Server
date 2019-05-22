//
// Created by timluchterhand on 06.05.19.
//

#include "setup.h"
#include <SopraMessages/types.hpp>

auto setup::createEnv(const gameModel::Config &config) -> std::shared_ptr<gameModel::Environment> {
    using ID = communication::messages::types::EntityId;
    gameModel::Chaser c1({2, 10}, "", {}, {}, ID::LEFT_CHASER1);
    gameModel::Chaser c2({8, 5}, "", {}, {}, ID::LEFT_CHASER2);
    gameModel::Chaser c3({10, 7}, "", {}, {}, ID::LEFT_CHASER3);
    gameModel::Chaser c4({6, 1}, "", {}, {}, ID::RIGHT_CHASER1);
    gameModel::Chaser c5({9, 9}, "", {}, {}, ID::RIGHT_CHASER2);
    gameModel::Chaser c6({7, 3}, "", {}, {}, ID::RIGHT_CHASER3);

    gameModel::Beater b1({1, 3}, "", {}, {}, ID::LEFT_BEATER1);
    gameModel::Beater b2({3, 0}, "", {}, {}, ID::LEFT_BEATER2);
    gameModel::Beater b3({0, 6}, "", {}, {}, ID::RIGHT_BEATER1);
    gameModel::Beater b4({4, 2}, "", {}, {}, ID::RIGHT_BEATER2);

    gameModel::Seeker s1({5, 4}, "", {}, {}, ID::LEFT_SEEKER);
    gameModel::Seeker s2({11, 8}, "", {}, {}, ID::RIGHT_SEEKER);

    gameModel::Keeper k1({12, 11}, "", {}, {}, ID::LEFT_KEEPER);
    gameModel::Keeper k2({13, 12}, "", {}, {}, ID::RIGHT_KEEPER);

    gameModel::Fanblock f(1, 2, 3, 1);

    auto t1 = std::make_shared<gameModel::Team>(s1, k1, std::array<gameModel::Beater, 2>{b1, b2}, std::array<gameModel::Chaser, 3>{c1, c2, c3}, "", "", "", f);
    auto t2 = std::make_shared<gameModel::Team>(s2, k2, std::array<gameModel::Beater, 2>{b3, b4}, std::array<gameModel::Chaser, 3>{c4, c5, c6}, "", "", "", f);

    return std::make_shared<gameModel::Environment>(config, t1, t2);
}



auto setup::createGame(util::Logging &log) -> gameHandling::Game {
    using namespace communication::messages;
    using namespace communication::messages::request;
    int maxRounds = 20;
    int playerTurnTimeout = 30;
    int fanTurnTimeout = 30;
    int unbanTurnTimeout = 30;
    int playerPhaseTime = 300;
    int fanPhaseTime = 300;
    int ballPhaseTime = 300;
    int unbanPhaseTime = 300;
    int teamFormationTimeout = 10;

    float throwSuccess = 0.8;
    float knockOut = 0.8;
    float catchSnitch = 0.4;
    float catchQuaffle = 0.4;
    float wrestQuaffle = 0.6;
    float extraTurn = 1;
    float foulDetection = 1;
    float fanFoulDetection = 1;
    broadcast::MatchConfig matchConfig(maxRounds, teamFormationTimeout, playerTurnTimeout, fanTurnTimeout, unbanTurnTimeout,
            playerPhaseTime, fanPhaseTime, ballPhaseTime, unbanPhaseTime, throwSuccess, knockOut,
            catchSnitch, catchQuaffle, wrestQuaffle, extraTurn, extraTurn, extraTurn, extraTurn,
            extraTurn, foulDetection, foulDetection, foulDetection, foulDetection, fanFoulDetection,
            fanFoulDetection, fanFoulDetection, fanFoulDetection, foulDetection,fanFoulDetection);

    auto t1 = createEnv({matchConfig})->team1;
    auto t2 = createEnv({matchConfig})->team2;
    TeamFormation formation1(t1->seeker->position.x, t1->seeker->position.y, t1->keeper->position.x,
            t1->keeper->position.y, t1->chasers[0]->position.x, t1->chasers[0]->position.y,
            t1->chasers[1]->position.x, t1->chasers[1]->position.y, t1->chasers[2]->position.x,
            t1->chasers[2]->position.y, t1->beaters[0]->position.x, t1->beaters[0]->position.y,
            t1->beaters[1]->position.x, t1->beaters[1]->position.y);

    TeamFormation formation2(t2->seeker->position.x, t2->seeker->position.y, t2->keeper->position.x,
                             t2->keeper->position.y, t2->chasers[0]->position.x, t2->chasers[0]->position.y,
                             t2->chasers[1]->position.x, t2->chasers[1]->position.y, t2->chasers[2]->position.x,
                             t2->chasers[2]->position.y, t2->beaters[0]->position.x, t2->beaters[0]->position.y,
                             t2->beaters[1]->position.x, t2->beaters[1]->position.y);

    TeamConfig tc("", "", "", "", "", 2, 2, 1, 1, 1, {}, {}, {}, {}, {}, {}, {});
    return gameHandling::Game(matchConfig, tc, tc, formation1, formation2, log);
}
