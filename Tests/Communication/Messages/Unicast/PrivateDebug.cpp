#include <gtest/gtest.h>
#include <Communication/Messages/Unicast/PrivateDebug.hpp>

using namespace communication::messages;

using T = unicast::PrivateDebug;

TEST(CommunicationMessagesUnicastPrivateDebug, SerializeDeserialize) {
    T orig{"Test"};
    std::string ser;
    nlohmann::json json;
    T comp;
    EXPECT_NO_THROW(json = orig);
    EXPECT_NO_THROW(ser = json.dump());
    EXPECT_NO_THROW(comp = nlohmann::json::parse(ser).get<T>());
    EXPECT_EQ(comp, orig);
}
