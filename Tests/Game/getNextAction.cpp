//
// Created by timluchterhand on 06.05.19.
//
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "setup.h"

TEST(game_get_next_action, generic_name){
    using namespace communication::messages::broadcast;
    using namespace communication::messages::types;
    using TT = communication::messages::types::TurnType;
    using Id = communication::messages::types::EntityId;
    auto game = setup::createGame();

    game.environment->snitch->position = game.environment->team1->chasers[1]->position;
    game.environment->quaffle->position = game.environment->team2->chasers[2]->position; //RIGHT_CHASE3 able to shoot
    game.environment->bludgers[0]->position = game.environment->team1->beaters[1]->position; //LEFT_BEATER2 able to shoot
    game.environment->bludgers[1]->position = game.environment->team2->beaters[0]->position;
    game.environment->team2->beaters[0]->knockedOut = true; //RIGHT_BEATER1 not able to shoot because knocked out
    game.environment->team2->seeker->isFined = true;


    //Ball phase
    auto next = game.getNextAction();
    std::cout << "ID " << toString(next.getEntityId()) << " | Type " << toString(next.getTurnType()) << " | Timeout " << next.getTimout() << std::endl;
    EXPECT_EQ(next, Next(Id::BLUDGER1, TT::MOVE, 0));
    next = game.getNextAction();
    std::cout << "ID " << toString(next.getEntityId()) << " | Type " << toString(next.getTurnType()) << " | Timeout " << next.getTimout() << std::endl;
    EXPECT_EQ(next, Next(Id::BLUDGER2, TT::MOVE, 0));

    //PlayerPhase
    std::deque<Id> playerIds;
    for(const auto &player : game.environment->getAllPlayers()){
        playerIds.emplace_back(player->id);
    }

    for(int i = 0; i < 27; i++){
        auto nxt = game.getNextAction();
        std::cout << "ID " << toString(nxt.getEntityId()) << " | Type " << toString(nxt.getTurnType()) << " | Timeout " << nxt.getTimout() << std::endl;
        EXPECT_THAT(nxt.getTurnType(), testing::AnyOf(TurnType::MOVE, TurnType::ACTION, TurnType::REMOVE_BAN));
        for(auto it = playerIds.begin(); it < playerIds.end(); it++){
            if(*it == nxt.getEntityId()){
                playerIds.erase(it);
                break;
            }
        }
    }

    EXPECT_EQ(playerIds.size(), 1);
    EXPECT_EQ(playerIds[0], Id::RIGHT_BEATER1);
    EXPECT_FALSE(game.environment->getPlayerById(Id::RIGHT_BEATER1)->knockedOut);
}
