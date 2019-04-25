/**
 * @file DeltaRequest.hpp
 * @author paul
 * @date 19.03.19
 * @brief DeltaRequest @TODO
 */

#ifndef SERVER_DELTAREQUEST_HPP
#define SERVER_DELTAREQUEST_HPP

#include <string>
#include <Lib/json.hpp>
#include <Communication/Messages/types.hpp>

namespace communication::messages::request {
    class DeltaRequest {
    public:
        DeltaRequest(types::DeltaType deltaType, std::optional<bool> success, std::optional<int> xPosOld,
                     std::optional<int> yPosOld, std::optional<int> xPosNew, std::optional<int> yPosNew,
                     std::optional<types::EntityId> activeEntity, std::optional<types::EntityId> passiveEntity);

        types::DeltaType getDeltaType() const;
        std::optional<bool> isSuccess() const;
        std::optional<int> getXPosOld() const;
        std::optional<int> getYPosOld() const;
        std::optional<int> getXPosNew() const;
        std::optional<int> getYPosNew() const;
        std::optional<types::EntityId> getActiveEntity() const;
        std::optional<types::EntityId> getPassiveEntity() const;

        static auto getName() -> std::string;
    private:
        types::DeltaType deltaType;
        std::optional<bool> success;
        std::optional<int> xPosOld, yPosOld, xPosNew, yPosNew;
        std::optional<types::EntityId> activeEntity, passiveEntity;
    };

    void to_json(nlohmann::json &j, const DeltaRequest &deltaRequest);
    void from_json(const nlohmann::json &j, DeltaRequest &deltaRequest);
}

#endif //SERVER_DELTAREQUEST_HPP
