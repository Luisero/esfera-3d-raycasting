cmake_minimum_required(VERSION 3.10)

project(main)
file(GLOB SOURCES src/*.cpp)
add_executable(main ${SOURCES})
target_include_directories(main PRIVATE include)
