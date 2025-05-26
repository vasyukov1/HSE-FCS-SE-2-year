cmake_minimum_required(VERSION 3.28)
project(SET_9)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)

file(GLOB SOURCES
        src/*.cpp
        main.cpp
)

add_executable(SET_9 ${SOURCES})