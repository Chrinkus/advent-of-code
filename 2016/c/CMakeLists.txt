cmake_minimum_required(VERSION 3.16)

project(aoc-2016 VERSION 0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Hurt me more
add_compile_options(-Wall -Wextra -Wpedantic -O2)

# My external library
set(LIBSXC_DIR "/home/$ENV{USER}/.local/libsxc")

include_directories("${LIBSXC_DIR}/include")
link_directories("${LIBSXC_DIR}/lib")

# Selected solutions
set(solutions
	"1601.c"
	"1602.c"
	"1603.c"
	"1604.c"
	"1605.c"
)

# Build all the solutions
foreach(file IN LISTS solutions)
	get_filename_component(exe_name "${file}" NAME_WE)
	add_executable("${exe_name}" "${file}")
	target_link_libraries("${exe_name}" sxc)
endforeach()

# SSL development package needed for Day 5
# Ubuntu package: libssl-dev
target_link_libraries(1605 ssl crypto)

