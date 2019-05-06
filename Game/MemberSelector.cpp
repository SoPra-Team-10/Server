//
// Created by timluchterhand on 06.05.19.
//

#include "MemberSelector.h"
#include <SopraGameLogic/GameController.h>
namespace gameHandling{
    auto MemberSelector::getNextPlayer() -> communication::messages::types::EntityId {
        if(playersLeft.empty()){
            throw std::runtime_error("No more players left to select");
        }
        auto pos = playersLeft.begin() + gameController::rng(0, static_cast<int>(playersLeft.size()) - 1);
        auto id = *pos;
        playersLeft.erase(pos);
        return id;
    }

    auto MemberSelector::getNextInterference() -> communication::messages::types::EntityId {
        if(interferencesLeft.empty()){
            throw std::runtime_error("No more interferences left to select");
        }
        auto pos = interferencesLeft.begin() + gameController::rng(0, static_cast<int>(interferencesLeft.size()) - 1);
        auto id = *pos;
        interferencesLeft.erase(pos);
        return interferenceToID(id);
    }

    MemberSelector::MemberSelector(const std::shared_ptr<gameModel::Team> &team, TeamSide side) : side(side) {
        using Id = gameModel::InterferenceType;
        for(const auto &player : team->getAllPlayers()){
            playersLeft.emplace_back(player->id);
        }

        interferencesLeft = {Id::Teleport, Id::RangedAttack, Id::Impulse, Id::SnitchPush};
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
}
