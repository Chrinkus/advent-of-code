cmake_minimum_required(VERSION 3.18)

project(aoc-2018 VERSION 0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Werror)

find_package(cgs REQUIRED)

set(solutions
	"1801.c"
	"1802.c"
	"1803.c"
	"1804.c"
	"1805.c"
)

foreach(file IN LISTS solutions)
	get_filename_component(exe_name "${file}" NAME_WE)
	add_executable("${exe_name}" "${file}")
	target_link_libraries("${exe_name}" cgs)
endforeach()

