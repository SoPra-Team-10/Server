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
    for(int i = 0; i < teamConfig1.getTrolls();i++ ){
        fansTeam1[i] = std::pair(communication::messages::types::FanType::TROLL, true);
    }
    for(int i = teamConfig1.getTrolls(); i < teamConfig1.getTrolls() + teamConfig1.getElfs() ;i++ ){
        fansTeam1[i] = std::pair(communication::messages::types::FanType::ELF, true);
    }
    for(int i = teamConfig1.getTrolls() + teamConfig1.getElfs(); i < teamConfig1.getTrolls() + teamConfig1.getElfs() + teamConfig1.getNifflers();i++ ){
        fansTeam1[i] = std::pair(communication::messages::types::FanType::NIFFLER, true);
    }
    for(int i = teamConfig1.getTrolls() + teamConfig1.getElfs() + teamConfig1.getNifflers(); i < teamConfig1.getTrolls() + teamConfig1.getElfs() + teamConfig1.getNifflers() + teamConfig1.getGoblins();i++ ){
        fansTeam1[i] = std::pair(communication::messages::types::FanType::GOBLIN, true);
    }
    for(int i = 0; i < teamConfig1.getTrolls();i++ ){
        fansTeam2[i] = std::pair(communication::messages::types::FanType::TROLL, true);
    }
    for(int i = teamConfig1.getTrolls(); i < teamConfig1.getTrolls() + teamConfig1.getElfs() ;i++ ){
        fansTeam2[i] = std::pair(communication::messages::types::FanType::ELF, true);
    }
    for(int i = teamConfig1.getTrolls() + teamConfig1.getElfs(); i < teamConfig1.getTrolls() + teamConfig1.getElfs() + teamConfig1.getNifflers();i++ ){
        fansTeam2[i] = std::pair(communication::messages::types::FanType::NIFFLER, true);
    }
    for(int i = teamConfig1.getTrolls() + teamConfig1.getElfs() + teamConfig1.getNifflers(); i < teamConfig1.getTrolls() + teamConfig1.getElfs() + teamConfig1.getNifflers() + teamConfig1.getGoblins();i++ ){
        fansTeam2[i] = std::pair(communication::messages::types::FanType::GOBLIN, true);
    }
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
            if (playerCounter == 13 - environment.team1.numberOfBannedMembers() - environment.team2.numberOfBannedMembers()) {
                arrayTeam1PlayerTurnUsed = {true};
                arrayTeam2PlayerTurnUsed = {true};
                playerCounter = 0;
                phaseCounter++;
            } else if (turnTeam1PlayerPhase) {
                do {
                    random = gameController::rng(0, 6 - environment.team1.numberOfBannedMembers());
                } while (!arrayTeam1PlayerTurnUsed[random]);
                random = gameController::rng(0, 6 - environment.team1.numberOfBannedMembers());
                arrayTeam1PlayerTurnUsed[random] = false;
                turnTeam1PlayerPhase = false;
                playerCounter++;
                return communication::messages::broadcast::Next(mapTeam1Player[random],
                                                                communication::messages::types::TurnType::MOVE,
                                                                environment.config.timeouts.playerPhase);
            } else {
                do {
                    random = gameController::rng(0, 6 - environment.team2.numberOfBannedMembers());
                } while (!arrayTeam2PlayerTurnUsed[random]);
                random = gameController::rng(0, 6 - environment.team2.numberOfBannedMembers());
                arrayTeam2PlayerTurnUsed[random] = false;
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
/*
 * Team Array Positionen:
 * 0 -> Keeper
 * 1 -> Seeker
 * 2 -> Beater1
 * 3 -> Beater2
 * 4 -> Chaser1
 * 5 -> Chaser2
 * 6 -> Chaser3
 */
auto Game::getSnapshot() const -> communication::messages::broadcast::Snapshot {
    return communication::messages::broadcast::Snapshot {deltaBroadcast, gamePhase.at(phaseCounter), {}, round,
                                                         communication::messages::broadcast::TeamSnapshot{pointsLeft, fansTeam1, environment.team1.seeker.get()->position.x, environment.team1.seeker.get()->position.y,
                                                                                                          environment.team1.seeker.get()->isFined, arrayTeam1PlayerTurnUsed[1], arrayTeam1PlayerKnockout[1],
                                                                                                          environment.team1.keeper.get()->position.x, environment.team1.keeper.get()->position.y,
                                                                                                          environment.team1.keeper.get()->isFined, quaffleHold.value_or(nullptr) == environment.team1.keeper,
                                                                                                          arrayTeam1PlayerTurnUsed[0], arrayTeam1PlayerKnockout[0],
                                                                                                          environment.team1.chasers[0].get()->position.x, environment.team1.chasers[0].get()->position.y,
                                                                                                          environment.team1.chasers[0].get()->isFined,quaffleHold == environment.team1.chasers[0], arrayTeam1PlayerTurnUsed[4],
                                                                                                          arrayTeam1PlayerKnockout[4],
                                                                                                          environment.team1.chasers[1].get()->position.x, environment.team1.chasers[1].get()->position.y,
                                                                                                          environment.team1.chasers[1].get()->isFined,quaffleHold == environment.team1.chasers[1], arrayTeam1PlayerTurnUsed[5],
                                                                                                          arrayTeam1PlayerKnockout[5],
                                                                                                          environment.team1.chasers[2].get()->position.x, environment.team1.chasers[2].get()->position.y,
                                                                                                          environment.team1.chasers[2].get()->isFined,quaffleHold == environment.team1.chasers[2], arrayTeam1PlayerTurnUsed[6],
                                                                                                          arrayTeam1PlayerKnockout[6],
                                                                                                          environment.team1.beaters[0].get()->position.x, environment.team1.beaters[0].get()->position.y,
                                                                                                          environment.team1.beaters[0].get()->isFined,bludgerHold == environment.team1.beaters[0], arrayTeam1PlayerTurnUsed[2],
                                                                                                          arrayTeam1PlayerKnockout[2],
                                                                                                          environment.team1.beaters[1].get()->position.x, environment.team1.beaters[1].get()->position.y,
                                                                                                          environment.team1.beaters[1].get()->isFined,bludgerHold == environment.team1.beaters[1], arrayTeam1PlayerTurnUsed[3],
                                                                                                          arrayTeam1PlayerKnockout[3]},
                                                         communication::messages::broadcast::TeamSnapshot{pointsRight, fansTeam2, environment.team2.seeker.get()->position.x, environment.team2.seeker.get()->position.y,
                                                                                                          environment.team2.seeker.get()->isFined, arrayTeam2PlayerTurnUsed[1], arrayTeam2PlayerKnockout[1],
                                                                                                          environment.team2.keeper.get()->position.x, environment.team2.keeper.get()->position.y,
                                                                                                          environment.team2.keeper.get()->isFined, quaffleHold.value_or(nullptr) == environment.team2.keeper,
                                                                                                          arrayTeam2PlayerTurnUsed[0], arrayTeam2PlayerKnockout[0],
                                                                                                          environment.team2.chasers[0].get()->position.x, environment.team2.chasers[0].get()->position.y,
                                                                                                          environment.team2.chasers[0].get()->isFined,quaffleHold == environment.team2.chasers[0], arrayTeam2PlayerTurnUsed[4],
                                                                                                          arrayTeam2PlayerKnockout[4],
                                                                                                          environment.team2.chasers[1].get()->position.x, environment.team2.chasers[1].get()->position.y,
                                                                                                          environment.team2.chasers[1].get()->isFined,quaffleHold == environment.team2.chasers[1], arrayTeam2PlayerTurnUsed[5],
                                                                                                          arrayTeam2PlayerKnockout[5],
                                                                                                          environment.team2.chasers[2].get()->position.x, environment.team2.chasers[2].get()->position.y,
                                                                                                          environment.team2.chasers[2].get()->isFined,quaffleHold == environment.team2.chasers[2], arrayTeam2PlayerTurnUsed[6],
                                                                                                          arrayTeam2PlayerKnockout[6],
                                                                                                          environment.team2.beaters[0].get()->position.x, environment.team2.beaters[0].get()->position.y,
                                                                                                          environment.team2.beaters[0].get()->isFined,bludgerHold == environment.team2.beaters[0], arrayTeam2PlayerTurnUsed[2],
                                                                                                          arrayTeam2PlayerKnockout[2],
                                                                                                          environment.team2.beaters[1].get()->position.x, environment.team2.beaters[1].get()->position.y,
                                                                                                          environment.team2.beaters[1].get()->isFined,bludgerHold == environment.team2.beaters[1], arrayTeam2PlayerTurnUsed[3],
                                                                                                          arrayTeam2PlayerKnockout[3]},
                                                                                                          //Ende of the TeamSnapShot
                                                                                                          environment.snitch.get()->position.x, environment.snitch.get()->position.y,
                                                                                                          environment.quaffle.get()->position.x, environment.quaffle.get()->position.y,
                                                                                                          environment.bludgers[0].get()->position.x, environment.bludgers[0].get()->position.y,
                                                                                                          environment.bludgers[1].get()->position.x, environment.bludgers[1].get()->position.y};
}

auto Game::getEndRound() const -> int {
    return round;
}

auto Game::getLeftPoints() const -> int {
    return pointsLeft;
}

auto Game::getRightPoints() const -> int {
    return pointsRight;
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