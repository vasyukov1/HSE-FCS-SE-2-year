cmake_minimum_required(VERSION 3.28)
project(A1)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)

file(GLOB SOURCES
        src/*.cpp
        main.cpp
)

add_executable(A1 ${SOURCES})