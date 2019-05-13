#include <utility>

//
// Created by timluchterhand on 06.05.19.
//

#include "PhaseManager.h"
#include "conversions.h"

namespace gameHandling{
    PhaseManager::PhaseManager(const std::shared_ptr<gameModel::Team> &teamLeft,
                               const std::shared_ptr<gameModel::Team> &teamRight,
                               std::shared_ptr<const gameModel::Environment> env) :
            teamLeft(teamLeft, TeamSide::LEFT), teamRight(teamRight, TeamSide::RIGHT), env(std::move(env)) {
        chooseTeam(currentSidePlayers);
        chooseTeam(currentSideInter);
    }

    auto PhaseManager::nextPlayer() -> communication::messages::broadcast::Next {
        using namespace communication::messages::types;
        if(!hasNextPlayer()){
            throw std::runtime_error("Player phase is over");
        }

        switch (playerTurnState){
            case PlayerTurnState::Move:
                switch (teamState){
                    case TeamState::BothAvailable:{
                        auto &team = getTeam(currentSidePlayers);
                        while ((currentPlayer = team.getNextPlayer())->knockedOut){
                            currentPlayer->knockedOut = false;
                        }

                        if(!team.hasConciousPlayer()){
                            teamState = TeamState::OneEmpty;
                        }

                        switchSide(currentSidePlayers);
                        break;
                    }
                    case TeamState::OneEmpty:{
                        auto &team = getTeam(currentSidePlayers);
                        while ((currentPlayer = team.getNextPlayer())->knockedOut){
                            currentPlayer->knockedOut = false;
                        }

                        if(!team.hasConciousPlayer()){
                            teamState = TeamState::BothEmpty;
                        }

                        break;
                    }
                    case TeamState::BothEmpty:
                        throw std::runtime_error("No more playable players available");
                }

                if(gameController::actionTriggered(env->config.getExtraTurnProb(currentPlayer->broom))){
                    playerTurnState = PlayerTurnState::ExtraMove;
                } else {
                    playerTurnState = PlayerTurnState::PossibleAction;
                }

                return {currentPlayer->id, TurnType::MOVE, env->config.timeouts.playerTurn};
            case PlayerTurnState::ExtraMove:
                playerTurnState = PlayerTurnState::PossibleAction;
                return {currentPlayer->id, TurnType::MOVE, env->config.timeouts.playerTurn};
            case PlayerTurnState::PossibleAction:
                playerTurnState = PlayerTurnState::Move;
                if(gameController::playerCanPerformAction(currentPlayer, env)){
                    return {currentPlayer->id, TurnType::ACTION, env->config.timeouts.playerTurn};
                } else {
                    return nextPlayer();
                }
            default:
                throw std::runtime_error("Enum out of bounds");
        }
    }

    auto PhaseManager::nextInterference() -> communication::messages::broadcast::Next {
        if(!hasNextInterference()){
            throw std::runtime_error("Fan phase is over");
        }

        auto &team = getTeam(currentSideInter);
        std::optional<communication::messages::types::EntityId> nextInter = {};
        if(team.hasInterference()){
            nextInter = team.getNextInterference();
        }

        if(!oneTeamEmptyInters){
            switchSide(currentSideInter);
        }

        if(nextInter.has_value()){
            return {nextInter.value(), communication::messages::types::TurnType::FAN, env->config.timeouts.fanTurn};
        } else if(!oneTeamEmptyInters){
            oneTeamEmptyInters = true;
            return nextInterference();
        }

        throw std::runtime_error("Fatal error, no interference found");
    }

    bool PhaseManager::hasNextPlayer() const{
        return teamState != TeamState::BothEmpty || playerTurnState == PlayerTurnState::ExtraMove ||
            (playerTurnState == PlayerTurnState::PossibleAction && gameController::playerCanPerformAction(currentPlayer, env));
    }

    bool PhaseManager::hasNextInterference() const {
        return teamLeft.hasInterference() || teamRight.hasInterference();
    }

    void PhaseManager::resetPlayers() {
        oneTeamEmptyPlayers = false;
        teamRight.resetPlayers();
        teamLeft.resetPlayers();
        chooseTeam(currentSidePlayers);
        currentTurnFinished = true;
    }

    void PhaseManager::resetInterferences() {
        oneTeamEmptyInters = false;
        teamRight.resetInterferences();
        teamLeft.resetInterferences();
        chooseTeam(currentSideInter);
    }

    void PhaseManager::reset() {
        resetPlayers();
        resetInterferences();
    }


    auto PhaseManager::getTeam(TeamSide side) -> MemberSelector & {
        if(side == TeamSide::LEFT){
            return teamLeft;
        } else {
            return teamRight;
        }
    }

    auto PhaseManager::getTeam(TeamSide side) const -> const MemberSelector & {
        if(side == TeamSide::LEFT){
            return teamLeft;
        } else {
            return teamRight;
        }
    }

    void PhaseManager::chooseTeam(TeamSide &side) const{
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

    int PhaseManager::interferencesUsedLeft(communication::messages::types::FanType type) const {
        return teamLeft.usedInterferences(type);
    }

    int PhaseManager::interferencesUsedRight(communication::messages::types::FanType type) const {
        return teamRight.usedInterferences(type);
    }

    bool PhaseManager::playerUsed(communication::messages::types::EntityId id) const {
        return getTeam(conversions::idToSide(id)).playerUsed(id) && currentPlayer->id != id;
    }

    void PhaseManager::switchSide(TeamSide &side) {
        side = side == TeamSide::LEFT ? TeamSide::RIGHT : TeamSide::LEFT;
    }
}
