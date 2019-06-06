//
// Created by timluchterhand on 06.05.19.
//

#include "PhaseManager.h"
#include "conversions.h"

namespace gameHandling{
    PhaseManager::PhaseManager(const std::shared_ptr<gameModel::Team> &teamLeft,
             const std::shared_ptr<gameModel::Team> &teamRight, std::shared_ptr<const gameModel::Environment> env,
             Timeouts timeouts) : teamLeft(teamLeft, TeamSide::LEFT), teamRight(teamRight, TeamSide::RIGHT), env(std::move(env)),
             timeouts(timeouts){
        chooseSide(currentSidePlayers);
        chooseSide(currentSideInter);
    }

    auto PhaseManager::nextPlayer() -> std::optional<communication::messages::broadcast::Next> {
        using namespace communication::messages;
        bool noTurnAllowed = false;
        if(currentPlayer.has_value() && currentPlayer.value()->isFined){
            playerTurnState = PlayerTurnState::Move;
            currentPlayer.reset();
            return nextPlayer();
        }

        switch (playerTurnState){
            case PlayerTurnState::Move:
                switch (teamStatePlayers){
                    case TeamState::BothAvailable:{
                        auto &team = getTeam(currentSidePlayers);
                        currentPlayer = team.getNextPlayer();
                        if(currentPlayer.value()->knockedOut || currentPlayer.value()->isFined){
                            currentPlayer.value()->knockedOut = false;
                            noTurnAllowed = true;
                        }

                        if(!team.hasPlayers()){
                            teamStatePlayers = TeamState::OneEmpty;
                            switchSide(currentSidePlayers);
                            break;
                        }

                        if(!currentPlayer.value()->isFined){
                            switchSide(currentSidePlayers);
                        }

                        break;
                    }

                    case TeamState::OneEmpty:{
                        auto &team = getTeam(currentSidePlayers);
                        currentPlayer = team.getNextPlayer();
                        if(currentPlayer.value()->knockedOut || currentPlayer.value()->isFined){
                            currentPlayer.value()->knockedOut = false;
                            noTurnAllowed = true;
                        }

                        if(!team.hasPlayers()){
                            teamStatePlayers = TeamState::BothEmpty;
                        }

                        break;
                    }

                    case TeamState::BothEmpty:
                        return {};
                }

                if(noTurnAllowed){
                    return nextPlayer();
                }

                if(gameController::actionTriggered(env->config.getExtraTurnProb(currentPlayer.value()->broom))){
                    playerTurnState = PlayerTurnState::ExtraMove;
                } else {
                    playerTurnState = PlayerTurnState::PossibleAction;
                }

                return broadcast::Next{currentPlayer.value()->id, types::TurnType::MOVE, timeouts.playerTurn};
            case PlayerTurnState::ExtraMove:
                playerTurnState = PlayerTurnState::PossibleAction;
                return broadcast::Next{currentPlayer.value()->id, types::TurnType::MOVE, timeouts.playerTurn};
            case PlayerTurnState::PossibleAction:
                playerTurnState = PlayerTurnState::Move;
                if(gameController::playerCanPerformAction(currentPlayer.value(), env)){
                    auto id = currentPlayer.value()->id;
                    currentPlayer.reset();
                    return broadcast::Next{id, types::TurnType::ACTION, timeouts.playerTurn};
                } else {
                    return nextPlayer();
                }

            default:
                throw std::runtime_error("Enum out of bounds");
        }
    }

    auto PhaseManager::nextInterference() -> std::optional<communication::messages::broadcast::Next> {
        using namespace communication::messages;
        types::EntityId nextInter;
        switch (teamStateInterferences){
            case TeamState::BothAvailable:{
                auto &team = getTeam(currentSideInter);
                nextInter = team.getNextInterference();
                if(!team.hasInterference()){
                    teamStateInterferences = TeamState::OneEmpty;
                }

                switchSide(currentSideInter);
                break;
            }

            case TeamState::OneEmpty:{
                auto &team = getTeam(currentSideInter);
                nextInter = team.getNextInterference();
                if(!team.hasInterference()){
                    teamStateInterferences = TeamState::BothEmpty;
                }

                break;
            }

            case TeamState::BothEmpty:
                return {};
        }

        return broadcast::Next{nextInter, types::TurnType::FAN, timeouts.fanTurn};
    }

    void PhaseManager::resetPlayers() {
        teamRight.resetPlayers();
        teamLeft.resetPlayers();
        chooseSide(currentSidePlayers);
        teamStatePlayers = TeamState::BothAvailable;
    }

    void PhaseManager::resetInterferences() {
        teamRight.resetInterferences();
        teamLeft.resetInterferences();
        if(teamRight.hasInterference() && teamLeft.hasInterference()){
            chooseSide(currentSideInter);
            teamStateInterferences = TeamState::BothAvailable;
        } else if(teamLeft.hasInterference()){
            currentSideInter = TeamSide::LEFT;
            teamStateInterferences = TeamState::OneEmpty;
        } else if(teamRight.hasInterference()){
            currentSideInter = TeamSide::RIGHT;
            teamStateInterferences = TeamState::OneEmpty;
        }else {
            teamStateInterferences = TeamState::BothEmpty;
        }
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

    void PhaseManager::chooseSide(TeamSide &side) const{
        if(gameController::rng(0, 1)){
            side = TeamSide::LEFT;
        } else {
            side = TeamSide::RIGHT;
        }
    }

    int PhaseManager::interferencesUsedLeft(communication::messages::types::FanType type) const {
        return teamLeft.usedInterferences(type);
    }

    int PhaseManager::interferencesUsedRight(communication::messages::types::FanType type) const {
        return teamRight.usedInterferences(type);
    }

    void PhaseManager::switchSide(TeamSide &side) {
        side = side == TeamSide::LEFT ? TeamSide::RIGHT : TeamSide::LEFT;
    }

    bool PhaseManager::playerUsed(const std::shared_ptr<const gameModel::Player> &player) const {
        auto &team = getTeam(conversions::idToSide(player->id));
        return team.playerUsed(player->id) && (currentPlayer != player || (playerTurnState == PlayerTurnState::PossibleAction &&
            !gameController::playerCanPerformAction(player, env)));
    }

    bool PhaseManager::hasInterference() const{
        return teamLeft.hasInterference() || teamRight.hasInterference();
    }
}
