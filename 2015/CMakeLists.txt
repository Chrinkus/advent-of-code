cmake_minimum_required(VERSION 3.16)

project(aoc_2015 VERSION 0.1 LANGUAGES C)

if(MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_subdirectory(src)
