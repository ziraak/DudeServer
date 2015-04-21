cmake_minimum_required(VERSION 3.1)
project(DudeServer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    README.md)

add_executable(DudeServer ${SOURCE_FILES} source/main.c source/utils/utils.h source/utils/utils.c source/main.h)
add_executable(DudeTestClient ${SOURCE_FILES} test/client/test_client.c source/utils/utils.h source/utils/utils.c)