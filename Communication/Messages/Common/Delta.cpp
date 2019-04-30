/**
 * @file Delta.cpp
 * @author paul
 * @date 25.04.19
 * @brief Definition of the Delta class
 */

#include "Delta.hpp"
#include <Util/OptionalSerialization.hpp>

namespace communication::messages {
    Delta::Delta(types::DeltaType deltaType,
                 std::optional<bool> success,
                 std::optional<int> xPosOld,
                 std::optional<int> yPosOld,
                 std::optional<int> xPosNew,
                 std::optional<int> yPosNew,
                 std::optional<types::EntityId> activeEntity,
                 std::optional<types::EntityId> passiveEntity) :
            deltaType(deltaType), success(success), xPosOld(xPosOld),
            yPosOld(yPosOld), xPosNew(xPosNew), yPosNew(yPosNew),
            activeEntity(activeEntity),
            passiveEntity(passiveEntity) {}

    types::DeltaType Delta::getDeltaType() const {
        return deltaType;
    }

    std::optional<bool> Delta::isSuccess() const {
        return success;
    }

    std::optional<int> Delta::getXPosOld() const {
        return xPosOld;
    }

    std::optional<int> Delta::getYPosOld() const {
        return yPosOld;
    }

    std::optional<int> Delta::getXPosNew() const {
        return xPosNew;
    }

    std::optional<int> Delta::getYPosNew() const {
        return yPosNew;
    }

    std::optional<types::EntityId> Delta::getActiveEntity() const {
        return activeEntity;
    }

    std::optional<types::EntityId> Delta::getPassiveEntity() const {
        return passiveEntity;
    }

    bool Delta::operator==(const Delta &rhs) const {
        return deltaType == rhs.deltaType &&
               success == rhs.success &&
               xPosOld == rhs.xPosOld &&
               yPosOld == rhs.yPosOld &&
               xPosNew == rhs.xPosNew &&
               yPosNew == rhs.yPosNew &&
               activeEntity == rhs.activeEntity &&
               passiveEntity == rhs.passiveEntity;
    }

    bool Delta::operator!=(const Delta &rhs) const {
        return !(rhs == *this);
    }

    void to_json(nlohmann::json &j, const Delta &delta) {
        j["deltaType"] = types::toString(delta.getDeltaType());
        j["success"] = delta.isSuccess();
        j["xPosOld"] = delta.getXPosOld();
        j["yPosOld"] = delta.getYPosOld();
        j["xPosNew"] = delta.getXPosNew();
        j["yPosNew"] = delta.getYPosNew();

        nlohmann::json activeEntity, passiveEntity;
        if (delta.getActiveEntity().has_value()) {
            j["activeEntity"] = types::toString(delta.getActiveEntity().value());
        } else {
            j["activeEntity"] = nullptr;
        }
        if (delta.getPassiveEntity().has_value()) {
            j["passiveEntity"] = types::toString(delta.getPassiveEntity().value());
        } else {
            j["passiveEntity"] = nullptr;
        }
    }

    void from_json(const nlohmann::json &j, Delta &delta) {
        std::optional<types::EntityId> active = std::nullopt, passive = std::nullopt;
        if (!j.at("activeEntity").is_null()) {
            active = types::fromStringEntityId(j.at("activeEntity").get<std::string>());
        }
        if (!j.at("passiveEntity").is_null()) {
            passive = types::fromStringEntityId(j.at("passiveEntity").get<std::string>());
        }

        delta = Delta{
                types::fromStringDeltaType(j.at("deltaType").get<std::string>()),
                j.at("success").get<std::optional<bool>>(),
                j.at("xPosOld").get<std::optional<int>>(),
                j.at("yPosOld").get<std::optional<int>>(),
                j.at("xPosNew").get<std::optional<int>>(),
                j.at("yPosNew").get<std::optional<int>>(),
                active, passive
        };
    }
}