//
// Created by timluchterhand on 07.05.19.
//

#include "conversions.h"

auto conversions::foulToBanReason(gameModel::Foul foul) -> communication::messages::types::BanReason {
    using Reason = communication::messages::types::BanReason;
    switch (foul){
        case gameModel::Foul::None:
            throw std::runtime_error("cannot convert");
        case gameModel::Foul::BlockGoal:
            return Reason::FLACKING;
        case gameModel::Foul::ChargeGoal:
            return Reason::HAVERSACKING;
        case gameModel::Foul::MultipleOffence:
            return Reason::STOOGING;
        case gameModel::Foul::Ramming:
            return Reason::BLATCHING;
        case gameModel::Foul::BlockSnitch:
            return Reason::SNITCHNIP;
    }
    throw std::runtime_error("Fatal error! Enum out of bounds");
}

auto conversions::interferenceToId(gameModel::InterferenceType type,
                                   gameHandling::TeamSide side) -> communication::messages::types::EntityId {
            using Id = communication::messages::types::EntityId;
    switch(type){
        case gameModel::InterferenceType::RangedAttack:
            return side == gameHandling::TeamSide::LEFT ? Id::LEFT_GOBLIN : Id::RIGHT_GOBLIN;
        case gameModel::InterferenceType::Teleport:
            return side == gameHandling::TeamSide::LEFT ? Id::LEFT_ELF : Id::RIGHT_ELF;
        case gameModel::InterferenceType::Impulse:
            return side == gameHandling::TeamSide::LEFT ? Id::LEFT_TROLL : Id::RIGHT_TROLL;
        case gameModel::InterferenceType::SnitchPush:
            return side == gameHandling::TeamSide::LEFT ? Id::LEFT_NIFFLER : Id::RIGHT_NIFFLER;
    }

    throw std::runtime_error("Fatal error! Enum out of bounds");
}

bool conversions::isFan(communication::messages::types::EntityId id) {
    switch (id){
        case communication::messages::types::EntityId::LEFT_GOBLIN:
        case communication::messages::types::EntityId::LEFT_TROLL:
        case communication::messages::types::EntityId::LEFT_ELF:
        case communication::messages::types::EntityId::LEFT_NIFFLER:
        case communication::messages::types::EntityId::RIGHT_GOBLIN:
        case communication::messages::types::EntityId::RIGHT_TROLL:
        case communication::messages::types::EntityId::RIGHT_ELF:
        case communication::messages::types::EntityId::RIGHT_NIFFLER:
            return true;
        default:
            return false;
    }

    throw std::runtime_error("Fatal error! Enum out of bounds");
}

bool conversions::isBall(communication::messages::types::EntityId id) {
    switch (id){

        case communication::messages::types::EntityId::SNITCH:
        case communication::messages::types::EntityId::BLUDGER1:
        case communication::messages::types::EntityId::BLUDGER2:
        case communication::messages::types::EntityId::QUAFFLE:
            return true;
        default:
            return false;
    }

    throw std::runtime_error("Fatal error! Enum out of bounds");
}

bool conversions::isPlayer(communication::messages::types::EntityId id) {
    switch (id){

        case communication::messages::types::EntityId::LEFT_SEEKER:
        case communication::messages::types::EntityId::LEFT_KEEPER:
        case communication::messages::types::EntityId::LEFT_CHASER1:
        case communication::messages::types::EntityId::LEFT_CHASER2:
        case communication::messages::types::EntityId::LEFT_CHASER3:
        case communication::messages::types::EntityId::LEFT_BEATER1:
        case communication::messages::types::EntityId::LEFT_BEATER2:
        case communication::messages::types::EntityId::RIGHT_SEEKER:
        case communication::messages::types::EntityId::RIGHT_KEEPER:
        case communication::messages::types::EntityId::RIGHT_CHASER1:
        case communication::messages::types::EntityId::RIGHT_CHASER2:
        case communication::messages::types::EntityId::RIGHT_CHASER3:
        case communication::messages::types::EntityId::RIGHT_BEATER1:
        case communication::messages::types::EntityId::RIGHT_BEATER2:
            return true;
        default:
            return false;
    }

    throw std::runtime_error("Fatal error! Enum out of bounds");
}

auto conversions::idToSide(communication::messages::types::EntityId id) -> gameHandling::TeamSide {
    using namespace gameHandling;
    switch (id){
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
        case communication::messages::types::EntityId::RIGHT_GOBLIN:
        case communication::messages::types::EntityId::RIGHT_TROLL:
        case communication::messages::types::EntityId::RIGHT_ELF:
        case communication::messages::types::EntityId::RIGHT_NIFFLER:
        case communication::messages::types::EntityId::RIGHT_SEEKER:
        case communication::messages::types::EntityId::RIGHT_KEEPER:
        case communication::messages::types::EntityId::RIGHT_CHASER1:
        case communication::messages::types::EntityId::RIGHT_CHASER2:
        case communication::messages::types::EntityId::RIGHT_CHASER3:
        case communication::messages::types::EntityId::RIGHT_BEATER1:
        case communication::messages::types::EntityId::RIGHT_BEATER2:
            return TeamSide::RIGHT;
        default:
            throw std::runtime_error("No player or Fan");
    }

    throw std::runtime_error("Fatal error! Enum out of bounds");
}

auto conversions::idToFantype(communication::messages::types::EntityId id) -> communication::messages::types::FanType {
    using namespace communication::messages::types;
    switch (id){

        case communication::messages::types::EntityId::LEFT_GOBLIN:
            return FanType::GOBLIN;
        case communication::messages::types::EntityId::LEFT_TROLL:
            return FanType::TROLL;
        case communication::messages::types::EntityId::LEFT_ELF:
            return FanType::ELF;
        case communication::messages::types::EntityId::LEFT_NIFFLER:
            return FanType::NIFFLER;
        case communication::messages::types::EntityId::RIGHT_GOBLIN:
            return FanType::GOBLIN;
        case communication::messages::types::EntityId::RIGHT_TROLL:
            return FanType::TROLL;
        case communication::messages::types::EntityId::RIGHT_ELF:
            return FanType::ELF;
        case communication::messages::types::EntityId::RIGHT_NIFFLER:
            return FanType::NIFFLER;
        default:
            throw std::runtime_error("No Fan");
    }

    throw std::runtime_error("Fatal error! Enum out of bounds");
}

