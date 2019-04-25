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
