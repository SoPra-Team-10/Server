project(Tests)

enable_testing()
find_package(GTest)
if (GTest_FOUND)
    include_directories(${GTEST_INCLUDE_DIR})

    set(TEST_SOURCES
            Communication/Messages/Common/types.cpp
            Communication/Messages/Common/Delta.cpp
            Communication/Messages/Broadcast/DeltaBroadcast.cpp
            Communication/Messages/Broadcast/GlobalDebug.cpp
            Communication/Messages/Broadcast/LoginGreeting.cpp
            Communication/Messages/Broadcast/MatchConfig.cpp)

    add_executable(${PROJECT_NAME} main.cpp ${SOURCES} ${TEST_SOURCES})
    target_link_libraries(${PROJECT_NAME} ${LIBS} gmock gtest pthread)

    add_test(
            NAME ${PROJECT_NAME}
            COMMAND ${PROJECT_NAME}
    )
endif()

