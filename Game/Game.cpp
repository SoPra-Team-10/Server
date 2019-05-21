//
// Created by bjorn on 02.05.19.
//

#include "Game.h"
#include "iostream"
#include "conversions.h"
#include <SopraGameLogic/GameController.h>
#include <SopraGameLogic/Interference.h>
#include <SopraGameLogic/GameModel.h>

namespace gameHandling{
    Game::Game(communication::messages::broadcast::MatchConfig matchConfig, const communication::messages::request::TeamConfig& teamConfig1,
            const communication::messages::request::TeamConfig& teamConfig2, communication::messages::request::TeamFormation teamFormation1,
               communication::messages::request::TeamFormation teamFormation2, util::Logging &log) : environment(std::make_shared<gameModel::Environment>
                       (matchConfig, teamConfig1, teamConfig2, teamFormation1, teamFormation2)), phaseManager(environment->team1, environment->team2, environment),
                       lastDeltas(), log(log){
        lastDeltas.emplace(communication::messages::types::DeltaType::ROUND_CHANGE, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, 0, std::nullopt);
        log.debug("Constructed game");
    }

    void Game::pause() {
        timer.pause();
        log.debug("Pause is called");
    }

    void Game::resume() {
        timer.resume();
        log.debug("Resume is called");
    }

    auto Game::getNextAction() -> communication::messages::broadcast::Next {
        using namespace communication::messages::types;
        switch (currentPhase){
            case PhaseType::BALL_PHASE:
                switch (ballTurn){
                    case EntityId ::SNITCH :
                        //Bludger1 turn next
                        ballTurn = EntityId::BLUDGER1;

                        //Snitch has to make move if it exists
                        if(environment->snitch->exists){
                            log.debug("Snitch requested to make a move");
                            return {EntityId::SNITCH, TurnType::MOVE, 0};
                        } else {
                            log.debug("Snitch does not exists. Fetching next turn");
                            return getNextAction();
                        }
                    case EntityId::BLUDGER1 :
                        //Bludger2 turn next
                        ballTurn = EntityId::BLUDGER2;
                        log.debug("Bludger1 requested to make a move");
                        return {EntityId::BLUDGER1, TurnType::MOVE, 0};
                    case EntityId ::BLUDGER2 :
                        //Snitch turn next time entering ball phase
                        ballTurn = EntityId::SNITCH;
                        //Ball phase end, Player phase next
                        currentPhase = PhaseType::PLAYER_PHASE;
                        log.debug("Bludger2 requested to make a move");
                        log.debug("Ball phase over");
                        changePhaseDelta();
                        return {EntityId::BLUDGER2, TurnType::MOVE, 0};
                    default:
                        throw std::runtime_error("Fatal Error! Inconsistent game state!");
                }

            case PhaseType::PLAYER_PHASE:{
                auto next = phaseManager.nextPlayer();
                if(next.has_value()){
                    currentSide = conversions::idToSide(next.value().getEntityId());
                    timer.setTimeout(std::bind(&Game::onTimeout, this), environment->config.timeouts.playerTurn);
                    log.debug("Requested player turn");
                    return expectedRequestType = next.value();
                } else {
                    currentPhase = PhaseType::FAN_PHASE;
                    changePhaseDelta();
                    return getNextAction();
                }
            }

            case PhaseType::FAN_PHASE: {
                auto next = phaseManager.nextInterference();
                if(next.has_value()){
                    currentSide = conversions::idToSide(next.value().getEntityId());
                    timer.setTimeout(std::bind(&Game::onTimeout, this), environment->config.timeouts.fanTurn);
                    log.debug("Requested fan turn");
                    return expectedRequestType = next.value();
                } else {
                    currentPhase = PhaseType::BALL_PHASE;
                    changePhaseDelta();
                    return getNextAction();
                }
            }

            default:
                throw std::runtime_error("Fatal error, inconsistent game state!");
        }
    }

    bool Game::executeDelta(communication::messages::request::DeltaRequest command, TeamSide side) {
        using namespace communication::messages::types;
        //stop current timer since expected request arrived
        timer.stop();
        auto addFouls = [this](std::vector<gameModel::Foul> fouls, const std::shared_ptr<gameModel::Player> &player){
            for(const auto &foul : fouls){
                lastDeltas.emplace(DeltaType::BAN, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                        player->id, std::nullopt, std::nullopt, std::nullopt, std::nullopt, conversions::foulToBanReason(foul));
            }
        };

        //Request in wrong phase or request from wrong side
        if((currentPhase != PhaseType::PLAYER_PHASE && currentPhase != PhaseType::FAN_PHASE) ||
            currentSide != side){
            log.warn("Received request not allowed: Wrong Player or wrong phase");
            return false;
        }

        switch (command.getDeltaType()){
            case communication::messages::types::DeltaType::SNITCH_CATCH:
                log.warn("Illegal delta request type");
                return false;
            case communication::messages::types::DeltaType::BLUDGER_BEATING:{
                if(command.getXPosNew().has_value() && command.getYPosNew().has_value() &&
                   command.getActiveEntity().has_value() && command.getPassiveEntity().has_value()){
                    if(!conversions::isPlayer(command.getActiveEntity().value())  ||
                       !conversions::isBall(command.getPassiveEntity().value())){
                        log.warn("Invalid entities for blusger shot");
                        return false;
                    }

                    //Requested different actor or requested move instead of action
                    if(command.getActiveEntity().value() != expectedRequestType.getEntityId() ||
                        expectedRequestType.getTurnType() != TurnType::ACTION){
                        log.warn("Received request not allowed: Wrong entity or no action allowed");
                        return false;
                    }

                    try{
                        auto player = environment->getPlayerById(command.getActiveEntity().value());
                        gameModel::Position target(command.getXPosNew().value(), command.getYPosNew().value());
                        auto targetPlayer = environment->getPlayer(target);
                        auto bludger = environment->getBallByID(command.getPassiveEntity().value());
                        auto oldX = bludger->position.x;
                        auto oldY = bludger->position.y;
                        auto oldXQuaf = environment->quaffle->position.x;
                        auto oldYQuaf = environment->quaffle->position.y;
                        gameController::Shot bShot(environment, player, bludger, target);
                        if(bShot.check() == gameController::ActionCheckResult::Impossible){
                            log.warn("Bludger shot impossible");
                            return false;
                        }

                        auto res = bShot.execute();
                        addFouls(res.second, player);
                        for(const auto &result : res.first){
                            if(result == gameController::ActionResult::Knockout){
                                //Successful knockout
                                if(!targetPlayer.has_value()){
                                    fatalErrorListener(std::string{"Fatal error, inconsistent game state"});
                                    return false;
                                }

                                log.debug("Sucessful knockout by bludger");
                                lastDeltas.emplace(DeltaType::BLUDGER_KNOCKOUT, true, target.x, target.y,
                                                   bludger->position.x, bludger->position.y, bludger->id, targetPlayer.value()->id,
                                                   std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);
                            } else if(result == gameController::ActionResult::FoolAway){
                                //Knockout an quaffle lost
                                if(!targetPlayer.has_value()){
                                    fatalErrorListener(std::string{"Fatal error, inconsistent game state"});
                                    return false;
                                }

                                log.debug("Quaffle was lost due to bludger knockout");
                                lastDeltas.emplace(DeltaType::FOOL_AWAY, std::nullopt, oldXQuaf, oldYQuaf,
                                                   environment->quaffle->position.x, environment->quaffle->position.y,
                                                   environment->quaffle->id, targetPlayer.value()->id, std::nullopt, std::nullopt,
                                                   std::nullopt, std::nullopt, std::nullopt);
                            } else {
                                fatalErrorListener(std::string{"Unexpected action result"});
                                return false;
                            }
                        }

                        //Failed Knockout
                        if(res.first.empty() && targetPlayer.has_value() && bludger->position == targetPlayer.value()->position){
                            log.debug("Failed knockout by bludger");
                            lastDeltas.emplace(DeltaType::BLUDGER_KNOCKOUT, false, target.x, target.y,
                                             std::nullopt, std::nullopt, bludger->id, targetPlayer.value()->id, std::nullopt, std::nullopt,
                                             std::nullopt, std::nullopt, std::nullopt);
                        }

                        //Always send: bludger was shot
                        log.debug("Bludger shot");
                        lastDeltas.emplace(DeltaType::BLUDGER_BEATING, std::nullopt, oldX, oldY, bludger->position.x, bludger->position.y,
                                player->id, bludger->id, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);

                        //After shot -> turn over
                        lastDeltas.emplace(DeltaType::TURN_USED, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                player->id, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);
                        return true;
                    } catch (std::runtime_error &e){
                        fatalErrorListener(std::string(e.what()));
                        return false;
                    }
                } else {
                    log.warn("Bludger shot has insufficient information");
                    return false;
                }
            }
            case communication::messages::types::DeltaType::QUAFFLE_THROW:{
                if(command.getActiveEntity().has_value() && command.getXPosNew().has_value() &&
                command.getYPosNew().has_value()){
                    if(!conversions::isPlayer(command.getActiveEntity().value())){
                        log.warn("Invalid entity for quaffle throw");
                        return false;
                    }

                    //Requested different actor or requested move instead of action
                    if(command.getActiveEntity().value() != expectedRequestType.getEntityId() ||
                        expectedRequestType.getTurnType() != TurnType::ACTION){
                        log.debug("Received request not allowed: Wrong entity or no action allowed");
                        return false;
                    }

                    try{
                        auto player = environment->getPlayerById(command.getActiveEntity().value());
                        gameModel::Position target(command.getXPosNew().value(), command.getYPosNew().value());
                        auto oldX = environment->quaffle->position.x;
                        auto oldY = environment->quaffle->position.y;
                        gameController::Shot qThrow(environment, player, environment->quaffle, target);
                        if(qThrow.check() == gameController::ActionCheckResult::Impossible){
                            log.warn("Quaffle throw impossible");
                            return false;
                        }

                        auto res = qThrow.execute();
                        auto destPlayer = environment->getPlayer(environment->quaffle->position);
                        std::optional<EntityId > destPlayerId = {};
                        if(destPlayer.has_value()){
                            destPlayerId = destPlayer.value()->id;
                        }

                        addFouls(res.second, player);
                        bool success = false;
                        for(const auto &result : res.first){
                            if(result == gameController::ActionResult::ScoreLeft ||
                                result == gameController::ActionResult::ScoreRight){
                                //Notify: goal was scored
                                log.debug("Goal was scored");
                                lastDeltas.emplace(DeltaType::GOAL_POINTS_CHANGE, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                                   std::nullopt, std::nullopt, std::nullopt, std::nullopt, environment->team1->score,
                                                   environment->team2->score, std::nullopt, std::nullopt);
                            } else if(result == gameController::ActionResult::ThrowSuccess){
                                success = true;
                            } else if(result != gameController::ActionResult::Intercepted &&
                                result != gameController::ActionResult::Miss){
                                fatalErrorListener(std::string{"Unexpected action result"});
                                return false;
                            }
                        }

                        if(success){
                            log.debug("Successful throw");
                        } else {
                            log.debug("Failed throw");
                        }

                        lastDeltas.emplace(DeltaType ::QUAFFLE_THROW, success, oldX, oldY, environment->quaffle->position.x,
                                         environment->quaffle->position.y, player->id, destPlayerId,
                                         std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);

                        lastDeltas.emplace(DeltaType::TURN_USED, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                player->id, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);

                        return true;
                    } catch (std::runtime_error &e){
                        fatalErrorListener(std::string(e.what()));
                        return false;
                    }
                } else{
                    log.warn("Quffle throw has insufficient information");
                    return false;
                }
            }
            case communication::messages::types::DeltaType::SNITCH_SNATCH:{
                if(expectedRequestType.getTurnType() != TurnType::FAN){
                    log.warn("Interference request but not in fan phase");
                    return false;
                }

                try{
                    auto &team = getTeam(side);
                    gameController::SnitchPush sPush(environment, team);
                    if(!sPush.isPossible()){
                        log.warn("Snitch push is impossible");
                        return false;
                    }

                    if(overTimeState != gameController::ExcessLength::None){
                        log.debug("Overtime, snitch push has no effect");
                        return true;
                    }

                    auto oldX = environment->snitch->position.x;
                    auto oldY = environment->snitch->position.y;
                    if(sPush.execute() == gameController::ActionCheckResult::Foul){
                        log.debug("Snitch push was detected as foul");
                        lastDeltas.emplace(DeltaType::BAN, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                         std::nullopt, conversions::interferenceToId(gameModel::InterferenceType::SnitchPush, side),
                                         std::nullopt, std::nullopt, std::nullopt, std::nullopt, BanReason::SNITCH_SNATCH);
                    }

                    log.debug("Snitch push");
                    lastDeltas.emplace(DeltaType::SNITCH_SNATCH, std::nullopt, oldX, oldY, environment->snitch->position.x, environment->snitch->position.y,
                                     conversions::interferenceToId(gameModel::InterferenceType::SnitchPush, side), environment->snitch->id,
                                     std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);
                    return true;
                } catch (std::runtime_error &e){
                    fatalErrorListener(std::string(e.what()));
                    return false;
                }
            }
            case communication::messages::types::DeltaType::TROLL_ROAR:{
                if(expectedRequestType.getTurnType() != TurnType::FAN){
                    log.warn("Interference request but not in fan phase");
                    return false;
                }

                try{
                    auto &team = getTeam(side);
                    gameController::Impulse impulse(environment, team);
                    if(!impulse.isPossible()){
                        log.warn("Impulse is impossible");
                        return false;
                    }

                    auto oldX = environment->quaffle->position.x;
                    auto oldY = environment->quaffle->position.y;
                    auto holdingPlayer = environment->getPlayer(environment->quaffle->position);
                    std::optional<EntityId> holdingPlayerId = {};
                    if(holdingPlayer.has_value()){
                        holdingPlayerId = holdingPlayer.value()->id;
                    }

                    if(impulse.execute() == gameController::ActionCheckResult::Foul){
                        log.debug("Impusle was detected as foul");
                        lastDeltas.emplace(DeltaType::BAN, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                         std::nullopt, conversions::interferenceToId(gameModel::InterferenceType::Impulse, side),
                                         std::nullopt, std::nullopt, std::nullopt, std::nullopt,  BanReason::TROLL_ROAR);
                    }

                    auto playerWithQuaffle = environment->getPlayer({oldX, oldY});
                    if(playerWithQuaffle.has_value()){
                        log.debug("Quaffle was lost due to impusle");
                        lastDeltas.emplace(DeltaType::FOOL_AWAY, std::nullopt, oldX, oldY, environment->quaffle->position.x,
                                         environment->quaffle->position.y, environment->quaffle->id, playerWithQuaffle.value()->id,
                                         std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);
                    }

                    log.debug("Impulse");
                    lastDeltas.emplace(DeltaType::TROLL_ROAR, std::nullopt, oldX, oldY, environment->quaffle->position.x, environment->quaffle->position.y,
                                     conversions::interferenceToId(gameModel::InterferenceType::Impulse, side), holdingPlayerId,
                                     std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);
                    return true;
                } catch (std::runtime_error &e){
                    fatalErrorListener(std::string(e.what()));
                    return false;
                }
            }
            case communication::messages::types::DeltaType::ELF_TELEPORTATION:{
                if(expectedRequestType.getTurnType() != TurnType::FAN){
                    log.warn("Interference request but not in fan phase");
                    return false;
                }

                if(command.getPassiveEntity().has_value()){
                    if(!conversions::isPlayer(command.getPassiveEntity().value())){
                        log.warn("Teleport target is no player");
                        return false;
                    }

                    try{
                        auto &team = getTeam(side);
                        auto targetPlayer = environment->getPlayerById(command.getPassiveEntity().value());
                        auto oldX = targetPlayer->position.x;
                        auto oldY = targetPlayer->position.y;
                        gameController::Teleport teleport(environment, team, targetPlayer);
                        if(!teleport.isPossible()){
                            log.warn("Teleport is impossible");
                            return false;
                        }

                        if(teleport.execute() == gameController::ActionCheckResult::Foul){
                            log.debug("Teleport was detected as foul");
                            lastDeltas.emplace(DeltaType::BAN, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                          conversions::interferenceToId(gameModel::InterferenceType::Teleport, side),
                                         std::nullopt, std::nullopt, std::nullopt, std::nullopt, BanReason::ELF_TELEPORTATION);
                        }

                        //@TODO was tun, wenn target den quaffle hält?
                        log.debug("Teleport");
                        lastDeltas.emplace(DeltaType::ELF_TELEPORTATION, std::nullopt, oldX, oldY, targetPlayer->position.x, targetPlayer->position.y,
                                         conversions::interferenceToId(gameModel::InterferenceType::Teleport, side), targetPlayer->id,
                                         std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);
                        return true;
                    } catch (std::runtime_error &e){
                        fatalErrorListener(std::string(e.what()));
                        return false;
                    }
                } else {
                    log.warn("Teleport request has insufficient information");
                    return false;
                }
            }
            case communication::messages::types::DeltaType::GOBLIN_SHOCK:
                if(expectedRequestType.getTurnType() != TurnType::FAN){
                    log.warn("Interference request but not in fan phase");
                    return false;
                }

                if(command.getPassiveEntity().has_value()){
                    if(!conversions::isPlayer(command.getPassiveEntity().value())){
                        return false;
                    }

                    try{
                        auto &team = getTeam(side);
                        auto targetPlayer = environment->getPlayerById(command.getPassiveEntity().value());
                        auto oldX = targetPlayer->position.x;
                        auto oldY = targetPlayer->position.y;
                        auto oldXQuaf = environment->quaffle->position.x;
                        auto oldYQuaf = environment->quaffle->position.y;
                        gameController::RangedAttack rAttack(environment, team, targetPlayer);
                        if(!rAttack.isPossible()){
                            log.warn("Ranged attack is impossible");
                            return false;
                        }

                        if(rAttack.execute() == gameController::ActionCheckResult::Foul){
                            log.debug("Ranged attack was detected as foul");
                          lastDeltas.emplace(DeltaType::BAN, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                           conversions::interferenceToId(gameModel::InterferenceType::RangedAttack, side),
                                         std::nullopt, std::nullopt, std::nullopt, std::nullopt, BanReason::GOBLIN_SHOCK);
                        }

                        if(gameModel::Position{oldX, oldY} == gameModel::Position{oldXQuaf, oldYQuaf}){
                            log.debug("Quaffle was lost due to ranged attack");
                            lastDeltas.emplace(DeltaType::FOOL_AWAY, std::nullopt, oldXQuaf, oldYQuaf,
                                         environment->quaffle->position.x, environment->quaffle->position.y,
                                         environment->quaffle->id, targetPlayer->id, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);
                        }

                        log.debug("Ranged attack");
                        lastDeltas.emplace(DeltaType::GOBLIN_SHOCK, std::nullopt, oldX, oldY, targetPlayer->position.x, targetPlayer->position.y,
                                         conversions::interferenceToId(gameModel::InterferenceType::RangedAttack, side),
                                         targetPlayer->id, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);

                        return true;
                    } catch (std::runtime_error &e){
                        fatalErrorListener(std::string(e.what()));
                        return false;
                    }
                } else {
                    log.warn("Ranged attack request has insufficient information");
                    return false;
                }
            case communication::messages::types::DeltaType::BAN:
                log.warn("Illegal delta request type");
                return false;
            case communication::messages::types::DeltaType::BLUDGER_KNOCKOUT:
                log.warn("Illegal delta request type");
                return false;
            case communication::messages::types::DeltaType::MOVE:
                if(command.getActiveEntity().has_value() && command.getXPosNew().has_value() &&
                    command.getYPosNew().has_value()){
                    if(!conversions::isPlayer(command.getActiveEntity().value())){
                        log.warn("Moving entity is no player");
                        return false;
                    }

                    if(command.getActiveEntity().value() != expectedRequestType.getEntityId() ||
                        expectedRequestType.getTurnType() != TurnType::MOVE){
                        log.warn("Received request not allowed: Wrong entity or no action allowed");
                        return false;
                    }

                    try{
                        auto player = environment->getPlayerById(command.getActiveEntity().value());
                        auto oldX = player->position.x;
                        auto oldY = player->position.y;
                        auto oldXQuaf = environment->quaffle->position.x;
                        auto oldYQuaf = environment->quaffle->position.y;
                        gameModel::Position target(command.getXPosNew().value(), command.getYPosNew().value());
                        auto targetPlayer = environment->getPlayer(target);
                        gameController::Move move(environment, player, target);
                        if(move.check() == gameController::ActionCheckResult::Impossible){
                            log.warn("Move is impossible");
                            return false;
                        }

                        auto res = move.execute();
                        addFouls(res.second, player);
                        bool snitchCought = false;
                        for(const auto &result : res.first){
                            if(result == gameController::ActionResult::ScoreRight ||
                                result == gameController::ActionResult::ScoreLeft){
                                log.debug("Goal was scored");
                                lastDeltas.emplace(DeltaType::GOAL_POINTS_CHANGE, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                                   std::nullopt, std::nullopt, std::nullopt, std::nullopt, environment->team1->score,
                                                   environment->team2->score, std::nullopt, std::nullopt);
                            } else if(result == gameController::ActionResult::FoolAway){
                                if(!targetPlayer.has_value()){
                                    fatalErrorListener(std::string{"Inconsistent game state at Ramming foul"});
                                    return false;
                                }

                                log.debug("Quaffle was lost due to ramming");
                                lastDeltas.emplace(DeltaType::FOOL_AWAY, std::nullopt, oldXQuaf, oldYQuaf, environment->quaffle->position.x,
                                                   environment->quaffle->position.y, environment->quaffle->id, targetPlayer.value()->id, std::nullopt,
                                                   std::nullopt, std::nullopt, std::nullopt, std::nullopt);
                            } else if(result == gameController::ActionResult::SnitchCatch){
                                snitchCought = true;
                            } else {
                                fatalErrorListener(std::string{"Unexpected action result"});
                                return false;
                            }
                        }

                        if(environment->snitch->position == player->position && (std::dynamic_pointer_cast<gameModel::Seeker>(player))){
                            if(!snitchCought){
                                log.debug("Failed to catch snitch");
                            }

                            lastDeltas.emplace(DeltaType::SNITCH_CATCH, snitchCought, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                             player->id, std::nullopt, std::nullopt, environment->team1->score, environment->team2->score,
                                             std::nullopt, std::nullopt);
                        }

                        log.debug("Move");
                        lastDeltas.emplace(DeltaType::MOVE, std::nullopt, oldX, oldY, player->position.x, player->position.y,
                                         player->id, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);

                        if(phaseManager.playerUsed(player)){
                            lastDeltas.emplace(DeltaType::TURN_USED, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                    player->id, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);
                        }

                        if(snitchCought){
                            log.debug("Snitch was caught");
                            lastDeltas.emplace(DeltaType::GOAL_POINTS_CHANGE, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                               std::nullopt, std::nullopt, std::nullopt, std::nullopt, environment->team1->score,
                                               environment->team2->score, std::nullopt, std::nullopt);
                            auto winningTeam = getVictoriousTeam(player);
                            winListener(winningTeam.first, winningTeam.second);
                        }

                        return true;
                    } catch (std::runtime_error &e) {
                        fatalErrorListener(std::string(e.what()));
                        return false;
                    }
                } else {
                    log.warn("Move request has insufficient information");
                    return false;
                }
            case communication::messages::types::DeltaType::PHASE_CHANGE:
                log.warn("Illegal delta request type");
                return false;
            case communication::messages::types::DeltaType::GOAL_POINTS_CHANGE:
                log.warn("Illegal delta request type");
                return false;
            case communication::messages::types::DeltaType::ROUND_CHANGE:
                log.warn("Illegal delta request type");
                return false;
            case communication::messages::types::DeltaType::SKIP:
                if(command.getActiveEntity().has_value()){
                    if(command.getActiveEntity() != expectedRequestType.getEntityId()){
                        log.warn("Received request not allowed: Wrong entity or no action allowed");
                        return false;
                    }

                    log.debug("Turn skipped");
                    lastDeltas.emplace(DeltaType::SKIP, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                     command.getActiveEntity().value(), std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                     std::nullopt, std::nullopt);
                    return true;
                } else {
                    log.warn("Skip request has insufficient information");
                    return false;
                }
            case communication::messages::types::DeltaType::UNBAN:
                if(command.getActiveEntity().has_value() && command.getXPosNew().has_value() &&
                    command.getYPosNew().has_value()){
                    if(!conversions::isPlayer(command.getActiveEntity().value())){
                        log.warn("Unban entity is no player");
                        return false;
                    }

                    if(command.getActiveEntity() != expectedRequestType.getEntityId()){
                        log.warn("Received request not allowed: Wrong entity or no action allowed");
                        return false;
                    }

                    try {
                        auto player = environment->getPlayerById(command.getActiveEntity().value());
                        if(!player->isFined){
                            log.warn("Player is not banned");
                            return false;
                        }

                        gameModel::Position target{command.getXPosNew().value(), command.getYPosNew().value()};
                        if(!environment->cellIsFree(target)){
                            log.warn("Invalid target for unban! Cell is occupied");
                            return false;
                        }

                        log.debug("Unban");
                        player->position = target;
                        player->isFined = false;
                        lastDeltas.emplace(DeltaType::UNBAN, std::nullopt, std::nullopt, std::nullopt, target.x, target.y, player->id,
                                         std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);
                        return true;
                    } catch (std::runtime_error &e) {
                        fatalErrorListener(std::string(e.what()));
                        return false;
                    }

                } else {
                    log.warn("Unban request has insufficient information");
                    return false;
                }
            case communication::messages::types::DeltaType::WREST_QUAFFLE:
                if(command.getActiveEntity().has_value()){
                    if(command.getActiveEntity().value() != expectedRequestType.getEntityId() ||
                        expectedRequestType.getTurnType() != TurnType::ACTION){
                        log.warn("Received request not allowed: Wrong entity or no action allowed");
                        return false;
                    }

                    try{
                        auto player = std::dynamic_pointer_cast<gameModel::Chaser>(environment->getPlayerById(command.getActiveEntity().value()));
                        auto targetPlayer = environment->getPlayer(environment->quaffle->position);
                        if(!player || !targetPlayer.has_value()){
                            log.warn("Wresting player is no Chaser or no player on target");
                            return false;
                        }

                        gameController::WrestQuaffle wQuaffle(environment, player, environment->quaffle->position);
                        if(wQuaffle.check() == gameController::ActionCheckResult::Impossible){
                            log.warn("Wrest is impossible");
                            return false;
                        }

                        auto oldX = environment->quaffle->position.x;
                        auto oldY = environment->quaffle->position.y;
                        auto res = wQuaffle.execute();
                        addFouls(res.second, player);
                        if(res.first.size() > 1){
                            fatalErrorListener(std::string{"Unexpected action result"});
                            return false;
                        }

                        lastDeltas.emplace(DeltaType::WREST_QUAFFLE, !res.first.empty(), oldX, oldY, environment->quaffle->position.x,
                                environment->quaffle->position.y, player->id, targetPlayer.value()->id, std::nullopt, std::nullopt, std::nullopt,
                                std::nullopt, std::nullopt);
                        return true;
                    } catch(std::runtime_error &e){
                        fatalErrorListener(std::string{e.what()});
                        return false;
                    }
                } else {
                    log.warn("Wrest request has insufficient information");
                    return false;
                }
            default:
                fatalErrorListener(std::string("Fatal error, DeltaType out of range! Possible memory corruption!"));
                return false;
        }
    }

    auto Game::getRound() const -> int {
        return roundNumber;
    }

    auto Game::getLeftPoints() const -> int {
        return getTeam(TeamSide::LEFT)->score;
    }

    auto Game::getRightPoints() const -> int {
        return getTeam(TeamSide::RIGHT)->score;
    }

    auto Game::getSnapshot() -> std::queue<communication::messages::broadcast::Snapshot> {
        using namespace communication::messages::broadcast;
        std::queue<Snapshot> ret;
        while (!lastDeltas.empty()){
            std::optional<int> snitchX = {};
            std::optional<int> snitchY = {};
            if(environment->snitch->exists){
                snitchX = environment->snitch->position.x;
                snitchY = environment->snitch->position.y;
            }

            ret.emplace(lastDeltas.front(), currentPhase, std::vector<std::string>{}, getRound(), teamToTeamSnapshot(environment->team1, TeamSide::LEFT),
                teamToTeamSnapshot(environment->team2, TeamSide::RIGHT), snitchX, snitchY, environment->quaffle->position.x,
                environment->quaffle->position.y, environment->bludgers[0]->position.x, environment->bludgers[0]->position.y,
                environment->bludgers[1]->position.x, environment->bludgers[1]->position.y,
                std::vector<std::pair<int,int>>{}, false); // @TODO change last two params
            lastDeltas.pop();
        }

        return ret;
    }

    void Game::executeBallDelta(communication::messages::types::EntityId entityId){
        using DType = communication::messages::types::DeltaType;
        std::shared_ptr<gameModel::Ball> ball;
        int oldX, oldY;
        changePhaseDelta();

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
                    lastDeltas.emplace(DType::BLUDGER_KNOCKOUT, res.value()->knockedOut, oldX, oldY, bludger->position.x, bludger->position.y,
                                 bludger->id, res.value()->id, currentPhase, std::nullopt, std::nullopt, std::nullopt, std::nullopt);
                }

                lastDeltas.emplace(DType::MOVE, std::nullopt, oldX, oldY, bludger->position.x, bludger->position.y, bludger->id,
                             std::nullopt, currentPhase, std::nullopt, std::nullopt, std::nullopt, std::nullopt);
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

            bool caught = gameController::moveSnitch(snitch, environment, overTimeState);
            lastDeltas.emplace(DType::MOVE, std::nullopt, oldX, oldY, snitch->position.x, snitch->position.y, snitch->id,
                         std::nullopt, currentPhase, std::nullopt, std::nullopt, std::nullopt, std::nullopt);
            if(caught){
                auto catcher = environment->getPlayer(environment->snitch->position);
                if(!catcher.has_value()){
                    fatalErrorListener(std::string{"Fatal error! Snitch did not collide with a seeker"});
                }

                lastDeltas.emplace(DType::SNITCH_CATCH, true, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                   catcher.value()->id, std::nullopt, std::nullopt, environment->team1->score, environment->team2->score,
                                   std::nullopt, std::nullopt);

                lastDeltas.emplace(DType::GOAL_POINTS_CHANGE, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                   std::nullopt, std::nullopt, std::nullopt, std::nullopt, environment->team1->score,
                                   environment->team2->score, std::nullopt, std::nullopt);
                auto winningTeam = getVictoriousTeam(catcher.value());
                winListener(winningTeam.first, winningTeam.second);
            }
        } else {
            throw std::runtime_error{"Quaffle or !ball passed to executeBallDelta!"};
        }
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

        try{
            bool keeperHoldsQuaffle = team->keeper->position == environment->quaffle->position;
            bool chaser1HoldsQuaffle = team->chasers[0]->position == environment->quaffle->position;
            bool chaser2HoldsQuaffle = team->chasers[1]->position == environment->quaffle->position;
            bool chaser3HoldsQuaffle = team->chasers[2]->position == environment->quaffle->position;
            bool beater1holdsBludger = team->beaters[0]->position == environment->bludgers[0]->position ||
                                       team->beaters[0]->position == environment->bludgers[1]->position;
            bool beater2holdsBludger = team->beaters[1]->position == environment->bludgers[0]->position ||
                                       team->beaters[1]->position == environment->bludgers[1]->position;
            return {
                team->score, fans,
                team->seeker->position.x, team->seeker->position.y,
                team->seeker->isFined, phaseManager.playerUsed(team->seeker),
                team->seeker->knockedOut,

                team->keeper->position.x, team->keeper->position.y, team->keeper->isFined,
                keeperHoldsQuaffle, phaseManager.playerUsed(team->keeper), team->keeper->knockedOut,

                team->chasers[0]->position.x, team->chasers[0]->position.y, team->chasers[0]->isFined,
                chaser1HoldsQuaffle, phaseManager.playerUsed(team->chasers[0]), team->chasers[0]->knockedOut,

                team->chasers[1]->position.x, team->chasers[1]->position.y, team->chasers[1]->isFined,
                chaser2HoldsQuaffle, phaseManager.playerUsed(team->chasers[1]), team->chasers[1]->knockedOut,

                team->chasers[2]->position.x, team->chasers[2]->position.y, team->chasers[2]->isFined,
                chaser3HoldsQuaffle, phaseManager.playerUsed(team->chasers[2]), team->chasers[2]->knockedOut,

                team->beaters[0]->position.x, team->beaters[0]->position.y, team->beaters[0]->isFined,
                beater1holdsBludger, phaseManager.playerUsed(team->beaters[0]), team->beaters[0]->knockedOut,

                team->beaters[1]->position.x, team->beaters[1]->position.y, team->beaters[1]->isFined,
                beater2holdsBludger, phaseManager.playerUsed(team->beaters[1]), team->beaters[1]->knockedOut
            };
        } catch (std::runtime_error &e){
            fatalErrorListener(std::string{e.what()});
            return {};
        }
    }

    void Game::changePhaseDelta() {
        using namespace communication::messages::types;
        lastDeltas.emplace(DeltaType::PHASE_CHANGE, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                std::nullopt, std::nullopt, currentPhase, std::nullopt, std::nullopt, std::nullopt, std::nullopt);

    }

    void Game::endRound() {
        using namespace communication::messages::types;
        //All fans had their turn
        if(!phaseManager.hasInterference()){
            roundNumber++;
            phaseManager.reset();
            lastDeltas.emplace(DeltaType::ROUND_CHANGE, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                               std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, getRound(), std::nullopt);
            switch (overTimeState){
                case gameController::ExcessLength::None:
                    if(roundNumber > environment->config.maxRounds){
                        overTimeState = gameController::ExcessLength::Stage1;
                    }

                    break;
                case gameController::ExcessLength::Stage1:
                    if(++overTimeCounter > 3){
                        overTimeState = gameController::ExcessLength::Stage2;
                        overTimeCounter = 0;
                    }
                    break;
                case gameController::ExcessLength::Stage2:
                    if(environment->snitch->position == gameModel::Position{8, 6} &&
                        ++overTimeCounter > 3){
                        overTimeState = gameController::ExcessLength::Stage3;
                    }
                    break;
                case gameController::ExcessLength::Stage3:
                    break;
            }
        }
    }

    void Game::onTimeout() {
        log.debug("Timeout");
        timeoutListener(expectedRequestType.getEntityId(), currentPhase);
    }

    auto Game::getVictoriousTeam(const std::shared_ptr<gameModel::Player> &winningPlayer) const -> std::pair<TeamSide,
    communication::messages::types::VictoryReason> {
        using namespace communication::messages::types;
        if(environment->team1->score > environment->team2->score){
            return {TeamSide::LEFT, VictoryReason::MOST_POINTS};
        } else if(environment->team1->score < environment->team2->score){
            return {TeamSide::RIGHT, VictoryReason::MOST_POINTS};
        } else {
            if(environment->team1->hasMember(winningPlayer)){
                return {TeamSide::LEFT, VictoryReason::POINTS_EQUAL_SNITCH_CATCH};
            } else {
                return {TeamSide::RIGHT, VictoryReason::POINTS_EQUAL_SNITCH_CATCH};
            }
        }
    }
}
