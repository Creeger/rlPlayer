cmake_minimum_required(VERSION 3.16)

project(rlp LANGUAGES C)

set(CMAKE_C_STANDARD 99)

# Raylib
find_package(raylib REQUIRED)

# Main C executable
add_executable(rlp src/main.c src/playlist.c)

target_link_libraries(rlp PRIVATE raylib)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

