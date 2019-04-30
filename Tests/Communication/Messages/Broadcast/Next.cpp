#include <gtest/gtest.h>
#include <Communication/Messages/Broadcast/Next.hpp>

using namespace communication::messages;

using T = broadcast::Next;

TEST(CommunicationMessagesBroadcastNext, SerializeDeserialize) {
    T orig{types::EntityId::RIGHT_GOBLIN, types::TurnType::MOVE, 37};
    std::string ser;
    nlohmann::json json;
    T comp;
    EXPECT_NO_THROW(json = orig);
    EXPECT_NO_THROW(ser = json.dump());
    EXPECT_NO_THROW(comp = nlohmann::json::parse(ser).get<T>());
    EXPECT_EQ(comp, orig);
}
