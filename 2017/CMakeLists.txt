cmake_minimum_required(VERSION 3.16)

project(aoc_2017 VERSION 0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra -Wpedantic -O2)
endif()

add_subdirectory(src)

