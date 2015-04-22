cmake_minimum_required(VERSION 3.1)
project(DudeServer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#set(SOURCE_FILES README.md)

add_executable(DudeServer ${SOURCE_FILES} source/main.c source/medium.h source/medium.c)

find_package(LibXml2)
include_directories(${LIBXML2_INCLUDE_DIR})
target_link_libraries(DudeServer ${LIBXML2_LIBRARIES})
add_custom_command(TARGET DudeServer PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/database $<TARGET_FILE_DIR:DudeServer>/database)