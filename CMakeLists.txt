cmake_minimum_required(VERSION 3.16)
project(rlp C)

set(CMAKE_C_STANDARD 99)

find_package(raylib REQUIRED)

add_executable(rlp src/main.c)

target_link_libraries(rlp PRIVATE raylib)

if (NOT MAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
