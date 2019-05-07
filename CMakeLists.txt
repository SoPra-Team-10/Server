cmake_minimum_required(VERSION 3.10)

# Build options
set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror -Wno-implicit-fallthrough")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
endif ()

# Building
project(Server VERSION 0.0.1 DESCRIPTION "Server for SoPra")

set(SOURCES
        ${CMAKE_SOURCE_DIR}/Util/ArgumentParser.cpp
        ${CMAKE_SOURCE_DIR}/Util/Logging.cpp
        ${CMAKE_SOURCE_DIR}/Communication/MessageHandler.cpp
        ${CMAKE_SOURCE_DIR}/Communication/Communicator.cpp
        ${CMAKE_SOURCE_DIR}/Communication/Lobby.cpp
        ${CMAKE_SOURCE_DIR}/Communication/AnimationQueue.cpp
        ${CMAKE_SOURCE_DIR}/Game/Game.cpp
        ${CMAKE_SOURCE_DIR}/Game/GameTypes.cpp
        ${CMAKE_SOURCE_DIR}/Game/MemberSelector.cpp
        ${CMAKE_SOURCE_DIR}/Game/PhaseManager.cpp
        ${CMAKE_SOURCE_DIR}/Game/conversions.cpp
        ${CMAKE_SOURCE_DIR}/Util/Timer.cpp)

set(LIBS pthread stdc++fs SopraGameLogic SopraMessages SopraNetwork)

include_directories(${CMAKE_SOURCE_DIR})
add_executable(${PROJECT_NAME} main.cpp ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBS})

add_subdirectory(Tests)
