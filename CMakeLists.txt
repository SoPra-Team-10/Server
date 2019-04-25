cmake_minimum_required(VERSION 3.10)

# Build options
set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
endif ()

# Building
project(Server VERSION 0.0.1 DESCRIPTION "Server for SoPra")

set(MESSAGE_TYPE_SOURCES
        ${CMAKE_SOURCE_DIR}/Communication/Messages/Broadcast/DeltaBroadcast.cpp
        ${CMAKE_SOURCE_DIR}/Communication/Messages/Broadcast/GlobalDebug.cpp
        ${CMAKE_SOURCE_DIR}/Communication/Messages/Broadcast/LoginGreeting.cpp
        ${CMAKE_SOURCE_DIR}/Communication/Messages/Broadcast/MatchConfig.cpp
        ${CMAKE_SOURCE_DIR}/Communication/Messages/Broadcast/MatchFinish.cpp
        ${CMAKE_SOURCE_DIR}/Communication/Messages/Broadcast/MatchStart.cpp
        ${CMAKE_SOURCE_DIR}/Communication/Messages/Broadcast/Next.cpp
        ${CMAKE_SOURCE_DIR}/Communication/Messages/Broadcast/PauseResponse.cpp
        ${CMAKE_SOURCE_DIR}/Communication/Messages/Broadcast/Replay.cpp
        ${CMAKE_SOURCE_DIR}/Communication/Messages/Broadcast/Snapshot.cpp
        ${CMAKE_SOURCE_DIR}/Communication/Messages/Request/ContinueRequest.cpp
        ${CMAKE_SOURCE_DIR}/Communication/Messages/Request/DeltaRequest.cpp
        ${CMAKE_SOURCE_DIR}/Communication/Messages/Request/GetReplay.cpp
        ${CMAKE_SOURCE_DIR}/Communication/Messages/Request/JoinRequest.cpp
        ${CMAKE_SOURCE_DIR}/Communication/Messages/Request/PauseRequest.cpp
        ${CMAKE_SOURCE_DIR}/Communication/Messages/Request/SendDebug.cpp
        ${CMAKE_SOURCE_DIR}/Communication/Messages/Request/TeamConfig.cpp
        ${CMAKE_SOURCE_DIR}/Communication/Messages/Request/TeamFormation.cpp
        ${CMAKE_SOURCE_DIR}/Communication/Messages/Unicast/JoinResponse.cpp
        ${CMAKE_SOURCE_DIR}/Communication/Messages/Unicast/PrivateDebug.cpp
        ${CMAKE_SOURCE_DIR}/Communication/Messages/Unicast/Reconnect.cpp
        ${CMAKE_SOURCE_DIR}/Communication/Messages/types.cpp)

set(SOURCES
        ${MESSAGE_TYPE_SOURCES}
        ${CMAKE_SOURCE_DIR}/Communication/Communicator.cpp)

set(LIBS SopraNetwork pthread)

include_directories(${CMAKE_SOURCE_DIR})
add_executable(${PROJECT_NAME} main.cpp ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBS})

add_subdirectory(Tests)
