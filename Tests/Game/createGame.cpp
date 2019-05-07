//
// Created by timluchterhand on 06.05.19.
//
#include <gtest/gtest.h>
#include "setup.h"

TEST(create_game_test, create){
    auto game = setup::createGame();
    SUCCEED();
}