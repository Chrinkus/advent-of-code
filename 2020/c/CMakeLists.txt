cmake_minimum_required(VERSION 3.18)

project(aoc-2020 VERSION 0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Hurt me more
add_compile_options(-Wall -Wextra -Wpedantic -O2)

# GLib
find_package(PkgConfig REQUIRED)
pkg_check_modules(glib REQUIRED IMPORTED_TARGET glib-2.0)

# Selected solutions
set(solutions
	"2001.c"
	"2002.c"
	"2003.c"
)

# Build all the solutions
foreach(file IN LISTS solutions)
	get_filename_component(exe_name "${file}" NAME_WE)
	add_executable("${exe_name}" "${file}")
	target_link_libraries("${exe_name}" PkgConfig::glib)
endforeach()

