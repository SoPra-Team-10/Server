//
// Created by timluchterhand on 06.05.19.
//

#include "PlayerPhase.h"
namespace gameHandling{
    PlayerPhase::PlayerPhase(const std::shared_ptr<gameModel::Team> &teamLeft,
                                           const std::shared_ptr<gameModel::Team> &teamRight) :
            teamLeft(teamLeft, TeamSide::LEFT), teamRight(teamRight, TeamSide::RIGHT){
        chooseTeam();
    }

    auto PlayerPhase::getNext(const std::shared_ptr<const gameModel::Environment> &env) -> communication::messages::broadcast::Next {
        if(!hasNext()){
            throw std::runtime_error("Player phase is over");
        }

        auto nextPlayer = getNextPlayer().value();
        return {nextPlayer->id, getNextPlayerAction(nextPlayer, env), env->config.timeouts.playerTurn};
    }

    bool PlayerPhase::hasNext() const{
        return teamLeft.hasConciousPlayer() && teamRight.hasConciousPlayer();
    }

    void PlayerPhase::reset() {
        oneTeamEmpty = false;
        teamRight.resetPlayers();
        teamLeft.resetPlayers();
        chooseTeam();
    }

    auto PlayerPhase::getNextPlayerAction(const std::shared_ptr<const gameModel::Player> &player,
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

    auto PlayerPhase::getTeam(TeamSide side) -> MemberSelector & {
        if(side == TeamSide::LEFT){
            return teamLeft;
        } else {
            return teamRight;
        }
    }

    void PlayerPhase::chooseTeam() {
        if(gameController::rng(0, 1)){
            currentSide = TeamSide::LEFT;
        } else {
            currentSide = TeamSide::RIGHT;
        }
    }

    auto PlayerPhase::getNextPlayer() -> std::optional<std::shared_ptr<gameModel::Player>> {
        if(!hasNext()){
            throw std::runtime_error("Player phase over");
        }

        auto team = getTeam(currentSide);
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

        if(!oneTeamEmpty){
            currentSide = currentSide == TeamSide::LEFT ? TeamSide::RIGHT : TeamSide::LEFT;
        }

        if(player.has_value()){
            return player.value();
        } else if(!oneTeamEmpty){
            oneTeamEmpty = true;
            return getNextPlayer();
        }

        return {};
    }
}
