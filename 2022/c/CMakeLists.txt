cmake_minimum_required(VERSION 3.22)

project(aoc-2022 VERSION 0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED YES)

# Support Library
find_package(cgs REQUIRED)

# General compilation flags
add_compile_options(-Wall -Wextra -Werror)
link_directories(${CMAKE_INSTALL_PREFIX}/lib64)

# Solution processing
set(solutions
        "2201.c"
        "2202.c"
        "2203.c"
        "2204.c"
        "2205.c"
)

foreach(file IN LISTS solutions)
        get_filename_component(exe "${file}" NAME_WE)
        add_executable("${exe}" "${file}")
        target_link_libraries("${exe}" cgs)
endforeach()
