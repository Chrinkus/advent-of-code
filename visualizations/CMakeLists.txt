cmake_minimum_required(VERSION 3.23)

project(aoc-viz VERSION 0.1 LANGUAGES C)

include(FetchContent)
FetchContent_Declare(fruity
        GIT_REPOSITORY  https://github.com/Chrinkus/fruity.git
        GIT_TAG         origin/v0.4
)
FetchContent_MakeAvailable(fruity)

add_executable(1518 1518.c)
target_link_libraries(1518 fruity ncurses)
