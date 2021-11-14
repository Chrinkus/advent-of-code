cmake_minimum_required(VERSION 3.16)

project(aoc_2020 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

include_directories(include)

add_subdirectory(alt)	# daily solutions

add_executable(day01 day01.cpp)
add_executable(day02 day02.cpp)
add_executable(day03 day03.cpp)
add_executable(day04 day04.cpp)
add_executable(day05 day05.cpp)
add_executable(day06 day06.cpp)
add_executable(day07 day07.cpp)
add_executable(day08 day08.cpp)
add_executable(day09 day09.cpp)
add_executable(day10 day10.cpp)
add_executable(day11 day11.cpp)
add_executable(day12 day12.cpp)
add_executable(day13 day13.cpp)
add_executable(day14 day14.cpp)
add_executable(day15 day15.cpp)
add_executable(day16 day16.cpp)
add_executable(day17 day17.cpp)

