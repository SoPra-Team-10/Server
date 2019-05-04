//
// Created by bjorn on 02.05.19.
//

#include "Game.h"
#include "iostream"


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

using entityID = communication::messages::types::EntityId ;
communication::messages::broadcast::Next Game::getNextActor() {
    std::map<entityID , int> listTeam1;
    std::map<entityID , int> listTeam2;
    std::cout<<"getNextActor() is called"<<std::endl;
    return communication::messages::broadcast::Next();
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
