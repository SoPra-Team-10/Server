/**
 * @file Next.cpp
 * @author paul
 * @date 19.03.19
 * @brief Next @TODO
 */

#include "Next.hpp"

#include <utility>

namespace communication::messages::broadcast {

    auto Next::getName() -> std::string {
        return "next";
    }

    Next::Next(types::EntityId entityId, types::TurnType turnType, std::string timout) : entityId(entityId),
                                                                                        turnType(turnType),
                                                                                        timout(std::move(timout)) {}

    types::EntityId Next::getEntityId() const {
        return entityId;
    }

    types::TurnType Next::getTurnType() const {
        return turnType;
    }

    std::string Next::getTimout() const {
        return timout;
    }

    void to_json(nlohmann::json &j, const Next &next) {
        switch (next.getEntityId()) {
            case types::EntityId::LEFT_SEEKER:
                j["turn"] = "leftSeeker";
                break;
            case types::EntityId::LEFT_KEEPER:
                j["turn"] = "leftKeeper";
                break;
            case types::EntityId::LEFT_CHASER1:
                j["turn"] = "leftChaser1";
                break;
            case types::EntityId::LEFT_CHASER2:
                j["turn"] = "leftChaser2";
                break;
            case types::EntityId::LEFT_CHASER3:
                j["turn"] = "leftChase3";
                break;
            case types::EntityId::LEFT_BEATER1:
                j["turn"] = "leftBeater1";
                break;
            case types::EntityId::LEFT_BEATER2:
                j["turn"] = "leftBeater2";
                break;
            case types::EntityId::RIGHT_SEEKER:
                j["turn"] = "rightSeeker";
                break;
            case types::EntityId::RIGHT_KEEPER:
                j["turn"] = "rightKeeper";
                break;
            case types::EntityId::RIGHT_CHASER1:
                j["turn"] = "rightChaser1";
                break;
            case types::EntityId::RIGHT_CHASER2:
                j["turn"] = "rightChaser2";
                break;
            case types::EntityId::RIGHT_CHASER3:
                j["turn"] = "rightChase3";
                break;
            case types::EntityId::RIGHT_BEATER1:
                j["turn"] = "rightBeater1";
                break;
            case types::EntityId::RIGHT_BEATER2:
                j["turn"] = "rightBeater2";
                break;
            case types::EntityId::SNITCH:
                j["turn"] = "snitch";
                break;
            case types::EntityId::BLUDGER1:
                j["turn"] = "bludger1";
                break;
            case types::EntityId::BLUDGER2:
                j["turn"] = "bludger2";
                break;
            case types::EntityId::QUAFFLE:
                j["turn"] = "quaffle";
                break;
            case types::EntityId::LEFT_GOBLIN:
                j["turn"] = "leftGoblin";
                break;
            case types::EntityId::LEFT_TROLL:
                j["turn"] = "leftTroll";
                break;
            case types::EntityId::LEFT_ELF:
                j["turn"] = "leftElf";
                break;
            case types::EntityId::LEFT_NIFFLER:
                j["turn"] = "leftNiffler";
                break;
            case types::EntityId::RIGHT_GOBLIN:
                j["turn"] = "rightGoblin";
                break;
            case types::EntityId::RIGHT_TROLL:
                j["turn"] = "rightTroll";
                break;
            case types::EntityId::RIGHT_ELF:
                j["turn"] = "rightElf";
                break;
            case types::EntityId::RIGHT_NIFFLER:
                j["turn"] = "rightNiffler";
                break;
        }

        switch (next.getTurnType()) {
            case types::TurnType::MOVE:
                j["type"] = "move";
                break;
            case types::TurnType::ACTION:
                j["type"] = "action";
                break;
        }

        j["timeout"] = next.getTimout();
    }

    void from_json(const nlohmann::json &j, Next &next) {
        const auto &entityIdS = j.at("entityID").get<std::string>();
        const auto &turnS = j.at("turnType").get<std::string>();
        types::EntityId entityId;
        types::TurnType turnType;

        if (entityIdS == "leftSeeker") {
            entityId = types::EntityId::LEFT_SEEKER;
        } else if (entityIdS == "leftKeeper") {
            entityId = types::EntityId::LEFT_KEEPER;
        } else if (entityIdS == "leftChaser1") {
            entityId = types::EntityId::LEFT_CHASER1;
        } else if (entityIdS == "leftChaser2") {
            entityId = types::EntityId::LEFT_CHASER2;
        } else if (entityIdS == "leftChaser2") {
            entityId = types::EntityId::LEFT_CHASER2;
        } else if (entityIdS == "leftBeater1") {
            entityId = types::EntityId::LEFT_BEATER1;
        } else if (entityIdS == "leftBeater2") {
            entityId = types::EntityId::LEFT_BEATER2;
        } else if (entityIdS == "rightSeeker") {
            entityId = types::EntityId::RIGHT_SEEKER;
        } else if (entityIdS == "rightKeeper") {
            entityId = types::EntityId::RIGHT_KEEPER;
        } else if (entityIdS == "rightChaser1") {
            entityId = types::EntityId::RIGHT_CHASER1;
        } else if (entityIdS == "rightChaser2") {
            entityId = types::EntityId::RIGHT_CHASER2;
        } else if (entityIdS == "rightChaser2") {
            entityId = types::EntityId::RIGHT_CHASER2;
        } else if (entityIdS == "rightBeater1") {
            entityId = types::EntityId::RIGHT_BEATER1;
        } else if (entityIdS == "rightBeater2") {
            entityId = types::EntityId::RIGHT_BEATER2;
        } else if (entityIdS == "snitch") {
            entityId = types::EntityId::SNITCH;
        } else if (entityIdS == "bludger1") {
            entityId = types::EntityId::BLUDGER1;
        } else if (entityIdS == "bludger2") {
            entityId = types::EntityId::BLUDGER2;
        } else if (entityIdS == "quaffel") {
            entityId = types::EntityId::QUAFFLE;
        } else if (entityIdS == "leftGoblin") {
            entityId = types::EntityId::LEFT_GOBLIN;
        } else if (entityIdS == "leftTroll") {
            entityId = types::EntityId::LEFT_TROLL;
        } else if (entityIdS == "leftElf") {
            entityId = types::EntityId::LEFT_ELF;
        } else if (entityIdS == "leftNiffler") {
            entityId = types::EntityId::LEFT_NIFFLER;
        } else if (entityIdS == "rightGoblin") {
            entityId = types::EntityId::RIGHT_GOBLIN;
        } else if (entityIdS == "rightTroll") {
            entityId = types::EntityId::RIGHT_TROLL;
        } else if (entityIdS == "rightElf") {
            entityId = types::EntityId::RIGHT_ELF;
        } else if (entityIdS == "rightNiffler") {
            entityId = types::EntityId::RIGHT_NIFFLER;
        } else {
            throw std::runtime_error{"Not a valid entity id"};
        }

        if(turnS == "move") {
            turnType = types::TurnType::MOVE;
        } else if (turnS == "action") {
            turnType = types::TurnType::ACTION;
        } else {
            throw std::runtime_error{"Not a valid turnType"};
        }

        next = Next{entityId, turnType, j.at("timeout").get<std::string>()};
    }
}
