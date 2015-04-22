cmake_minimum_required(VERSION 3.1)
project(DudeServer)

set(SOURCE_FILES
    source/utils/utils.h source/utils/utils.c
    source/commands/login.h source/commands/login.c
    source/commands/join.c source/commands/join.h
    source/commands/privmsg.c source/commands/privmsg.h
    README.md)

set(TEST_CLIENT_FILES ${SOURCE_FILES})

add_executable(DudeServer ${SOURCE_FILES} source/main.c  source/main.h)
add_executable(DudeTestClient source/utils/utils.h source/utils/utils.c test/client/test_client.c)