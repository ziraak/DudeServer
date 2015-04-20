cmake_minimum_required(VERSION 3.1)
project(DudeServer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    README.md
    Vagrantfile
    vagrantreadme.txt)

add_executable(DudeServer ${SOURCE_FILES} source/main.c)