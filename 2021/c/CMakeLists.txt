cmake_minimum_required(VERSION 3.18)

project(aoc-2021 VERSION 0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Hurt me more
add_compile_options(-Wall -Wextra -Wpedantic -O2)

# My external library
set(LIBSXC_DIR "/home/$ENV{USER}/.local")

include_directories("${LIBSXC_DIR}/include")
link_directories("${LIBSXC_DIR}/lib")

# Selected solutions
set(solutions
	"2101.c"
	"2102.c"
	"2103.c"
	"2104.c"
	"2105.c"
	"2106.c"
	"2107.c"
	"2108.c"
)

# Build all the solutions
foreach(file IN LISTS solutions)
	get_filename_component(exe_name "${file}" NAME_WE)
	add_executable("${exe_name}" "${file}")
	target_link_libraries("${exe_name}" sxc)
endforeach()

