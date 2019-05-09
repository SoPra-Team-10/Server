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

