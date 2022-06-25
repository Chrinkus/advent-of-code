cmake_minimum_required(VERSION 3.18)

project(aoc-2018 VERSION 0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Libraries
find_package(cgs REQUIRED)
find_package(fruity REQUIRED)

# Protections against different generator types behaving differently
# Boilerplate-y from Professional CMake Section 14.3
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
                        "Unknown build type: ${CMAKE_BUILD_TYPE}"
                )
        endif()
endif()

# Profile build settings
set(CMAKE_C_FLAGS_PROFILE               "-pg -g -O2" CACHE STRING "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_PROFILE      "-pg" CACHE STRING "" FORCE)

# General compilation flags
add_compile_options(-Wall -Wextra -Werror)

if(APPLE)
        include_directories(${CMAKE_INSTALL_PREFIX}/include)
        link_directories(${CMAKE_INSTALL_PREFIX}/lib)
endif()

set(solutions
        "1801.c"
	"1802.c"
	"1803.c"
	"1804.c"
	"1805.c"
	"1806.c"
        "1807.c"
        "1808.c"
        "1809.c"
        "1810.c"
        "1811.c"
)

foreach(file IN LISTS solutions)
	get_filename_component(exe_name "${file}" NAME_WE)
	add_executable("${exe_name}" "${file}")
	target_link_libraries("${exe_name}" cgs fruity)
endforeach()

