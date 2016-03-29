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

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

aux_source_directory(./src ROOT_SRC_LIST)
aux_source_directory(./src/shared/network NETWORK_SRC_LIST)
aux_source_directory(./src/shared/port PORT_SRC_LIST)
aux_source_directory(./src/shared/common COMMON_SRC_LIST)
aux_source_directory(./src/shared/util UTIL_SRC_LIST)
aux_source_directory(./src/shared/thread THREAD_SRC_LIST)
aux_source_directory(./src/database DATABASE_SRC_LIST)

set(SRC_LIST
    ${ROOT_SRC_LIST}
    ${NETWORK_SRC_LIST}
    ${PORT_SRC_LIST}
    ${COMMON_SRC_LIST}
    ${UTIL_SRC_LIST}
    ${THREAD_SRC_LIST}
    ${DATABASE_SRC_LIST}
    )

set(INCLUDE_DIR_LIST
    ./src/shared
    ./src/database
    )

set(LIBRARY_DIR_LIST
    ${PROJECT_SOURCE_DIR}/src/shared/library
    )

message(STATUS "SRC_LIST: ${SRC_LIST}")
message(STATUS "INCLUDE_DIR_LIST: ${INCLUDE_DIR_LIST}")
message(STATUS "LIBRARY_DIR_LIST: ${LIBRARY_DIR_LIST}")

include_directories(${INCLUDE_DIR_LIST})
link_directories(${LIBRARY_DIR_LIST})

add_executable(Database ${SRC_LIST})
target_link_libraries(Database event_core event_pthreads mysqlclient hiredis protobuf)
