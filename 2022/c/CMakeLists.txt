cmake_minimum_required(VERSION 3.22)

project(aoc-2022 VERSION 0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED YES)

# Support Libraries
include(FetchContent)
FetchContent_Declare(cgs
        GIT_REPOSITORY  https://github.com/Chrinkus/libcgs.git
        GIT_TAG         main
)
FetchContent_Declare(fruity
        GIT_REPOSITORY  https://github.com/Chrinkus/fruity.git
        GIT_TAG         main
)
FetchContent_MakeAvailable(cgs fruity)

# Define the 'Profile' build-type and set default to 'Release' for
# non-multi-config generators
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

# Solution processing
set(solutions
        "2201.c"
        "2202.c"
        "2203.c"
        "2204.c"
        "2205.c"
        "2206.c"
        "2207.c"
        "2208.c"
        "2209.c"
        "2210.c"
        "2211.c"
        "2212.c"
        #"2213.c"
)

foreach(file IN LISTS solutions)
        get_filename_component(exe "${file}" NAME_WE)
        add_executable("${exe}" "${file}")
        target_link_libraries("${exe}" cgs fruity)
endforeach()
