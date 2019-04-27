/**
 * @file Delta.hpp
 * @author paul
 * @date 25.04.19
 * @brief Delta @TODO
 */

#ifndef SERVER_DELTA_HPP
#define SERVER_DELTA_HPP

#include <optional>
#include <Lib/json.hpp>

#include "types.hpp"

namespace communication::messages {
    class Delta {
    public:
        Delta() = default;
        Delta(types::DeltaType deltaType, std::optional<bool> success, std::optional<int> xPosOld,
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

    private:
        types::DeltaType deltaType;
        std::optional<bool> success;
        std::optional<int> xPosOld, yPosOld, xPosNew, yPosNew;
        std::optional<types::EntityId> activeEntity, passiveEntity;
    };

    void to_json(nlohmann::json &j, const Delta &delta);
    void from_json(const nlohmann::json &j, Delta &delta);
}

#endif //SERVER_DELTA_HPP
