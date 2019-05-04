//
// Created by bjorn on 02.05.19.
//

#include "Game.h"
#include "iostream"
#include <SopraGameLogic/GameController.h>


Game::Game(communication::messages::broadcast::MatchConfig matchConfig,
           const communication::messages::request::TeamConfig& teamConfig1,
           const communication::messages::request::TeamConfig& teamConfig2,
           communication::messages::request::TeamFormation teamFormation1,
           communication::messages::request::TeamFormation teamFormation2) : environment(matchConfig,teamConfig1, teamConfig2, teamFormation1, teamFormation2){
    std::cout<<"Constructor is called"<<std::endl;
}

void Game::pause() {
    timer.stop();
    std::cout<<"pause() is called"<<std::endl;
}

void Game::resume() {
    std::cout<<"resume() is called"<<std::endl;
}

communication::messages::broadcast::Next Game::getNextActor() {
    int random = 0;
    switch(phaseCounter){
        case 0:
            if()
        case 1:
            if(playerCounter == 13){
                arrayTeam1 = {true};
                arrayTeam2 = {true};
                playerCounter = 0;
            }else if(turnTeam1){
                do{
                    random = gameController::rng(0,6);
                }while (!arrayTeam1[random]);
                random = gameController::rng(0,6);
                arrayTeam1[random] = false;
                turnTeam1 = false;
                playerCounter++;
                return communication::messages::broadcast::Next(mapTeam1[random], communication::messages::types::TurnType::MOVE, environment.config.timeouts.playerPhase);
            }else {
                do{
                    random = gameController::rng(0,6);
                }while (!arrayTeam2[random]);
                random = gameController::rng(0,6);
                arrayTeam2[random] = false;
                turnTeam1 = true;
                playerCounter++;
                return communication::messages::broadcast::Next(mapTeam2[random], communication::messages::types::TurnType::MOVE, environment.config.timeouts.playerPhase);
            }break;
        case 2:;
}

bool Game::executeDelta(communication::messages::request::DeltaRequest) {
    std::cout<<"executeDelta is called"<<std::endl;
    return false;
}

auto Game::getSnapshot() const -> communication::messages::broadcast::Snapshot {
    return communication::messages::broadcast::Snapshot();
}

auto Game::getEndRound() const -> int {
    return 0;
}

auto Game::getLeftPoints() const -> int {
    return 0;
}

auto Game::getRightPoints() const -> int {
    return 0;
}
