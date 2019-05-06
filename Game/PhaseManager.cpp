//
// Created by timluchterhand on 06.05.19.
//

#include "PhaseManager.h"
namespace gameHandling{
    PhaseManager::PhaseManager(const std::shared_ptr<gameModel::Team> &teamLeft, const std::shared_ptr<gameModel::Team> &teamRight) :
            teamLeft(teamLeft, TeamSide::LEFT), teamRight(teamRight, TeamSide::RIGHT){
        chooseTeam(currentSidePlayers);
        chooseTeam(currentSideInter);
    }

    auto PhaseManager::nextPlayer(const std::shared_ptr<const gameModel::Environment> &env) -> communication::messages::broadcast::Next {
        if(!hasNextPlayer()){
            throw std::runtime_error("Player phase is over");
        }

        auto nextPlayer = getNextPlayer().value();
        return {nextPlayer->id, getNextPlayerAction(nextPlayer, env), env->config.timeouts.playerTurn};
    }

    auto PhaseManager::nextInterference(const std::shared_ptr<const gameModel::Environment> &env) -> communication::messages::broadcast::Next {
        if(!hasNextInterference()){
            throw std::runtime_error("Fan phase is over");
        }

        auto &team = getTeam(currentSideInter);
        std::optional<communication::messages::types::EntityId> nextInter = {};
        if(team.hasInterference()){
            nextInter = team.getNextInterference();
        }

        if(!oneTeamEmptyInters){
            currentSideInter = currentSideInter == TeamSide::LEFT ? TeamSide::RIGHT : TeamSide::LEFT;
        }

        if(nextInter.has_value()){
            return {nextInter.value(), communication::messages::types::TurnType::FAN, env->config.timeouts.fanTurn};
        } else if(!oneTeamEmptyInters){
            oneTeamEmptyInters = true;
            return nextInterference(env);
        }

        throw std::runtime_error("Fatal error, no interference found");
    }

    bool PhaseManager::hasNextPlayer() const{
        return teamLeft.hasConciousPlayer() && teamRight.hasConciousPlayer();
    }

    bool PhaseManager::hasNextInterference() const {
        return teamLeft.hasInterference() && teamRight.hasInterference();
    }

    void PhaseManager::resetPlayers() {
        oneTeamEmptyPlayers = false;
        teamRight.resetPlayers();
        teamLeft.resetPlayers();
        chooseTeam(currentSidePlayers);
    }

    void PhaseManager::resetInterferences() {
        oneTeamEmptyInters = false;
        teamRight.resteInterferences();
        teamLeft.resteInterferences();
        chooseTeam(currentSideInter);
    }

    void PhaseManager::reset() {
        resetPlayers();
        resetInterferences();
    }

    auto PhaseManager::getNextPlayerAction(const std::shared_ptr<const gameModel::Player> &player,
                                          const std::shared_ptr<const gameModel::Environment> &env) const -> communication::messages::types::TurnType {
        using TurnType = communication::messages::types::TurnType;
        static TurnType turnState = TurnType::MOVE;
        static bool extraMove = false;

        if(turnState == TurnType::MOVE && !extraMove){
            if(gameController::actionTriggered(env->config.getExtraTurnProb(player->broom))){
                extraMove = true;
            } else {
                turnState = TurnType::ACTION;
            }

            return TurnType::MOVE;
        } else if(turnState == TurnType::MOVE){
            extraMove = false;
            turnState = TurnType::ACTION;
            return TurnType::MOVE;
        } else if(turnState == TurnType::ACTION){
            turnState = TurnType::MOVE;
            return TurnType::ACTION;
        }

        throw std::runtime_error("Fatal error, possible memory corruption");
    }

    auto PhaseManager::getTeam(TeamSide side) -> MemberSelector & {
        if(side == TeamSide::LEFT){
            return teamLeft;
        } else {
            return teamRight;
        }
    }

    void PhaseManager::chooseTeam(TeamSide &side) {
        if(gameController::rng(0, 1)){
            side = TeamSide::LEFT;
        } else {
            side = TeamSide::RIGHT;
        }
    }

    auto PhaseManager::getNextPlayer() -> std::optional<std::shared_ptr<gameModel::Player>> {
        if(!hasNextPlayer()){
            throw std::runtime_error("Player phase over");
        }

        auto &team = getTeam(currentSidePlayers);
        std::optional<std::shared_ptr<gameModel::Player>> player{};
        while(team.hasPlayers()){
            player = team.getNextPlayer();
            if(player.value()->knockedOut){
                player.value()->knockedOut = false;
                player.reset();
            } else {
                break;
            }
        }

        if(!oneTeamEmptyPlayers){
            currentSidePlayers = currentSidePlayers == TeamSide::LEFT ? TeamSide::RIGHT : TeamSide::LEFT;
        }

        if(player.has_value()){
            return player.value();
        } else if(!oneTeamEmptyPlayers){
            oneTeamEmptyPlayers = true;
            return getNextPlayer();
        }

        return {};
    }
}
