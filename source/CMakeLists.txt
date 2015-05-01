cmake_minimum_required(VERSION 3.1)
project(DudeServer)

# all files needed for just the main
set(MAIN_SOURCES main.c)

# all files contained inside the library
set(LIB_SOURCES utils/utils.c
    commands/login.c commands/join.c commands/privmsg.c commands/part.c
    database/database.c database/channel.c database/user.c)

# all header files
set(HEADER_SOURCES main.h
    utils/utils.h
    commands/login.h commands/join.h commands/privmsg.h commands/part.h
    database/database.h database/channel.h database/user.h database/databaseStructs.h database/dbDefines.h)

# creating the library for the server
add_library(DudeServerLib STATIC ${LIB_SOURCES} ${HEADERS})
target_link_libraries(DudeServerLib ${LIBXML2_LIBRARIES})

# create the executable and link the library
add_executable(DudeServer ${HEADERS} ${MAIN_SOURCES})
target_link_libraries(DudeServer DudeServerLib)

# remove all older database files
add_custom_command(TARGET DudeServer PRE_BUILD
COMMAND ${CMAKE_COMMAND} -E remove_directory
$<TARGET_FILE_DIR:DudeServer>/xml)
add_custom_command(TARGET DudeServer PRE_BUILD
COMMAND ${CMAKE_COMMAND} -E remove_directory
$<TARGET_FILE_DIR:DudeServer>/database)
add_custom_command(TARGET DudeServer POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory
${CMAKE_SOURCE_DIR}/xml $<TARGET_FILE_DIR:DudeServer>/xml)