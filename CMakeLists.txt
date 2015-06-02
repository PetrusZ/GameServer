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
set(CMAKE_CXX_FLAGS "-std=c++11  -rdynamic")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -Wall")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(INCLUDE_DIR_LIST ${PROJECT_SOURCE_DIR})

############################################## GameServer ##############################################

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

aux_source_directory(. ROOT_SRC_LIST)
aux_source_directory(./network NETWORK_SRC_LIST)
aux_source_directory(./port PORT_SRC_LIST)
aux_source_directory(./common COMMON_SRC_LIST)
aux_source_directory(./util UTIL_SRC_LIST)
aux_source_directory(./game GAME_SRC_LIST)

set(SRC_LIST
    ${ROOT_SRC_LIST}
    ${NETWORK_SRC_LIST}
    ${PORT_SRC_LIST}
    ${COMMON_SRC_LIST}
    ${UTIL_SRC_LIST}
    ${GAME_SRC_LIST}
    )

message(STATUS "SRC_LIST: ${SRC_LIST}")
message(STATUS "INCLUDE_DIR_LIST: ${INCLUDE_DIR_LIST}")

include_directories(${INCLUDE_DIR_LIST})
link_directories(${PROJECT_SOURCE_DIR}/library)

add_executable(GameServer ${SRC_LIST})
target_link_libraries(GameServer event_core event_pthreads)

set(SCRIPT_LIST
    scripts/Start.sh
    scripts/Stop.sh
    scripts/ClearLog.sh
    )
# INSTALL(DIRECTORY script DESTINATION ${EXECUTABLE_OUTPUT_PATH} PATTERN "scripts/*" PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)

# message(STATUS "SCRIPT_LIST: ${SCRIPT_LIST}")
INSTALL(PROGRAMS ${SCRIPT_LIST} DESTINATION ${EXECUTABLE_OUTPUT_PATH} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE)

################################################# test #################################################

set(TEST_CLIENT_SRC_LIST
    ${NETWORK_SRC_LIST}
    ${PORT_SRC_LIST}
    ${COMMON_SRC_LIST}
    ${UTIL_SRC_LIST}
    test/TcpServerTest.cc
    )
add_executable(TestClient ${TEST_CLIENT_SRC_LIST})
target_link_libraries(TestClient event_core event_pthreads)
