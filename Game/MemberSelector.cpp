//
// Created by timluchterhand on 06.05.19.
//

#include "MemberSelector.h"
#include <SopraGameLogic/GameController.h>
namespace gameHandling{
    MemberSelector::MemberSelector(const std::shared_ptr<gameModel::Team> &team, TeamSide side) : team(team), fanblock(team->fanblock), side(side){
        resetPlayers();
        resteInterferences();
    }

    auto MemberSelector::getNextPlayer() -> std::shared_ptr<gameModel::Player> {
        if(!hasPlayers()){
            throw std::runtime_error("No more players left to select");
        }

        auto pos = selectRandom<std::shared_ptr<gameModel::Player>>(playersLeft);
        auto ret = *pos;
        playersLeft.erase(pos);
        return ret;
    }

    auto MemberSelector::getNextInterference() -> communication::messages::types::EntityId {
        if(!hasInterference()){
            throw std::runtime_error("No more interferences left to select");
        }

        auto pos = selectRandom<std::pair<gameModel::InterferenceType, int>>(interferencesLeft);
        auto ret = pos->first;
        if(--pos->second <= 0){
            interferencesLeft.erase(pos);
        }

        return interferenceToID(ret);
    }

    auto MemberSelector::interferenceToID(gameModel::InterferenceType type) const -> communication::messages::types::EntityId {
        using Id = communication::messages::types::EntityId;
        switch(type){
            case gameModel::InterferenceType::RangedAttack:
                return side == TeamSide::LEFT ? Id::LEFT_GOBLIN : Id::RIGHT_GOBLIN;
            case gameModel::InterferenceType::Teleport:
                return side == TeamSide::LEFT ? Id::LEFT_ELF : Id::RIGHT_ELF;
            case gameModel::InterferenceType::Impulse:
                return side == TeamSide::LEFT ? Id::LEFT_TROLL : Id::RIGHT_TROLL;
            case gameModel::InterferenceType::SnitchPush:
                return side == TeamSide::LEFT ? Id::LEFT_NIFFLER : Id::RIGHT_NIFFLER;
        }

        throw std::runtime_error("Fatal error! Enum out of bounds");
    }

    bool MemberSelector::hasPlayers() const {
        return !playersLeft.empty();
    }

    bool MemberSelector::hasInterference() const {
        return !interferencesLeft.empty();
    }

    void MemberSelector::resetPlayers() {
        playersLeft.clear();
        for(const auto &player : team->getAllPlayers()){
            playersLeft.emplace_back(player);
        }
    }

    void MemberSelector::resteInterferences() {
        using Id = gameModel::InterferenceType;
        interferencesLeft = {{Id::Teleport, 0}, {Id::RangedAttack, 0}, {Id::Impulse, 0}, {Id::SnitchPush, 0}};
        for(auto it = interferencesLeft.begin(); it < interferencesLeft.end();){
            int uses = fanblock.getUses(it->first);
            if(uses == 0){
                it = interferencesLeft.erase(it);
            } else {
                it->second = uses;
                it++;
            }
        }
    }

    bool MemberSelector::hasConciousPlayer() const {
        bool found = false;
        for(const auto &player : playersLeft){
            if(!player->knockedOut){
                found = true;
                break;
            }
        }

        return found;
    }
}