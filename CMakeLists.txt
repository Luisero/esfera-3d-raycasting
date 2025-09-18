cmake_minimum_required(VERSION 3.10)

project(main)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
file(GLOB SOURCES src/*.cpp)
find_package(SFML 2.5.1 REQUIRED COMPONENTS system window graphics)


add_executable(main ${SOURCES})
target_include_directories(main PRIVATE include)
# 4. Linka as bibliotecas ao seu executável.
target_link_libraries(main PRIVATE sfml-graphics)
