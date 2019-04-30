#include <gtest/gtest.h>
#include <Communication/Messages/Broadcast/PauseResponse.hpp>

using namespace communication::messages;

using T = broadcast::PauseResponse;

TEST(CommunicationMessagesBroadcastPauseResponse, SerializeDeserialize) {
    T orig{"Test", "Test2", true};
    std::string ser;
    nlohmann::json json;
    T comp;
    EXPECT_NO_THROW(json = orig);
    EXPECT_NO_THROW(ser = json.dump());
    EXPECT_NO_THROW(comp = nlohmann::json::parse(ser).get<T>());
    EXPECT_EQ(comp, orig);
}
