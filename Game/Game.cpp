//
// Created by bjorn on 02.05.19.
//

#include "Game.h"
#include "iostream"


Game::Game(communication::messages::broadcast::MatchConfig matchConfig,
           communication::messages::request::TeamConfig teamConfig1,
           communication::messages::request::TeamConfig teamConfig2,
           communication::messages::request::TeamFormation teamFormation1,
           communication::messages::request::TeamFormation teamFormation2)
           //: env(config(matchConfig.getMaxRounds(),
           //{matchConfig.getPlayerTurnTimeout(),matchConfig.getFanTurnTimeout(), matchConfig.getPlayerPhaseTime(), matchConfig.getFanPhaseTime(), matchConfig.getBallPhaseTime()},
           //{matchConfig.getProbFoulStooging(),matchConfig}))
           {

    /*static std::map<communication::messages::types::Broom, gameModel::Broom > broomMap;
    broomMap[communication::messages::types::Broom::THINDERBLAST] = gameModel::Broom::Thinderblast;
    broomMap[communication::messages::types::Broom ::NIMBUS2001] = gameModel::Broom::Nimbus_2001;
    broomMap[communication::messages::types::Broom ::COMET260] = gameModel::Broom::Comet_260;
    broomMap[communication::messages::types::Broom ::CLEANSWEEP11] = gameModel::Broom::Cleansweep_11;
    broomMap[communication::messages::types::Broom ::FIREBOLT] = gameModel::Broom::Firebolt;
    this->env.team1.seeker.broom = broomMap[teamConfig1.getSeeker().getBroom()];
    this->env.team1.keeper.broom = broomMap[teamConfig1.getKeeper().getBroom()];
    this->env.team1.beaters[0].broom = broomMap[teamConfig1.getBeater1().getBroom()];
    this->env.team1.beaters[1].broom = broomMap[teamConfig1.getBeater2().getBroom()];
    this->env.team1.chasers[0].broom = broomMap[teamConfig1.getChaser1().getBroom()];
    this->env.team1.chasers[1].broom = broomMap[teamConfig1.getChaser2().getBroom()];
    this->env.team1.chasers[2].broom = broomMap[teamConfig1.getChaser3().getBroom()];
    this->env.team2.seeker.broom = broomMap[teamConfig2.getSeeker().getBroom()];
    this->env.team2.keeper.broom = broomMap[teamConfig2.getKeeper().getBroom()];
    this->env.team2.beaters[0].broom = broomMap[teamConfig2.getBeater1().getBroom()];
    this->env.team2.beaters[1].broom = broomMap[teamConfig2.getBeater2().getBroom()];
    this->env.team2.chasers[0].broom = broomMap[teamConfig2.getChaser1().getBroom()];
    this->env.team2.chasers[1].broom = broomMap[teamConfig2.getChaser2().getBroom()];
    this->env.team2.chasers[2].broom = broomMap[teamConfig2.getChaser3().getBroom()];*/

    std::cout<<"Constructor is called"<<std::endl;
}

void Game::pause() {
    std::cout<<"pause() is called"<<std::endl;
}

void Game::resume() {
    std::cout<<"resume() is called"<<std::endl;
}

communication::messages::broadcast::Next Game::getNextActor() {
    std::cout<<"getNextActor() is called"<<std::endl;
    return communication::messages::broadcast::Next();
}

bool Game::executeDelta() {
    std::cout<<"executeDelta is called"<<std::endl;
    return false;
}
