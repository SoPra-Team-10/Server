/**
 * @file DeltaRequest.cpp
 * @author paul
 * @date 19.03.19
 * @brief DeltaRequest @TODO
 */

#include "DeltaRequest.hpp"
#include <Util/OptionalSerialization.hpp>

namespace communication::messages::request {

    auto DeltaRequest::getName() -> std::string {
        return "deltaRequest";
    }

    DeltaRequest::DeltaRequest(types::DeltaType deltaType,
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

    types::DeltaType DeltaRequest::getDeltaType() const {
        return deltaType;
    }

    std::optional<bool> DeltaRequest::isSuccess() const {
        return success;
    }

    std::optional<int> DeltaRequest::getXPosOld() const {
        return xPosOld;
    }

    std::optional<int> DeltaRequest::getYPosOld() const {
        return yPosOld;
    }

    std::optional<int> DeltaRequest::getXPosNew() const {
        return xPosNew;
    }

    std::optional<int> DeltaRequest::getYPosNew() const {
        return yPosNew;
    }

    std::optional<types::EntityId> DeltaRequest::getActiveEntity() const {
        return activeEntity;
    }

    std::optional<types::EntityId> DeltaRequest::getPassiveEntity() const {
        return passiveEntity;
    }

    void to_json(nlohmann::json &j, const DeltaRequest &deltaRequest) {
        j["deltaType"] = types::toString(deltaRequest.getDeltaType());
        j["success"] = deltaRequest.isSuccess();
        j["xPosOld"] = deltaRequest.getXPosOld();
        j["yPosOld"] = deltaRequest.getYPosOld();
        j["xPosNew"] = deltaRequest.getXPosNew();
        j["yPosNew"] = deltaRequest.getYPosNew();
        if (deltaRequest.getActiveEntity().has_value()) {
            j["activeEntity"] = types::toString(deltaRequest.getActiveEntity().value());
        } else {
            j["activeEntity"] = nullptr;
        }
        if (deltaRequest.getPassiveEntity().has_value()) {
            j["passiveEntity"] = types::toString(deltaRequest.getPassiveEntity().value());
        } else {
            j["passiveEntity"] = nullptr;
        }
    }

    void from_json(const nlohmann::json &j, DeltaRequest &deltaRequest) {
        std::optional<types::EntityId> active = std::nullopt, passive = std::nullopt;
        if (!j.at("activeEntity").is_null()) {
            active = types::fromStringEntityId(j.at("activeEntity").get<std::string>());
        }
        if (!j.at("passiveEntity").is_null()) {
            passive = types::fromStringEntityId(j.at("passiveEntity").get<std::string>());
        }

        deltaRequest = DeltaRequest{
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
