#include <catch2/catch.hpp>
#include "Intcode_vm.hpp"

TEST_CASE("Intcode_vm can be constructed and parse instructions", "[CTOR]") {

	std::string data = "1,9,10,3,2,3,11,0,99,30,40,50";

	Intcode_vm vm {data};
	REQUIRE(vm.size() == 12 * 4);   // vm memory is resized to 4 * data
}
