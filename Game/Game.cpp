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
            phaseManager(environment->team1, environment->team2){
        std::cout<<"Constructor is called"<<std::endl;
    }

    void Game::pause() {
        std::cout<<"pause() is called"<<std::endl;
    }

    void Game::resume() {
        std::cout<<"resume() is called"<<std::endl;
    }

    auto Game::getNextAction() -> communication::messages::broadcast::Next {
        using namespace communication::messages::types;
        switch (roundState){
            case GameState::BallPhase:
                switch (ballTurn){
                    case EntityId ::SNITCH :
                        //Bludger1 turn next
                        ballTurn = EntityId::BLUDGER1;

                        //Snitch has to make move if it exists
                        if(environment->snitch->exists){
                            return {EntityId::SNITCH, TurnType::MOVE, 0};
                        } else {
                            return getNextAction();
                        }
                    case EntityId::BLUDGER1 :
                        //Bludger2 turn next
                        ballTurn = EntityId::BLUDGER2;
                        return {EntityId::BLUDGER1, TurnType::MOVE, 0};
                    case EntityId ::BLUDGER2 :
                        //Snitch turn next time entering ball phase
                        ballTurn = EntityId::SNITCH;
                        //Ball phase end, Player phase next
                        roundState = GameState::PlayerPhase;
                        return {EntityId::BLUDGER2, TurnType::MOVE, 0};
                    default:
                        throw std::runtime_error("Fatal Error! Inconsistent game state!");
                }

            case GameState::PlayerPhase:{
                if(phaseManager.hasNextPlayer()){
                    return phaseManager.nextPlayer(environment);
                } else{
                    roundState = GameState::InterferencePhase;
                    return getNextAction();
                }
            }
            case GameState::InterferencePhase:
                if(phaseManager.hasNextInterference()){
                    return phaseManager.nextInterference(environment);
                } else {
                    roundState = GameState::BallPhase;
                    endRound();
                    return getNextAction();
                }
            default:
                throw std::runtime_error("Fatal error, inconsistent game state!");
        }
    }

    bool Game::executeDelta(communication::messages::request::DeltaRequest) {
        return false;
    }

    auto Game::getRound() const -> int {
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

    auto Game::executeBallDelta(
            communication::messages::types::EntityId entityId) -> communication::messages::request::DeltaRequest {
        std::shared_ptr<gameModel::Ball> ball;
        int oldX, oldY;

        if (entityId == communication::messages::types::EntityId::BLUDGER1 ||
            entityId == communication::messages::types::EntityId::BLUDGER2) {
            std::shared_ptr<gameModel::Bludger> currBludger;
            for (auto &bludger : environment->bludgers) {
                if (bludger->id == entityId) {
                    ball = currBludger = bludger;
                    break;
                }
            }

            if (!currBludger) {
                throw std::runtime_error{"Ball for entity id not found, we fucked up!"};
            }
            oldX = currBludger->position.x;
            oldY = currBludger->position.y;

            gameController::moveBludger(currBludger, environment);
        } else if (entityId == communication::messages::types::EntityId::SNITCH) {
            ball = environment->snitch;
            oldX = ball->position.x;
            oldY = ball->position.y;
            //@TODO actually move the snitch
        } else {
            throw std::runtime_error{"Quaffle or !ball passed to executeBallDelta!"};
        }

        return communication::messages::request::DeltaRequest{
            communication::messages::types::DeltaType::MOVE,
            true,
            oldX,
            oldY,
            ball->position.x,
            ball->position.y,
            entityId,
            std::nullopt,
            communication::messages::types::PhaseType::BALL_PHASE,
            getLeftPoints(),
            getRightPoints(),
            getRound(),
            std::nullopt
        };
    }

    void Game::endRound() {
        phaseManager.reset();
    }
}
