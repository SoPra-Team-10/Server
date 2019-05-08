//
// Created by bjorn on 02.05.19.
//

#include "Game.h"
#include "iostream"
#include <SopraGameLogic/GameController.h>
#include <SopraGameLogic/Interference.h>
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
        timer.pause();
        std::cout<<"pause() is called"<<std::endl;
    }

    void Game::resume() {
        timer.resume();
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

    bool Game::executeDelta(communication::messages::request::DeltaRequest command, TeamSide side) {
        if(roundState != GameState::PlayerPhase || roundState != GameState::InterferencePhase){
            return false;
        }

        switch (command.getDeltaType()){
            case communication::messages::types::DeltaType::SNITCH_CATCH:
                return false;
            case communication::messages::types::DeltaType::BLUDGER_BEATING:{
                if(command.getXPosNew().has_value() && command.getYPosNew().has_value() &&
                   command.getActiveEntity().has_value() && command.getPassiveEntity().has_value()){
                    try{
                        auto player = environment->getPlayerById(command.getActiveEntity().value());
                        if(!gameController::playerCanShoot(player, environment)){
                            return false;
                        }

                        gameModel::Position target(command.getXPosNew().value(), command.getYPosNew().value());
                        auto bludger = environment->getBallByID(command.getPassiveEntity().value());
                        gameController::Shot bShot(environment, player, bludger, target);
                        if(bShot.check() == gameController::ActionCheckResult::Impossible){
                            return false;
                        }

                        bShot.execute();
                        return true;
                    } catch (std::runtime_error &e){
                        fatalErrorListener(std::string(e.what()));
                        return false;
                    }
                } else {
                    return false;
                }
            }
            case communication::messages::types::DeltaType::QUAFFLE_THROW:{
                if(command.getPassiveEntity().has_value() && command.getActiveEntity().has_value() &&
                   command.getXPosNew().has_value() && command.getYPosNew().has_value() &&
                   command.getXPosOld().has_value() && command.getYPosOld().has_value()){
                    try{
                        auto player = environment->getPlayerById(command.getActiveEntity().value());
                        if(!gameController::playerCanShoot(player, environment)){
                            return false;
                        }

                        gameModel::Position target(command.getXPosNew().value(), command.getYPosNew().value());
                        gameController::Shot qThrow(environment, player, environment->quaffle, target);
                        if(qThrow.check() == gameController::ActionCheckResult::Impossible){
                            return false;
                        }

                        qThrow.execute();
                        return true;
                    } catch (std::runtime_error &e){
                        fatalErrorListener(std::string(e.what()));
                        return false;
                    }
                } else{
                    return false;
                }
            }
            case communication::messages::types::DeltaType::SNITCH_SNATCH:{
                try{
                    auto &team = getTeam(side);
                    gameController::SnitchPush sPush(environment, team);
                    if(!sPush.isPossible()){
                        return false;
                    }

                    sPush.execute();
                    return true;
                } catch (std::runtime_error &e){
                    fatalErrorListener(std::string(e.what()));
                    return false;
                }
            }
            case communication::messages::types::DeltaType::TROLL_ROAR:{
                try{
                    auto &team= getTeam(side);
                    gameController::Impulse impulse(environment, team);
                    if(!impulse.isPossible()){
                        return false;
                    }

                    impulse.execute();
                    return true;
                } catch (std::runtime_error &e){
                    fatalErrorListener(std::string(e.what()));
                    return false;
                }
            }
            case communication::messages::types::DeltaType::ELF_TELEPORTATION:{
                if(command.getPassiveEntity().has_value()){
                    try{
                        auto &team = getTeam(side);
                        auto targetPlayer = environment->getPlayerById(command.getPassiveEntity().value());
                        gameController::Teleport teleport(environment, team, targetPlayer);
                        if(!teleport.isPossible()){
                            return false;
                        }

                        teleport.execute();
                        return true;
                    } catch (std::runtime_error &e){
                        fatalErrorListener(std::string(e.what()));
                        return false;
                    }
                } else {
                    return false;
                }
            }
            case communication::messages::types::DeltaType::GOBLIN_SHOCK:
                if(command.getPassiveEntity().has_value()){
                    try{
                        auto &team = getTeam(side);
                        auto targetPlayer = environment->getPlayerById(command.getPassiveEntity().value());
                        gameController::RangedAttack rAttack(environment, team, targetPlayer);
                        if(!rAttack.isPossible()){
                            return false;
                        }

                        rAttack.execute();
                        return true;
                    } catch (std::runtime_error &e){
                        fatalErrorListener(std::string(e.what()));
                        return false;
                    }
                } else {
                    return false;
                }
            case communication::messages::types::DeltaType::BAN:
                return false;
            case communication::messages::types::DeltaType::BLUDGER_KNOCKOUT:
                return false;
            case communication::messages::types::DeltaType::MOVE:
                if(command.getActiveEntity().has_value() && command.getXPosNew().has_value() &&
                    command.getYPosNew().has_value()){
                    try{
                        auto player = environment->getPlayerById(command.getActiveEntity().value());
                        gameModel::Position target(command.getXPosNew().value(), command.getYPosNew().value());
                        gameController::Move move(environment, player, target);
                        if(move.check() == gameController::ActionCheckResult::Impossible){
                            return false;
                        }

                        move.execute();
                        return true;
                    } catch (std::runtime_error &e) {
                        fatalErrorListener(std::string(e.what()));
                        return false;
                    }
                } else {
                    return false;
                }
            case communication::messages::types::DeltaType::PHASE_CHANGE:
                return false;
            case communication::messages::types::DeltaType::GOAL_POINTS_CHANGE:
                return false;
            case communication::messages::types::DeltaType::ROUND_CHANGE:
                return false;
            case communication::messages::types::DeltaType::SKIP:
                return true;
            case communication::messages::types::DeltaType::UNBAN:
                return false;
            default:
                fatalErrorListener(std::string("Fatal error, DeltaType out of range! Possible memory corruption!"));
                return false;
        }
    }

    auto Game::getRound() const -> int {
        return roundNumber;
    }

    auto Game::getLeftPoints() const -> int {
        return environment->team1->score;
    }

    auto Game::getRightPoints() const -> int {
        return environment->team2->score;
    }

    auto Game::getSnapshot() const -> communication::messages::broadcast::Snapshot {
        return communication::messages::broadcast::Snapshot();
    }

    auto Game::executeBallDelta(communication::messages::types::EntityId entityId)
            -> communication::messages::request::DeltaRequest {
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

    auto Game::getTeam(TeamSide side) const -> std::shared_ptr<gameModel::Team> & {
        if(side == TeamSide::LEFT){
            return environment->team1;
        } else {
            return environment->team2;
        }
    }
}
