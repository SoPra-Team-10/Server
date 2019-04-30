#include <gtest/gtest.h>
#include <Communication/Messages/Request/DeltaRequest.hpp>

using namespace communication::messages;

using T = request::DeltaRequest;

TEST(CommunicationMessagesRequestDeltaRequest, SerializeDeserialize) {
    T orig{};
    std::string ser;
    nlohmann::json json;
    T comp;
    EXPECT_NO_THROW(json = orig);
    EXPECT_NO_THROW(ser = json.dump());
    EXPECT_NO_THROW(comp = nlohmann::json::parse(ser).get<T>());
    EXPECT_EQ(comp, orig);
}
