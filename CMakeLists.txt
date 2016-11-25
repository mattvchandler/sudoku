cmake_minimum_required (VERSION 2.8.8)
project(sudoku)

#flags
set(CMAKE_CXX_FLAGS "-Wall -std=c++17")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

add_executable(sudoku
    sudoku.cpp
    )
add_executable(sudoku_old
    sudoku_old.cpp
    )
