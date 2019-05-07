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
            phaseManager(environment->team1, environment->team2), lastDelta(communication::messages::types::DeltaType::ROUND_CHANGE,
                    {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, 0, {}){
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
            case PhaseType::BALL_PHASE:
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
                        roundState = PhaseType::PLAYER_PHASE;
                        return {EntityId::BLUDGER2, TurnType::MOVE, 0};
                    default:
                        throw std::runtime_error("Fatal Error! Inconsistent game state!");
                }

            case PhaseType::PLAYER_PHASE:{
                if(phaseManager.hasNextPlayer()){
                    return phaseManager.nextPlayer(environment);
                } else{
                    roundState = PhaseType::FAN_PHASE;
                    return getNextAction();
                }
            }
            case PhaseType::FAN_PHASE:
                if(phaseManager.hasNextInterference()){
                    return phaseManager.nextInterference(environment);
                } else {
                    roundState = PhaseType::BALL_PHASE;
                    endRound();
                    return getNextAction();
                }
            default:
                throw std::runtime_error("Fatal error, inconsistent game state!");
        }
    }

    bool Game::executeDelta(communication::messages::request::DeltaRequest command, TeamSide side) {
        using PType = communication::messages::types::PhaseType;
        if(roundState != PType::PLAYER_PHASE || roundState != PType::FAN_PHASE){
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
                if(command.getActiveEntity().has_value() && command.getXPosNew().has_value() &&
                command.getYPosNew().has_value()){
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
        return 0;
    }

    auto Game::getLeftPoints() const -> int {
        return 0;
    }

    auto Game::getRightPoints() const -> int {
        return 0;
    }

    auto Game::getSnapshot() const -> communication::messages::broadcast::Snapshot {
        using namespace communication::messages::broadcast;
        std::optional<int> snitchX = {};
        std::optional<int> snitchY = {};
        if(environment->snitch->exists){
            snitchX = environment->snitch->position.x;
            snitchY = environment->snitch->position.y;
        }
        return {lastDelta, roundState, {}, getRound(), teamToTeamSnapshot(environment->team1, TeamSide::LEFT),
                teamToTeamSnapshot(environment->team2, TeamSide::RIGHT), snitchX, snitchY, environment->quaffle->position.x,
                environment->quaffle->position.y, environment->bludgers[0]->position.x,
                environment->bludgers[0]->position.y, environment->bludgers[1]->position.x,
                environment->bludgers[1]->position.y};
    }

    void Game::executeBallDelta(communication::messages::types::EntityId entityId){
        using DType = communication::messages::types::DeltaType;
        std::shared_ptr<gameModel::Ball> ball;
        int oldX, oldY;

        if (entityId == communication::messages::types::EntityId::BLUDGER1 ||
            entityId == communication::messages::types::EntityId::BLUDGER2) {
            try{
                ball = environment->getBallByID(entityId);
                std::shared_ptr<gameModel::Bludger> bludger = std::dynamic_pointer_cast<gameModel::Bludger>(ball);

                if(!bludger){
                    fatalErrorListener(std::string{"We done fucked it up!"});
                }

                oldX = bludger->position.x;
                oldY = bludger->position.y;

                auto res = gameController::moveBludger(bludger, environment);
                if(res.has_value()){
                    oldX = res.value()->position.x;
                    oldY = res.value()->position.y;
                    lastDelta = {DType::BLUDGER_KNOCKOUT, res.value()->knockedOut, oldX, oldY, bludger->position.x, bludger->position.y,
                                 bludger->id, res.value()->id, roundState, {}, {}, {}, {}};
                } else {
                    lastDelta = {DType::MOVE, {}, oldX, oldY, bludger->position.x, bludger->position.y, bludger->id,
                                 {}, roundState, {}, {}, {}, {}};
                }
            } catch (std::runtime_error &e){
                fatalErrorListener(std::string{e.what()});
            }

        } else if (entityId == communication::messages::types::EntityId::SNITCH) {
            ball = environment->snitch;
            oldX = ball->position.x;
            oldY = ball->position.y;
            auto snitch = std::dynamic_pointer_cast<gameModel::Snitch>(ball);
            if(!snitch){
                fatalErrorListener(std::string{"We done fucked it up!"});
            }

            gameController::moveSnitch(snitch, environment);
            lastDelta = {DType::MOVE, {}, oldX, oldY, snitch->position.x, snitch->position.y, snitch->id,
                         {}, roundState, {}, {}, {}, {}};
        } else {
            throw std::runtime_error{"Quaffle or !ball passed to executeBallDelta!"};
        }
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

    auto Game::teamToTeamSnapshot(const std::shared_ptr<const gameModel::Team> &team, TeamSide side) const
        -> communication::messages::broadcast::TeamSnapshot {
        using FType = communication::messages::types::FanType;
        std::vector<communication::messages::broadcast::Fan> fans;
        fans.reserve(7);

        auto makeFans = [this, &fans, &team, &side](FType type){
            for(int i = 0; i < team->fanblock.getBannedCount(type); i++){
                fans.emplace_back(communication::messages::broadcast::Fan{type, true, false});
            }

            int used = side == TeamSide::LEFT ? phaseManager.interferencesUsedLeft(type) :
                    phaseManager.interferencesUsedRight(type);
            int left = team->fanblock.getUses(type) - used;
            for(int i = 0; i < used; i++){
                fans.emplace_back(communication::messages::broadcast::Fan{type, false, true});
            }

            for(int i = 0; i < left; i++){
                fans.emplace_back(communication::messages::broadcast::Fan{type, false, false});
            }
        };

        try{
            makeFans(FType::NIFFLER);
            makeFans(FType::TROLL);
            makeFans(FType::ELF);
            makeFans(FType::GOBLIN);
        } catch (std::runtime_error &e){
            fatalErrorListener(std::string{e.what()});
        }

        if(fans.size() != 7){
            fatalErrorListener(std::string{"Fanblock corrupt"});
        }

        communication::messages::broadcast::TeamSnapshot ret;

        try{
            if(side == TeamSide::LEFT){
                ret = {team->score, fans,

                       team->seeker->position.x, team->seeker->position.y,
                       team->seeker->isFined, phaseManager.playerUsedLeft(team->seeker->id),
                       team->seeker->knockedOut,

                       team->keeper->position.x, team->keeper->position.y, team->keeper->isFined,
                       team->keeper->position == environment->quaffle->position,
                       phaseManager.playerUsedLeft(team->keeper->id), team->keeper->knockedOut,

                       team->chasers[0]->position.x, team->chasers[0]->position.y, team->chasers[0]->isFined,
                       team->chasers[0]->position == environment->quaffle->position,
                       phaseManager.playerUsedLeft(team->chasers[0]->id), team->chasers[0]->knockedOut,

                       team->chasers[1]->position.x, team->chasers[1]->position.y, team->chasers[1]->isFined,
                       team->chasers[1]->position == environment->quaffle->position,
                       phaseManager.playerUsedLeft(team->chasers[1]->id), team->chasers[1]->knockedOut,

                       team->chasers[2]->position.x, team->chasers[2]->position.y, team->chasers[2]->isFined,
                       team->chasers[2]->position == environment->quaffle->position,
                       phaseManager.playerUsedLeft(team->chasers[2]->id), team->chasers[2]->knockedOut,

                       team->beaters[0]->position.x, team->beaters[0]->position.y, team->beaters[0]->isFined,
                       (team->beaters[0]->position == environment->bludgers[0]->position ||
                        team->beaters[0]->position == environment->bludgers[1]->position),
                       phaseManager.playerUsedLeft(team->beaters[0]->id), team->beaters[0]->knockedOut,

                       team->beaters[1]->position.x, team->beaters[1]->position.y, team->beaters[1]->isFined,
                       (team->beaters[1]->position == environment->bludgers[0]->position ||
                        team->beaters[1]->position == environment->bludgers[1]->position),
                       phaseManager.playerUsedLeft(team->beaters[1]->id), team->beaters[1]->knockedOut
                };
            } else {
                ret = {team->score, fans,

                       team->seeker->position.x, team->seeker->position.y,
                       team->seeker->isFined, phaseManager.playerUsedRight(team->seeker->id),
                       team->seeker->knockedOut,

                       team->keeper->position.x, team->keeper->position.y, team->keeper->isFined,
                       team->keeper->position == environment->quaffle->position,
                       phaseManager.playerUsedRight(team->keeper->id), team->keeper->knockedOut,

                       team->chasers[0]->position.x, team->chasers[0]->position.y, team->chasers[0]->isFined,
                       team->chasers[0]->position == environment->quaffle->position,
                       phaseManager.playerUsedRight(team->chasers[0]->id), team->chasers[0]->knockedOut,

                       team->chasers[1]->position.x, team->chasers[1]->position.y, team->chasers[1]->isFined,
                       team->chasers[1]->position == environment->quaffle->position,
                       phaseManager.playerUsedRight(team->chasers[1]->id), team->chasers[1]->knockedOut,

                       team->chasers[2]->position.x, team->chasers[2]->position.y, team->chasers[2]->isFined,
                       team->chasers[2]->position == environment->quaffle->position,
                       phaseManager.playerUsedRight(team->chasers[2]->id), team->chasers[2]->knockedOut,

                       team->beaters[0]->position.x, team->beaters[0]->position.y, team->beaters[0]->isFined,
                       (team->beaters[0]->position == environment->bludgers[0]->position ||
                        team->beaters[0]->position == environment->bludgers[1]->position),
                       phaseManager.playerUsedRight(team->beaters[0]->id), team->beaters[0]->knockedOut,

                       team->beaters[1]->position.x, team->beaters[1]->position.y, team->beaters[1]->isFined,
                       (team->beaters[1]->position == environment->bludgers[0]->position ||
                        team->beaters[1]->position == environment->bludgers[1]->position),
                       phaseManager.playerUsedRight(team->beaters[1]->id), team->beaters[1]->knockedOut
                };
            }
        } catch (std::runtime_error &e){
            fatalErrorListener(std::string{e.what()});
        }

        return ret;
    }
}
