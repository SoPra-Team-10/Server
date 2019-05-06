//
// Created by timluchterhand on 06.05.19.
//
#include <gtest/gtest.h>
#include "setup.h"

TEST(game_get_next_action, generic_name){
    using namespace communication::messages::broadcast;
    using namespace communication::messages::types;
    using TT = communication::messages::types::TurnType;
    using Id = communication::messages::types::EntityId;
    auto game = setup::createGame();

    //Ball phase
    auto next = game.getNextAction();
    std::cout << "ID " << toString(next.getEntityId()) << " | Type " << toString(next.getTurnType()) << " | Timeout " << next.getTimout();
    EXPECT_EQ(next, Next(Id::BLUDGER1, TT::MOVE, 0));
    next = game.getNextAction();
    std::cout << "ID " << toString(next.getEntityId()) << " | Type " << toString(next.getTurnType()) << " | Timeout " << next.getTimout();
    EXPECT_EQ(next, Next(Id::BLUDGER2, TT::MOVE, 0));

    //PlayerPhase
    std::deque<Id> playerIds;
    for(const auto &player : game.environment->getAllPlayers()){
        playerIds.emplace_back(player->id);
    }

    for(int i = 0; i < 14; i++){
        auto nxt = game.getNextAction();
        for(auto it = playerIds.begin(); it < playerIds.end(); it++){
            if(*it == nxt.getEntityId()){
                playerIds.erase(it);
                break;
            }
        }
    }

    EXPECT_EQ(playerIds.size(), 0);
}
