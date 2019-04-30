#include <gtest/gtest.h>
#include <Communication/Messages/Request/GetReplay.hpp>

using namespace communication::messages;

using T = request::GetReplay;

TEST(CommunicationMessagesRequestGetReplay, SerializeDeserialize) {
    T orig{};
    std::string ser;
    nlohmann::json json;
    T comp;
    EXPECT_NO_THROW(json = orig);
    EXPECT_NO_THROW(ser = json.dump());
    EXPECT_NO_THROW(comp = nlohmann::json::parse(ser).get<T>());
    EXPECT_EQ(comp, orig);
}
