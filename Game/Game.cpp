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
               communication::messages::request::TeamFormation teamFormation2) : environment(std::make_shared<gameModel::Environment>
                       (matchConfig, teamConfig1, teamConfig2, teamFormation1, teamFormation2)), phaseManager(environment->team1, environment->team2){
        lastDeltas.emplace(communication::messages::types::DeltaType::ROUND_CHANGE, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, 0, std::nullopt);
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
                    phaseManager.reset();
                    return getNextAction();
                }
            default:
                throw std::runtime_error("Fatal error, inconsistent game state!");
        }
    }

    bool Game::executeDelta(communication::messages::request::DeltaRequest command, TeamSide side) {
        using namespace communication::messages::types;

        auto addFouls = [this](std::vector<gameModel::Foul> fouls, const std::shared_ptr<gameModel::Player> &player){
            for(const auto &foul : fouls){
                lastDeltas.emplace(DeltaType::BAN, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                        player->id, std::nullopt, std::nullopt, std::nullopt, std::nullopt, conversions::foulToBanReason(foul));
            }
        };

        if(roundState != PhaseType::PLAYER_PHASE || roundState != PhaseType::FAN_PHASE){
            return false;
        }

        switch (command.getDeltaType()){
            case communication::messages::types::DeltaType::SNITCH_CATCH:
                return false;
            case communication::messages::types::DeltaType::BLUDGER_BEATING:{
                if(command.getXPosNew().has_value() && command.getYPosNew().has_value() &&
                   command.getActiveEntity().has_value() && command.getPassiveEntity().has_value()){
                    if(!conversions::isPlayer(command.getActiveEntity().value())  ||
                       !conversions::isBall(command.getPassiveEntity().value())){
                        return false;
                    }

                    try{
                        auto player = environment->getPlayerById(command.getActiveEntity().value());
                        if(!gameController::playerCanShoot(player, environment)){
                            return false;
                        }

                        gameModel::Position target(command.getXPosNew().value(), command.getYPosNew().value());
                        auto bludger = environment->getBallByID(command.getPassiveEntity().value());
                        auto oldX = bludger->position.x;
                        auto oldY = bludger->position.y;
                        gameController::Shot bShot(environment, player, bludger, target);
                        if(bShot.check() == gameController::ActionCheckResult::Impossible){
                            return false;
                        }

                        auto res = bShot.execute();
                        addFouls(res.second, player);
                        for(const auto &result : res.first){
                            switch (result){
                                case gameController::ActionResult::Intercepted:
                                    fatalErrorListener(std::string{"Unexpected action result"});
                                    return false;
                                case gameController::ActionResult::Miss:
                                    fatalErrorListener(std::string{"Unexpected action result"});
                                    return false;
                                case gameController::ActionResult::ScoreRight:
                                    fatalErrorListener(std::string{"Unexpected action result"});
                                    return false;
                                case gameController::ActionResult::ScoreLeft:
                                    fatalErrorListener(std::string{"Unexpected action result"});
                                    return false;
                                case gameController::ActionResult::ThrowSuccess:
                                    fatalErrorListener(std::string{"Unexpected action result"});
                                    return false;
                                case gameController::ActionResult::Knockout:
                                    lastDeltas.emplace(DeltaType::BLUDGER_KNOCKOUT, true, player->position.x, player->position.y,
                                                     bludger->position.x, bludger->position.y, bludger->id, player->id,
                                                     std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);
                                    break;
                                case gameController::ActionResult::SnitchCatch:
                                    fatalErrorListener(std::string{"Unexpected action result"});
                                    return false;
                                case gameController::ActionResult::WrestQuaffel:
                                    fatalErrorListener(std::string{"Unexpected action result"});
                                    return false;
                                case gameController::ActionResult::FoolAway:
                                    lastDeltas.emplace(DeltaType::FOOL_AWAY, std::nullopt, player->position.x, player->position.y,
                                                     environment->quaffle->position.x, environment->quaffle->position.y,
                                                     environment->quaffle->id, player->id, std::nullopt, std::nullopt,
                                                     std::nullopt, std::nullopt, std::nullopt);
                                    break;
                                default:
                                    fatalErrorListener(std::string("Fatal error, enum out of range! Possible memory corruption!"));
                                    return false;
                            }
                        }

                        //Failed Knockout
                        if(res.first.empty() && bludger->position == player->position){
                            lastDeltas.emplace(DeltaType::BLUDGER_KNOCKOUT, false, player->position.x, player->position.y,
                                             std::nullopt, std::nullopt, bludger->id, player->id, std::nullopt, std::nullopt,
                                             std::nullopt, std::nullopt, std::nullopt);
                        }

                        lastDeltas.emplace(DeltaType::BLUDGER_BEATING, std::nullopt, oldX, oldY, bludger->position.x, bludger->position.y,
                                player->id, bludger->id, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);
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
                    if(!conversions::isPlayer(command.getActiveEntity().value())){
                        return false;
                    }

                    try{
                        auto player = environment->getPlayerById(command.getActiveEntity().value());
                        if(!gameController::playerCanShoot(player, environment)){
                            return false;
                        }

                        gameModel::Position target(command.getXPosNew().value(), command.getYPosNew().value());
                        auto oldX = environment->quaffle->position.x;
                        auto oldY = environment->quaffle->position.y;
                        gameController::Shot qThrow(environment, player, environment->quaffle, target);
                        if(qThrow.check() == gameController::ActionCheckResult::Impossible){
                            return false;
                        }

                        auto res = qThrow.execute();
                        auto destPlayer = environment->getPlayer(environment->quaffle->position);
                        std::optional<EntityId > destPlayerId = {};
                        if(destPlayer.has_value()){
                            destPlayerId = destPlayer.value()->id;
                        }

                        addFouls(res.second, player);
                        bool success = true;
                        for(const auto &result : res.first){
                            switch (result){
                                case gameController::ActionResult::Intercepted:
                                    //Not defined in "standard"
                                    success = false;
                                    break;
                                case gameController::ActionResult::Miss:
                                    //Not defined ins "standard"
                                    success = false;
                                    break;
                                case gameController::ActionResult::ScoreRight:
                                    lastDeltas.emplace(DeltaType::GOAL_POINTS_CHANGE, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                                     std::nullopt, std::nullopt, std::nullopt, std::nullopt, environment->team1->score,
                                                     environment->team2->score, std::nullopt, std::nullopt);
                                    break;
                                case gameController::ActionResult::ScoreLeft:
                                    lastDeltas.emplace(DeltaType::GOAL_POINTS_CHANGE, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                                     std::nullopt, std::nullopt, std::nullopt, std::nullopt, environment->team1->score,
                                                     environment->team2->score, std::nullopt, std::nullopt);
                                    break;
                                case gameController::ActionResult::ThrowSuccess:
                                    break;
                                case gameController::ActionResult::Knockout:
                                    fatalErrorListener(std::string{"Unexpected action result"});
                                    return false;
                                case gameController::ActionResult::SnitchCatch:
                                    fatalErrorListener(std::string{"Unexpected action result"});
                                    return false;
                                case gameController::ActionResult::WrestQuaffel:
                                    fatalErrorListener(std::string{"Unexpected action result"});
                                    return false;
                                case gameController::ActionResult::FoolAway:
                                    fatalErrorListener(std::string{"Unexpected action result"});
                                    return false;
                                default:
                                    fatalErrorListener(std::string("Fatal error, enum out of range! Possible memory corruption!"));
                            }
                        }

                        lastDeltas.emplace(DeltaType ::QUAFFLE_THROW, success, oldX, oldY, environment->quaffle->position.x,
                                         environment->quaffle->position.y, player->id, destPlayerId,
                                         std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);

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

                    auto oldX = environment->snitch->position.x;
                    auto oldY = environment->snitch->position.y;
                    if(sPush.execute() == gameController::ActionCheckResult::Foul){
                        lastDeltas.emplace(DeltaType::BAN, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                         std::nullopt, conversions::interferenceToId(gameModel::InterferenceType::SnitchPush, side),
                                         std::nullopt, std::nullopt, std::nullopt, std::nullopt, BanReason::SNITCH_SNATCH);
                    }

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
                try{
                    auto &team = getTeam(side);
                    gameController::Impulse impulse(environment, team);
                    if(!impulse.isPossible()){
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
                        lastDeltas.emplace(DeltaType::BAN, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                         std::nullopt, conversions::interferenceToId(gameModel::InterferenceType::Impulse, side),
                                         std::nullopt, std::nullopt, std::nullopt, std::nullopt,  BanReason::TROLL_ROAR);
                    }

                    auto playerWithQuaffle = environment->getPlayer({oldX, oldY});
                    if(playerWithQuaffle.has_value()){
                        lastDeltas.emplace(DeltaType::FOOL_AWAY, std::nullopt, oldX, oldY, environment->quaffle->position.x,
                                         environment->quaffle->position.y, environment->quaffle->id, playerWithQuaffle.value()->id,
                                         std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);
                    }

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
                if(command.getPassiveEntity().has_value()){
                    if(!conversions::isPlayer(command.getPassiveEntity().value())){
                        return false;
                    }

                    try{
                        auto &team = getTeam(side);
                        auto targetPlayer = environment->getPlayerById(command.getPassiveEntity().value());
                        auto oldX = targetPlayer->position.x;
                        auto oldY = targetPlayer->position.y;
                        gameController::Teleport teleport(environment, team, targetPlayer);
                        if(!teleport.isPossible()){
                            return false;
                        }

                        if(teleport.execute() == gameController::ActionCheckResult::Foul){
                         lastDeltas.emplace(DeltaType::BAN, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                          conversions::interferenceToId(gameModel::InterferenceType::Teleport, side),
                                         std::nullopt, std::nullopt, std::nullopt, std::nullopt, BanReason::ELF_TELEPORTATION);
                        }

                        //@TODO was tun, wenn target den quaffle hält?
                        lastDeltas.emplace(DeltaType::ELF_TELEPORTATION, std::nullopt, oldX, oldY, targetPlayer->position.x, targetPlayer->position.y,
                                         conversions::interferenceToId(gameModel::InterferenceType::Teleport, side), targetPlayer->id,
                                         std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);
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
                            return false;
                        }

                        if(rAttack.execute() == gameController::ActionCheckResult::Foul){
                          lastDeltas.emplace(DeltaType::BAN, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                           conversions::interferenceToId(gameModel::InterferenceType::RangedAttack, side),
                                         std::nullopt, std::nullopt, std::nullopt, std::nullopt, BanReason::GOBLIN_SHOCK);
                        }

                        if(gameModel::Position{oldX, oldY} == gameModel::Position{oldXQuaf, oldYQuaf}){
                            lastDeltas.emplace(DeltaType::FOOL_AWAY, std::nullopt, oldXQuaf, oldYQuaf,
                                         environment->quaffle->position.x, environment->quaffle->position.y,
                                         environment->quaffle->id, targetPlayer->id, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);
                        }

                        lastDeltas.emplace(DeltaType::GOBLIN_SHOCK, std::nullopt, oldX, oldY, targetPlayer->position.x, targetPlayer->position.y,
                                         conversions::interferenceToId(gameModel::InterferenceType::RangedAttack, side),
                                         targetPlayer->id, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);

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
                    if(!conversions::isPlayer(command.getActiveEntity().value())){
                        return false;
                    }

                    try{
                        auto player = environment->getPlayerById(command.getActiveEntity().value());
                        auto oldX = player->position.x;
                        auto oldY = player->position.y;
                        gameModel::Position target(command.getXPosNew().value(), command.getYPosNew().value());
                        gameController::Move move(environment, player, target);
                        if(move.check() == gameController::ActionCheckResult::Impossible){
                            return false;
                        }

                        auto res = move.execute();
                        addFouls(res.second, player);
                        bool snitchCought = false;
                        for(const auto &result : res.first){
                            switch (result){
                                case gameController::ActionResult::Intercepted:
                                    fatalErrorListener(std::string{"Unexpected action result"});
                                    return false;
                                case gameController::ActionResult::Miss:
                                    fatalErrorListener(std::string{"Unexpected action result"});
                                    return false;
                                case gameController::ActionResult::ScoreRight:
                                    lastDeltas.emplace(DeltaType::GOAL_POINTS_CHANGE, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                                     std::nullopt, std::nullopt, std::nullopt, std::nullopt, environment->team1->score,
                                                     environment->team2->score, std::nullopt, std::nullopt);
                                    break;
                                case gameController::ActionResult::ScoreLeft:
                                    lastDeltas.emplace(DeltaType::GOAL_POINTS_CHANGE, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                                     std::nullopt, std::nullopt, std::nullopt, std::nullopt, environment->team1->score,
                                                     environment->team2->score, std::nullopt, std::nullopt);
                                    break;
                                case gameController::ActionResult::ThrowSuccess:
                                    fatalErrorListener(std::string{"Unexpected action result"});
                                    return false;
                                case gameController::ActionResult::Knockout:
                                    fatalErrorListener(std::string{"Unexpected action result"});
                                    return false;
                                case gameController::ActionResult::SnitchCatch:
                                    snitchCought = true;
                                    break;
                                case gameController::ActionResult::WrestQuaffel:
                                    fatalErrorListener(std::string{"Unexpected action result"});
                                    return false;
                                case gameController::ActionResult::FoolAway:
                                    fatalErrorListener(std::string{"Unexpected action result"});
                                    return false;
                                default:
                                    fatalErrorListener(std::string("Fatal error, enum out of range! Possible memory corruption!"));
                                    return false;
                            }
                        }

                        if(environment->snitch->position == player->position){
                            lastDeltas.emplace(DeltaType::SNITCH_CATCH, snitchCought, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                             player->id, std::nullopt, std::nullopt, environment->team1->score, environment->team2->score,
                                             std::nullopt, std::nullopt);
                        }

                        lastDeltas.emplace(DeltaType::MOVE, std::nullopt, oldX, oldY, player->position.x, player->position.y,
                                         player->id, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);

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
                if(command.getActiveEntity().has_value()){
                    lastDeltas.emplace(DeltaType::SKIP, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                     command.getActiveEntity().value(), std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                                     std::nullopt, std::nullopt);
                    return true;
                } else {
                    return false;
                }
            case communication::messages::types::DeltaType::UNBAN:
                if(command.getActiveEntity().has_value() && command.getXPosNew().has_value() &&
                    command.getYPosNew().has_value()){
                    if(!conversions::isPlayer(command.getActiveEntity().value())){
                        return false;
                    }

                    try {
                        auto player = environment->getPlayerById(command.getActiveEntity().value());
                        if(!player->isFined){
                            return false;
                        }

                        gameModel::Position target{command.getXPosNew().value(), command.getYPosNew().value()};
                        if(!environment->cellIsFree(target)){
                            return false;
                        }

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
                    return false;
                }
            case communication::messages::types::DeltaType::WREST_QUAFFLE:
                if(command.getActiveEntity().has_value()){
                    try{
                        auto player = std::dynamic_pointer_cast<gameModel::Chaser>(environment->getPlayerById(command.getActiveEntity().value()));
                        auto targetPlayer = environment->getPlayer(environment->quaffle->position);
                        if(!player || !targetPlayer.has_value()){
                            return false;
                        }

                        gameController::WrestQuaffle wQuaffle(environment, player, environment->quaffle->position);
                        if(wQuaffle.check() == gameController::ActionCheckResult::Impossible){
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
                    return false;
                }
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

            ret.emplace(lastDeltas.front(), roundState, std::vector<std::string>{}, getRound(), teamToTeamSnapshot(environment->team1, TeamSide::LEFT),
                teamToTeamSnapshot(environment->team2, TeamSide::RIGHT), snitchX, snitchY, environment->quaffle->position.x,
                environment->quaffle->position.y, environment->bludgers[0]->position.x, environment->bludgers[0]->position.y,
                environment->bludgers[1]->position.x, environment->bludgers[1]->position.y);
            lastDeltas.pop();
        }

        return ret;
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
                    lastDeltas.emplace(DType::BLUDGER_KNOCKOUT, res.value()->knockedOut, oldX, oldY, bludger->position.x, bludger->position.y,
                                 bludger->id, res.value()->id, roundState, std::nullopt, std::nullopt, std::nullopt, std::nullopt);
                }

                lastDeltas.emplace(DType::MOVE, std::nullopt, oldX, oldY, bludger->position.x, bludger->position.y, bludger->id,
                             std::nullopt, roundState, std::nullopt, std::nullopt, std::nullopt, std::nullopt);
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
            lastDeltas.emplace(DType::MOVE, std::nullopt, oldX, oldY, snitch->position.x, snitch->position.y, snitch->id,
                         std::nullopt, roundState, std::nullopt, std::nullopt, std::nullopt, std::nullopt);
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
