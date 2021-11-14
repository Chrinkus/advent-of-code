# Alternate solutions
add_executable(day02a day02_alt.cpp)
add_executable(day03p day03_parallel.cpp)
add_executable(day05a day05_alt.cpp)

if(UNIX)
    set(THREADS_PREFER_PTHREAD ON)
    find_package(Threads REQUIRED)

    target_link_libraries(day03p PRIVATE Threads::Threads)
endif()


