#include <catch2/catch.hpp>

#include "Intcode_vm.hpp"

TEST_CASE("Op can be constructed and queried", "[CTOR]") {
	Op op1 {3};
	REQUIRE(op1.code() == 3);
	REQUIRE(op1.mode(0) == 0);
	REQUIRE(op1.mode(1) == 0);
	REQUIRE(op1.mode(2) == 0);

	Op op2 {50142};
	REQUIRE(op2.code() == 42);
	REQUIRE(op2.mode(0) == 1);
	REQUIRE(op2.mode(1) == 0);
	REQUIRE(op2.mode(2) == 5);
}
