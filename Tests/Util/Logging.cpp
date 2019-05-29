#include <gtest/gtest.h>

#include <Util/Logging.hpp>

using namespace util;

TEST(UtilLogging, NoLogSendError) {
    std::stringstream stringstream;

    Logging logging{stringstream, 0};
    ASSERT_NO_THROW(logging.error("abc"));
    ASSERT_TRUE(stringstream.str().empty());
}

TEST(UtilLogging, NoLogSendWarn) {
    std::stringstream stringstream;

    Logging logging{stringstream, 0};
    ASSERT_NO_THROW(logging.warn("abc"));
    ASSERT_TRUE(stringstream.str().empty());
}

TEST(UtilLogging, NoLogSendInfo) {
    std::stringstream stringstream;

    Logging logging{stringstream, 0};
    ASSERT_NO_THROW(logging.info("abc"));
    ASSERT_TRUE(stringstream.str().empty());
}

TEST(UtilLogging, NoLogSendDebug) {
    std::stringstream stringstream;

    Logging logging{stringstream, 0};
    ASSERT_NO_THROW(logging.debug("abc"));
    ASSERT_TRUE(stringstream.str().empty());
}




TEST(UtilLogging, ErrorSendError) {
    std::stringstream stringstream;

    Logging logging{stringstream, 1};
    ASSERT_NO_THROW(logging.error("abc"));
    ASSERT_FALSE(stringstream.str().empty());
}

TEST(UtilLogging, ErrorSendWarn) {
    std::stringstream stringstream;

    Logging logging{stringstream, 1};
    ASSERT_NO_THROW(logging.warn("abc"));
    ASSERT_TRUE(stringstream.str().empty());
}

TEST(UtilLogging, ErrorSendInfo) {
    std::stringstream stringstream;

    Logging logging{stringstream, 1};
    ASSERT_NO_THROW(logging.info("abc"));
    ASSERT_TRUE(stringstream.str().empty());
}

TEST(UtilLogging, ErrorSendDebug) {
    std::stringstream stringstream;

    Logging logging{stringstream, 1};
    ASSERT_NO_THROW(logging.debug("abc"));
    ASSERT_TRUE(stringstream.str().empty());
}



TEST(UtilLogging, WarnSendError) {
    std::stringstream stringstream;

    Logging logging{stringstream, 2};
    ASSERT_NO_THROW(logging.error("abc"));
    ASSERT_FALSE(stringstream.str().empty());
}

TEST(UtilLogging, WarnSendWarn) {
    std::stringstream stringstream;

    Logging logging{stringstream, 2};
    ASSERT_NO_THROW(logging.warn("abc"));
    ASSERT_FALSE(stringstream.str().empty());
}

TEST(UtilLogging, WarnSendInfo) {
    std::stringstream stringstream;

    Logging logging{stringstream, 2};
    ASSERT_NO_THROW(logging.info("abc"));
    ASSERT_TRUE(stringstream.str().empty());
}

TEST(UtilLogging, WarnSendDebug) {
    std::stringstream stringstream;

    Logging logging{stringstream, 2};
    ASSERT_NO_THROW(logging.debug("abc"));
    ASSERT_TRUE(stringstream.str().empty());
}




TEST(UtilLogging, InfoSendError) {
    std::stringstream stringstream;

    Logging logging{stringstream, 3};
    ASSERT_NO_THROW(logging.error("abc"));
    ASSERT_FALSE(stringstream.str().empty());
}

TEST(UtilLogging, InfoSendWarn) {
    std::stringstream stringstream;

    Logging logging{stringstream, 3};
    ASSERT_NO_THROW(logging.warn("abc"));
    ASSERT_FALSE(stringstream.str().empty());
}

TEST(UtilLogging, InfoSendInfo) {
    std::stringstream stringstream;

    Logging logging{stringstream, 3};
    ASSERT_NO_THROW(logging.info("abc"));
    ASSERT_FALSE(stringstream.str().empty());
}

TEST(UtilLogging, InfoSendDebug) {
    std::stringstream stringstream;

    Logging logging{stringstream, 3};
    ASSERT_NO_THROW(logging.debug("abc"));
    ASSERT_TRUE(stringstream.str().empty());
}




TEST(UtilLogging, DebugSendError) {
    std::stringstream stringstream;

    Logging logging{stringstream, 4};
    ASSERT_NO_THROW(logging.error("abc"));
    ASSERT_FALSE(stringstream.str().empty());
}

TEST(UtilLogging, DebugSendWarn) {
    std::stringstream stringstream;

    Logging logging{stringstream, 4};
    ASSERT_NO_THROW(logging.warn("abc"));
    ASSERT_FALSE(stringstream.str().empty());
}

TEST(UtilLogging, DebugSendInfo) {
    std::stringstream stringstream;

    Logging logging{stringstream, 4};
    ASSERT_NO_THROW(logging.info("abc"));
    ASSERT_FALSE(stringstream.str().empty());
}

TEST(UtilLogging, DebugSendDebug) {
    std::stringstream stringstream;

    Logging logging{stringstream, 4};
    ASSERT_NO_THROW(logging.debug("abc"));
    ASSERT_FALSE(stringstream.str().empty());
}



TEST(UtilLogging, ArbitrarySendError) {
    std::stringstream stringstream;

    Logging logging{stringstream, 42};
    ASSERT_NO_THROW(logging.error("abc"));
    ASSERT_FALSE(stringstream.str().empty());
}

TEST(UtilLogging, ArbitrarySendWarn) {
    std::stringstream stringstream;

    Logging logging{stringstream, 42};
    ASSERT_NO_THROW(logging.warn("abc"));
    ASSERT_FALSE(stringstream.str().empty());
}

TEST(UtilLogging, ArbitrarySendInfo) {
    std::stringstream stringstream;

    Logging logging{stringstream, 42};
    ASSERT_NO_THROW(logging.info("abc"));
    ASSERT_FALSE(stringstream.str().empty());
}

TEST(UtilLogging, ArbitrarySendDebug) {
    std::stringstream stringstream;

    Logging logging{stringstream, 42};
    ASSERT_NO_THROW(logging.debug("abc"));
    ASSERT_FALSE(stringstream.str().empty());
}
