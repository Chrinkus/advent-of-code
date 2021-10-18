#include <catch2/catch.hpp>

#include "Point.hpp"

TEST_CASE("Point constructs as expected", "[CTOR]") {
    Point p1;

    REQUIRE(p1.x() == 0);
    REQUIRE(p1.y() == 0);

    Point p2 {5, -7};

    REQUIRE(p2.x() == 5);
    REQUIRE(p2.y() == -7);
}

TEST_CASE("Point equality compare works as intended", "[eq]") {
    Point p1;
    Point p2 {0, 0};

    REQUIRE(p1 == p2);

    Point p3 {0, 1};
    Point p4 {-1, 0};

    REQUIRE(p1 != p3);
    REQUIRE(p2 != p4);
}
