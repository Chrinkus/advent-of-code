set(CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/lib/cmake")
find_package(Catch2 REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/source")

add_executable(tests_all
	"tests_main.cpp" 
	"tests_intcode_vm.cpp"
	"tests_op.cpp"
	"tests_space_image.cpp"
	"tests_point.cpp"
)

target_link_libraries(tests_all PRIVATE Catch2::Catch2)
target_link_libraries(tests_all PRIVATE intcode_vm sif sxy_graph)

include(CTest)
include(ParseAndAddCatchTests)
ParseAndAddCatchTests(tests_all)
