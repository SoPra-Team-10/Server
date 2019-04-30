#include <gtest/gtest.h>
#include <Communication/Messages/Common/Delta.hpp>

using namespace communication::messages;

TEST(CommunicationMessagesCommonDelta, SerializeDeserialize) {
    Delta orig{types::DeltaType::SNITCH_SNATCH, true, 17,38,-12,35,
                types::EntityId::LEFT_NIFFLER, types::EntityId::RIGHT_BEATER1};
    std::string ser;
    nlohmann::json json;
    Delta comp;
    EXPECT_NO_THROW(json = orig);
    EXPECT_NO_THROW(ser = json.dump());
    EXPECT_NO_THROW(comp = nlohmann::json::parse(ser).get<Delta>());
    EXPECT_EQ(comp, orig);
}