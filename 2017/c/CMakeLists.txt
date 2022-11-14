cmake_minimum_required(VERSION 3.22)

project(aoc-2017 VERSION 0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Libraries
find_package(cgs REQUIRED)

# Protections against different generator types behaving differently
# Source: Professional CMake §14.3
get_property(isMultiConfig GLOBAL
        PROPERTY GENERATOR_IS_MULTI_CONFIG
)
if (isMultiConfig)
        if (NOT "Profile" IN_LIST CMAKE_CONFIGURATION_TYPES)
                list(APPEND CMAKE_CONFIGURATION_TYPES Profile)
        endif()
else()
        set(allowedBuildTypes Debug Release Profile)
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY
                STRINGS "${allowedBuildTypes}"
        )
        if (NOT CMAKE_BUILD_TYPE)
                set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
        elseif(NOT CMAKE_BUILD_TYPE IN_LIST allowedBuildTypes)
                message(FATAL_ERROR
                        "Unkown build type: ${CMAKE_BUILD_TYPE}"
                )
        endif()
endif()

# Profile build settings
set(CMAKE_C_FLAGS_PROFILE               "-pg -g -02" CACHE STRING "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_PROFILE      "-pg" CACHE STRING "" FORCE)

# General compilation flags
add_compile_options(-Wall -Wextra -Werror)
link_directories(${CMAKE_INSTALL_PREFIX}/lib64)

set(solutions
        "1701.c"
        "1702.c"
        "1704.c"
)

foreach(file IN LISTS solutions)
        get_filename_component(exe_name "${file}" NAME_WE)
        add_executable("${exe_name}" "${file}")
        target_link_libraries("${exe_name}" cgs)
endforeach()

