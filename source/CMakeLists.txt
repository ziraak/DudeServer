# all files needed for just the main
set(MAIN_SOURCES main.c)

# all files contained inside the library
set(LIB_SOURCES utils/utils.c communication.c
    commands/login.c commands/join.c commands/privmsg.c commands/part.c commands/create_user.c commands/delete_user.c commands/update_password.c commands/update_nickname.c
    database/database.c database/channel.c database/user.c database/databaseStructs_free.c
    commands/poll.c)

# all header files
set(HEADER_SOURCES main.h
    utils/utils.h communication.h
    commands/login.h commands/join.h commands/privmsg.h commands/part.h commands/create_user.h commands/delete_user.h commands/update_password.h commands/update_nickname.h
    database/database.h database/channel.h database/user.h database/databaseStructs.h database/dbDefines.h
    commands/poll.h)

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
add_custom_command(TARGET DudeServer POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory
${CMAKE_SOURCE_DIR}/xml $<TARGET_FILE_DIR:DudeServer>/xml)