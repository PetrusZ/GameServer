# GameServer CMake project
#
#   Build example:
#
#       mkdir build
#       cd build
#       cmake ..
#       make
#
cmake_minimum_required(VERSION 2.8)
project(GameServer)

SET(CMAKE_BUILD_TYPE debug)
set(CMAKE_VERBOSE_MAKEFILE off)
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -Wall")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(INCLUDE_DIR_LIST ${PROJECT_SOURCE_DIR})

############################################## GameServer ##############################################

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

aux_source_directory(./ ROOT_SRC_LIST)
aux_source_directory(./network NETWORK_SRC_LIST)
aux_source_directory(./port PORT_SRC_LIST)
aux_source_directory(./common COMMON_SRC_LIST)

set(SRC_LIST
    ${ROOT_SRC_LIST}
    ${NETWORK_SRC_LIST}
    ${PORT_SRC_LIST}
    ${COMMON_SRC_LIST}
    )

include_directories(${INCLUDE_DIR_LIST})
link_directories(${PROJECT_SOURCE_DIR}/library)

add_executable(GameServer ${SRC_LIST})
target_link_libraries(GameServer event_core)

################################################# test #################################################

set(TEST_CLIENT_SRC_LIST
    test/client.cc
    )
add_executable(client ${TEST_CLIENT_SRC_LIST})
target_link_libraries(client event_core pthread)

set(TEST_SERVER_SRC_LIST
    test/server.cc
    )
add_executable(server ${TEST_SERVER_SRC_LIST})
target_link_libraries(server event_core pthread)
