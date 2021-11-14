add_executable(day01 day01.c)
add_executable(day02 day02.c)
add_executable(day03 day03.c)
add_executable(day04 day04.c)
add_executable(day05 day05.c)
add_executable(day06 day06.c)
add_executable(day07 day07.c)
add_executable(day08 day08.c)

add_executable(day10 day10.c)
add_executable(day11 day11.c)

target_link_libraries(day04 ssl crypto)
