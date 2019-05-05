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
    switch(phaseCounter) {
        case 0: {
            if (ballCounter == 2) {
                phaseCounter++;
                ballCounter = 0;
                break;
            }
            communication::messages::broadcast::Next next = getBallPhase(mapBallPhase[ballCounter]);
            ballCounter++;
            return next;
        }
        case 1:
            if(playerCounter %2 == 0){
                if(gameController::rng(0,1) == 0){
                    turnTeam1PlayerPhase = true;
                }else{
                    turnTeam1PlayerPhase = false;
                }
            }
            if (playerCounter == 13) {
                arrayTeam1Player = {true};
                arrayTeam2Player = {true};
                playerCounter = 0;
                phaseCounter++;
            } else if (turnTeam1PlayerPhase) {
                do {
                    random = gameController::rng(0, 6);
                } while (!arrayTeam1Player[random]);
                random = gameController::rng(0, 6);
                arrayTeam1Player[random] = false;
                turnTeam1PlayerPhase = false;
                playerCounter++;
                return communication::messages::broadcast::Next(mapTeam1Player[random],
                                                                communication::messages::types::TurnType::MOVE,
                                                                environment.config.timeouts.playerPhase);
            } else {
                do {
                    random = gameController::rng(0, 6);
                } while (!arrayTeam2Player[random]);
                random = gameController::rng(0, 6);
                arrayTeam2Player[random] = false;
                turnTeam1PlayerPhase = true;
                playerCounter++;
                return communication::messages::broadcast::Next(mapTeam2Player[random],
                                                                communication::messages::types::TurnType::MOVE,
                                                                environment.config.timeouts.playerPhase);
            }
            break;
        case 2:{
            if(fanCounter % 2 == 0){
                if(gameController::rng(0,1) == 0){
                    turnTeam1FanPhase = true;
                }else {
                    turnTeam1FanPhase = false;
                }
            }
            if(fanCounter == 13){
                arrayTeam1Fan = {true};
                arrayTeam2Fan = {true};
                fanCounter = 0;
                phaseCounter = 0;
            }else if (turnTeam1FanPhase) {
                do {
                    random = gameController::rng(0, 6);
                } while (!arrayTeam1Fan[random]);
                random = gameController::rng(0, 6);
                arrayTeam1Fan[random] = false;
                turnTeam1FanPhase = false;
                fanCounter++;
                return communication::messages::broadcast::Next(mapTeam1Fan[random],
                                                                communication::messages::types::TurnType::FAN,
                                                                environment.config.timeouts.fanPhase);
            } else {
                do {
                    random = gameController::rng(0, 6);
                } while (!arrayTeam2Fan[random]);
                random = gameController::rng(0, 6);
                arrayTeam2Fan[random] = false;
                turnTeam1FanPhase = true;
                fanCounter++;
                return communication::messages::broadcast::Next(mapTeam2Fan[random],
                                                                communication::messages::types::TurnType::FAN,
                                                                environment.config.timeouts.fanPhase);
            }
            break;
        }
    }
    return communication::messages::broadcast::Next();
}

bool Game::executeDelta(communication::messages::request::DeltaRequest) {
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

auto Game::getBallPhase(communication::messages::types::EntityId entityId) -> communication::messages::broadcast::Next {
    switch (entityId) {
        case communication::messages::types::EntityId::SNITCH : {
            std::vector<gameModel::Position> vector = environment.getSurroundingPositions(
                    environment.snitch.get()->position);
            environment.snitch.get()->position = gameModel::Position{
                    vector[gameController::rng(0, static_cast<int> (vector.max_size())-1)]};
            return communication::messages::broadcast::Next{environment.snitch.get()->id,
                                                            communication::messages::types::TurnType::MOVE,
                                                            environment.config.timeouts.ballPhase};
        }
        case communication::messages::types::EntityId::BLUDGER1 : {
            std::vector<gameModel::Position> vector = environment.getSurroundingPositions(
                    environment.snitch.get()->position);
            environment.snitch.get()->position = gameModel::Position{
                    vector[gameController::rng(0, static_cast<int> (vector.max_size())-1)]};
            return communication::messages::broadcast::Next{environment.snitch.get()->id,
                                                            communication::messages::types::TurnType::MOVE,
                                                            environment.config.timeouts.ballPhase};
        }
        case communication::messages::types::EntityId::BLUDGER2: {
            std::vector<gameModel::Position> vector = environment.getSurroundingPositions(
                    environment.snitch.get()->position);
            environment.snitch.get()->position = gameModel::Position{
                    vector[gameController::rng(0, static_cast<int> (vector.max_size())-1)]};
            return communication::messages::broadcast::Next{environment.snitch.get()->id,
                                                            communication::messages::types::TurnType::MOVE,
                                                            environment.config.timeouts.ballPhase};
        }
        default:
            return communication::messages::broadcast::Next {};
    }
}

