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

set(CMAKE_VERBOSE_MAKEFILE off)
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -Wall")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

aux_source_directory(./ SRC_LIST)

include_directories(${PROJECT_SOURCE_DIR})
link_directories(${PROJECT_SOURCE_DIR}/library)

if(${MY_BUILD_TYPE} MATCHES "debug")
    add_executable(GameServer ${SRC_LIST})
    # target_link_libraries(hellod Ad Bd.a Cd.so)
else()
    add_executable(GameServer ${SRC_LIST})
    # target_link_libraries(hello A B.a C.so)
endif()
