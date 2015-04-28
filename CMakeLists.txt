cmake_minimum_required(VERSION 3.1)
project(DudeServer)

set(SOURCE_FILES
    source/utils/utils.h source/utils/utils.c
    source/commands/login.h source/commands/login.c
    source/commands/join.c source/commands/join.h
    source/commands/privmsg.c source/commands/privmsg.h
    source/medium.h source/medium.c
    source/xmlWriter.h source/xmlWriter.h
    README.md)

set(TEST_CLIENT_FILES ${SOURCE_FILES})

add_executable(DudeServer ${SOURCE_FILES} source/main.c  source/main.h
source/database/channel.h source/database/channel.c source/medium.h source/medium.c
 source/xmlWriter.h source/xmlWriter.c source/database/database.h source/database/user.h source/database/user.c source/database/database.h source/database/database.c)
add_executable(DudeTestClient source/utils/utils.h source/utils/utils.c test/client/test_client.c)

find_package(LibXml2)
include_directories(${LIBXML2_INCLUDE_DIR})
target_link_libraries(DudeServer ${LIBXML2_LIBRARIES})
add_custom_command(TARGET DudeServer PRE_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory
${CMAKE_SOURCE_DIR}/database $<TARGET_FILE_DIR:DudeServer>/database)