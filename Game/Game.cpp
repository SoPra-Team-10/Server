//
// Created by bjorn on 02.05.19.
//

#include "Game.h"
#include "iostream"
#include <SopraGameLogic/GameController.h>
#include <SopraGameLogic/GameModel.h>

namespace gameHandling{
    Game::Game(communication::messages::broadcast::MatchConfig matchConfig,
               const communication::messages::request::TeamConfig& teamConfig1,
               const communication::messages::request::TeamConfig& teamConfig2,
               communication::messages::request::TeamFormation teamFormation1,
               communication::messages::request::TeamFormation teamFormation2) :
            environment(std::make_shared<gameModel::Environment> (matchConfig, teamConfig1, teamConfig2, teamFormation1, teamFormation2)),
            playerPhaseManager(environment->team1, environment->team2){
        std::cout<<"Constructor is called"<<std::endl;
    }

    void Game::pause() {
        std::cout<<"pause() is called"<<std::endl;
    }

    void Game::resume() {
        std::cout<<"resume() is called"<<std::endl;
    }

    auto Game::getNextActor() -> communication::messages::broadcast::Next {
        using namespace communication::messages::types;
        switch (roundState){
            case GameState::BallPhase:
                switch (ballTurn){
                    case EntityId ::SNITCH :
                        //Bludger1 turn next
                        ballTurn = EntityId::BLUDGER1;

                        //Snitch has to make move if it exists
                        if(environment->snitch->exists){
                            return {ballTurn, TurnType::MOVE, 0};
                        }
                    case EntityId::BLUDGER1 :
                        //Bludger2 turn next
                        ballTurn = EntityId::BLUDGER2;
                        return {ballTurn, TurnType::MOVE, 0};
                    case EntityId ::BLUDGER2 :
                        //Snitch turn next time entering ball phase
                        ballTurn = EntityId::SNITCH;
                        //Ball phase end, Player phase next
                        roundState = GameState::PlayerPhase;
                        return {ballTurn, TurnType::MOVE, 0};
                    default:
                        throw std::runtime_error("Fatal Error! Inconsistent game state!");
                }

            case GameState::PlayerPhase:
               if(playerPhaseManager.hasNextPlayer()){
                   return playerPhaseManager.nextPlayer(environment);
               } else{
                   playerPhaseManager.resetPlayers();
                   roundState = GameState::InterferencePhase;
               }

            case GameState::InterferencePhase:break;
        }


        return communication::messages::broadcast::Next();
    }

    bool Game::executeDelta(communication::messages::request::DeltaRequest) {
        return false;
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

    auto Game::getSnapshot() const -> communication::messages::broadcast::Snapshot {
        return communication::messages::broadcast::Snapshot();
    }

}
