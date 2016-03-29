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
set(CMAKE_CXX_FLAGS_RELEASE "-Wall")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")

set(SCRIPT_LIST
    ./src/scripts/Start.sh
    ./src/scripts/Stop.sh
    ./src/scripts/ClearLog.sh
    )
# INSTALL(DIRECTORY script DESTINATION ${EXECUTABLE_OUTPUT_PATH} PATTERN "scripts/*" PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)

# message(STATUS "SCRIPT_LIST: ${SCRIPT_LIST}")
INSTALL(PROGRAMS ${SCRIPT_LIST} DESTINATION ${EXECUTABLE_OUTPUT_PATH} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE)
