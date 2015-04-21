cmake_minimum_required(VERSION 3.1)
project(DudeServer)

set(SOURCE_FILES source/utils/utils.h source/utils/utils.c README.md)
set(TEST_CLIENT_FILES ${SOURCE_FILES})

add_executable(DudeServer ${SOURCE_FILES} source/main.c  source/main.h)
add_executable(DudeTestClient ${TEST_CLIENT_FILES} test/client/test_client.c)