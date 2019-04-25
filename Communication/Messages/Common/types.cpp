/**
 * @file types.cpp
 * @author paul
 * @date 25.04.19
 * @brief types @TODO
 */

#include <stdexcept>
#include "types.hpp"

auto communication::messages::types::toString(communication::messages::types::EntityId entityId) -> std::string {
    switch (entityId) {
        case types::EntityId::LEFT_SEEKER:
            return "leftSeeker";
        case types::EntityId::LEFT_KEEPER:
            return "leftKeeper";
        case types::EntityId::LEFT_CHASER1:
            return "leftChaser1";
        case types::EntityId::LEFT_CHASER2:
            return "leftChaser2";
        case types::EntityId::LEFT_CHASER3:
            return "leftChase3";
        case types::EntityId::LEFT_BEATER1:
            return "leftBeater1";
        case types::EntityId::LEFT_BEATER2:
            return "leftBeater2";
        case types::EntityId::RIGHT_SEEKER:
            return "rightSeeker";
        case types::EntityId::RIGHT_KEEPER:
            return "rightKeeper";
        case types::EntityId::RIGHT_CHASER1:
            return "rightChaser1";
        case types::EntityId::RIGHT_CHASER2:
            return "rightChaser2";
        case types::EntityId::RIGHT_CHASER3:
            return "rightChase3";
        case types::EntityId::RIGHT_BEATER1:
            return "rightBeater1";
        case types::EntityId::RIGHT_BEATER2:
            return "rightBeater2";
        case types::EntityId::SNITCH:
            return "snitch";
        case types::EntityId::BLUDGER1:
            return "bludger1";
        case types::EntityId::BLUDGER2:
            return "bludger2";
        case types::EntityId::QUAFFLE:
            return "quaffle";
        case types::EntityId::LEFT_GOBLIN:
            return "leftGoblin";
        case types::EntityId::LEFT_TROLL:
            return "leftTroll";
        case types::EntityId::LEFT_ELF:
            return "leftElf";
        case types::EntityId::LEFT_NIFFLER:
            return "leftNiffler";
        case types::EntityId::RIGHT_GOBLIN:
            return "rightGoblin";
        case types::EntityId::RIGHT_TROLL:
            return "rightTroll";
        case types::EntityId::RIGHT_ELF:
            return "rightElf";
        case types::EntityId::RIGHT_NIFFLER:
            return "rightNiffler";
    }
    throw std::runtime_error{"We shouln't really be here"};
}

auto communication::messages::types::fromStringEntityId(const std::string &s) -> communication::messages::types::EntityId {
    if (s == "leftSeeker") {
        return types::EntityId::LEFT_SEEKER;
    } else if (s == "leftKeeper") {
        return types::EntityId::LEFT_KEEPER;
    } else if (s == "leftChaser1") {
        return types::EntityId::LEFT_CHASER1;
    } else if (s == "leftChaser2") {
        return types::EntityId::LEFT_CHASER2;
    } else if (s == "leftChaser2") {
        return types::EntityId::LEFT_CHASER2;
    } else if (s == "leftBeater1") {
        return types::EntityId::LEFT_BEATER1;
    } else if (s == "leftBeater2") {
        return types::EntityId::LEFT_BEATER2;
    } else if (s == "rightSeeker") {
        return types::EntityId::RIGHT_SEEKER;
    } else if (s == "rightKeeper") {
        return types::EntityId::RIGHT_KEEPER;
    } else if (s == "rightChaser1") {
        return types::EntityId::RIGHT_CHASER1;
    } else if (s == "rightChaser2") {
        return types::EntityId::RIGHT_CHASER2;
    } else if (s == "rightChaser2") {
        return types::EntityId::RIGHT_CHASER2;
    } else if (s == "rightBeater1") {
        return types::EntityId::RIGHT_BEATER1;
    } else if (s == "rightBeater2") {
        return types::EntityId::RIGHT_BEATER2;
    } else if (s == "snitch") {
        return types::EntityId::SNITCH;
    } else if (s == "bludger1") {
        return types::EntityId::BLUDGER1;
    } else if (s == "bludger2") {
        return types::EntityId::BLUDGER2;
    } else if (s == "quaffel") {
        return types::EntityId::QUAFFLE;
    } else if (s == "leftGoblin") {
        return types::EntityId::LEFT_GOBLIN;
    } else if (s == "leftTroll") {
        return types::EntityId::LEFT_TROLL;
    } else if (s == "leftElf") {
        return types::EntityId::LEFT_ELF;
    } else if (s == "leftNiffler") {
        return types::EntityId::LEFT_NIFFLER;
    } else if (s == "rightGoblin") {
        return types::EntityId::RIGHT_GOBLIN;
    } else if (s == "rightTroll") {
        return types::EntityId::RIGHT_TROLL;
    } else if (s == "rightElf") {
        return types::EntityId::RIGHT_ELF;
    } else if (s == "rightNiffler") {
        return types::EntityId::RIGHT_NIFFLER;
    } else {
        throw std::runtime_error{"Not a valid entity id"};
    }
}

auto communication::messages::types::toString(communication::messages::types::TurnType turnType) -> std::string {
    switch (turnType) {
        case types::TurnType::MOVE:
            return "move";
        case types::TurnType::ACTION:
            return "action";
    }
    throw std::runtime_error{"We shouln't really be here"};
}

auto communication::messages::types::fromStringTurnType(const std::string &s)
        -> communication::messages::types::TurnType {
    if(s == "move") {
        return types::TurnType::MOVE;
    } else if (s == "action") {
        return types::TurnType::ACTION;
    } else {
        throw std::runtime_error{"Not a valid turnType"};
    }
}

auto communication::messages::types::toString(communication::messages::types::Broom broom) -> std::string {
    switch (broom) {
        case types::Broom::THINDERBLAST:
            return "thinderblast";
        case types::Broom::CLEANSWEEP11:
            return "cleansweep-11";
        case types::Broom::COMET260:
            return "comet-260";
        case types::Broom::NIMBUS2001:
            return "nimbus-2001";
        case types::Broom::FIREBOLT:
            return "firebolt";
    }
    throw std::runtime_error{"We shouln't really be here"};
}

auto communication::messages::types::fromStringBroom(const std::string &s) -> communication::messages::types::Broom {
    if (s == "thinderblast") {
        return types::Broom::THINDERBLAST;
    } else if (s == "cleansweep-11") {
        return types::Broom::CLEANSWEEP11;
    } else if (s == "comet-260") {
        return types::Broom::COMET260;
    } else if (s == "nimbus-2001") {
        return types::Broom::NIMBUS2001;
    } else if (s == "firebolt") {
        return types::Broom::FIREBOLT;
    } else {
        throw std::runtime_error{"Not a valid broom"};
    }
}

auto communication::messages::types::toString(communication::messages::types::Sex s) -> std::string {
    switch (s) {
        case types::Sex::M:
            return "m";
        case types::Sex::F:
            return "f";
    }
    throw std::runtime_error{"We shouln't really be here"};
}

auto communication::messages::types::fromStringSex(const std::string &s) -> communication::messages::types::Sex {
    if (s == "m") {
        return types::Sex::M;
    } else if (s == "f") {
        return types::Sex::F;
    } else {
        throw std::runtime_error{"Not a valid sex"};
    }
}

auto communication::messages::types::toString(communication::messages::types::DeltaType deltaType) -> std::string {
    switch (deltaType) {
        case DeltaType::SNITCH_CATCH:
            return "snitchCatch";
        case DeltaType::BLUDGER_BEATING:
            return "bludgerBeating";
        case DeltaType::QUAFFLE_THROW:
            return "quaffleThrow";
        case DeltaType::SNITCH_SNATCH:
            return "snitchSnatch";
        case DeltaType::TROLL_ROAR:
            return "trollRoar";
        case DeltaType::ELF_TELEPORTATION:
            return "elfTeleportation";
        case DeltaType::GOBLIN_SHOCK:
            return "goblinShock";
        case DeltaType::BAN:
            return "ban";
        case DeltaType::BLUDGER_KNOCKOUT:
            return "bludgerKnockout";
        case DeltaType::MOVE:
            return "move";
    }
    throw std::runtime_error{"We shouln't really be here"};
}

auto communication::messages::types::fromStringDeltaType(const std::string &s)
        -> communication::messages::types::DeltaType {
    if (s == "snitchCatch") {
        return DeltaType::SNITCH_CATCH;
    } else if (s == "bludgerBeating") {
        return DeltaType::BLUDGER_BEATING;
    } else if (s == "quaffleThrow") {
        return DeltaType::QUAFFLE_THROW;
    } else if (s == "snitchSnatch") {
        return DeltaType::SNITCH_SNATCH;
    } else if (s == "trollRoar") {
        return DeltaType::TROLL_ROAR;
    } else if (s == "elfTeleportation") {
        return DeltaType::ELF_TELEPORTATION;
    } else if (s == "goblinShock") {
        return DeltaType::GOBLIN_SHOCK;
    } else if (s == "ban") {
        return DeltaType::BAN;
    } else if (s == "bludgerKnockout") {
        return DeltaType::BLUDGER_KNOCKOUT;
    } else if (s == "move") {
        return DeltaType::MOVE;
    } else {
        throw std::runtime_error{"Not a valid deltaType"};
    }
}

auto communication::messages::types::toString(communication::messages::types::FanType fanType) -> std::string {
    switch (fanType) {
        case FanType::GOBLIN:
            return "goblin";
        case FanType::TROLL:
            return "troll";
        case FanType::ELF:
            return "elf";
        case FanType::NIFFLER:
            return "niffler";
    }
    throw std::runtime_error{"We shouln't really be here"};
}

auto communication::messages::types::fromStringFanType(const std::string &s)
        -> communication::messages::types::FanType {
    if (s == "goblin") {
        return FanType::GOBLIN;
    } else if (s == "troll") {
        return FanType::TROLL;
    } else if (s == "elf") {
        return FanType::ELF;
    } else if (s == "niffler") {
        return FanType::NIFFLER;
    } else {
        throw std::runtime_error{"Not a valid FanType"};
    }
}

auto communication::messages::types::toString(communication::messages::types::PhaseType phaseType) -> std::string {
    switch (phaseType) {
        case PhaseType::BALL_PHASE:
            return "ballPhase";
        case PhaseType::PLAYER_PHASE:
            return "playerPhase";
        case PhaseType::ACTION_PHASE:
            return "actionPhase";
        case PhaseType::FAN_PHASE:
            return "fanPhase";
        case PhaseType::GAME_FINISH:
            return "gameFinish";
    }
    throw std::runtime_error{"We shouln't really be here"};
}

auto communication::messages::types::fromStringPhaseType(const std::string &s)
        -> communication::messages::types::PhaseType {
    if (s == "ballPhase") {
        return PhaseType::BALL_PHASE;
    } else if (s == "playerPhase") {
        return PhaseType::PLAYER_PHASE;
    } else if (s == "actionPhase") {
        return PhaseType::ACTION_PHASE;
    } else if (s == "fanPhase") {
        return PhaseType::FAN_PHASE;
    } else if (s == "gameFinish") {
        return PhaseType::GAME_FINISH;
    } else {
        throw std::runtime_error{"Not a valid PhaseType"};
    }
}

auto communication::messages::types::toString(communication::messages::types::VictoryReason victoryReason)
        -> std::string {
    switch (victoryReason) {
        case VictoryReason::DISQUALIFICATION:
            return "disqualification";
        case VictoryReason::BOTH_DISQUALIFICATION_MOST_POINTS:
            return "bothDisqualificationMostPoints";
        case VictoryReason::BOTH_DISQUALIFICATION_POINTS_EQUAL_SNITCH_CATCH:
            return "bothDisqualificationPointsEqualSnitchCatch";
        case VictoryReason::BOTH_DISQUALIFICATION_POINTS_EQUAL_LAST_DISQUALIFICATION:
            return "bothDisqualificationPointsEqualLastDisqualification";
        case VictoryReason::MOST_POINTS:
            return "mostPoints";
        case VictoryReason::POINTS_EQUAL_SNITCH_CATCH:
            return "pointsEqualSnitchCatch";
        case VictoryReason::VIOLATION_OF_PROTOCOL:
            return "violationOfProtocol";
    }
    throw std::runtime_error{"We shouln't really be here"};
}

auto communication::messages::types::fromStringVictoryReason(
        const std::string &s) -> communication::messages::types::VictoryReason {
    if (s == "disqualification") {
        return VictoryReason::DISQUALIFICATION;
    } else if (s == "bothDisqualificationMostPoints") {
        return VictoryReason::BOTH_DISQUALIFICATION_MOST_POINTS;
    } else if (s == "bothDisqualificationPointsEqualSnitchCatch") {
        return VictoryReason::BOTH_DISQUALIFICATION_POINTS_EQUAL_SNITCH_CATCH;
    } else if (s == "bothDisqualificationPointsEqualLastDisqualification") {
        return VictoryReason::BOTH_DISQUALIFICATION_POINTS_EQUAL_LAST_DISQUALIFICATION;
    } else if (s == "mostPoints") {
        return VictoryReason::MOST_POINTS;
    } else if (s == "pointsEqualSnitchCatch") {
        return VictoryReason::POINTS_EQUAL_SNITCH_CATCH;
    } else if (s == "violationOfProtocol") {
        return VictoryReason::VIOLATION_OF_PROTOCOL;
    } else {
        throw std::runtime_error{"Not a valid VictoryReason"};
    }
}
