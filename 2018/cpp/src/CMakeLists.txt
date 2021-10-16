# for each dayXX.cpp make a dayXX executable
file(GLOB files "./*.cpp")
foreach(file ${files})
    get_filename_component(exe_name ${file} NAME_WE)
    add_executable(${exe_name} ${file})
    target_compile_features(${exe_name} PRIVATE cxx_std_14)
endforeach()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(day05 PRIVATE Threads::Threads)

