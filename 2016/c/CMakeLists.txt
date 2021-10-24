cmake_minimum_required(VERSION 3.16)

project(aoc-2016 VERSION 0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Hurt me more
add_compile_options(-Wall -Wextra -Wpedantic)

include_directories("../../include")

# Selected solutions
set(solutions
	"1601.c"
	"1602.c"
	"1603.c"
	"1604.c"
)

# Build all the solutions
foreach(file IN LISTS solutions)
	get_filename_component(exe_name "${file}" NAME_WE)
	add_executable("${exe_name}" "${file}")
endforeach()

