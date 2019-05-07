//
// Created by paulnykiel on 07.05.19.
//
#include <stdexcept>
#include "GameTypes.h"

namespace gameHandling {
    auto getSideFromEntity(communication::messages::types::EntityId entityId) -> TeamSide {
        switch (entityId) {
            case communication::messages::types::EntityId::LEFT_SEEKER:
            case communication::messages::types::EntityId::LEFT_KEEPER:
            case communication::messages::types::EntityId::LEFT_CHASER1:
            case communication::messages::types::EntityId::LEFT_CHASER2:
            case communication::messages::types::EntityId::LEFT_CHASER3:
            case communication::messages::types::EntityId::LEFT_BEATER1:
            case communication::messages::types::EntityId::LEFT_BEATER2:
            case communication::messages::types::EntityId::LEFT_GOBLIN:
            case communication::messages::types::EntityId::LEFT_TROLL:
            case communication::messages::types::EntityId::LEFT_ELF:
            case communication::messages::types::EntityId::LEFT_NIFFLER:
                return TeamSide::LEFT;
            case communication::messages::types::EntityId::RIGHT_SEEKER:
            case communication::messages::types::EntityId::RIGHT_KEEPER:
            case communication::messages::types::EntityId::RIGHT_CHASER1:
            case communication::messages::types::EntityId::RIGHT_CHASER2:
            case communication::messages::types::EntityId::RIGHT_CHASER3:
            case communication::messages::types::EntityId::RIGHT_BEATER1:
            case communication::messages::types::EntityId::RIGHT_BEATER2:
            case communication::messages::types::EntityId::RIGHT_GOBLIN:
            case communication::messages::types::EntityId::RIGHT_TROLL:
            case communication::messages::types::EntityId::RIGHT_ELF:
            case communication::messages::types::EntityId::RIGHT_NIFFLER:
                return TeamSide::RIGHT;
            default:
                throw std::runtime_error{"Balls do not have a TeamSide"};
        }
    }
}