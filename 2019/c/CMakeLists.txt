cmake_minimum_required(VERSION 3.16)

project(aoc-2019 VERSION 0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Hurt me more
add_compile_options(-Wall -Wextra -Wpedantic)

include_directories("../../include")

# Selected solutions
set(solutions
	"1901.c"
	"1902.c"
)

# Build all the solutions
foreach(file IN LISTS solutions)
	get_filename_component(exe_name "${file}" NAME_WE)
	add_executable("${exe_name}" "${file}")
endforeach()

# Symlink inputs for ease of running
#file(CREATE_LINK "${PROJECT_SOURCE_DIR}/inputs"
#"${PROJECT_SOURCE_DIR}/build/inputs" SYMBOLIC)
