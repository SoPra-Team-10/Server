//
// Created by timluchterhand on 06.05.19.
//
#include <gtest/gtest.h>
#include "setup.h"

TEST(create_game_test, create){
    std::stringstream ostream;
    util::Logging log{ostream, 5};
    auto game = setup::createGame(log);
    SUCCEED();
}