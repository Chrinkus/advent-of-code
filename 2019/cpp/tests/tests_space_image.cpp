#include <catch2/catch.hpp>
#include "Space_image.cpp"

#include <string>

const int width = 2;
const int height = 2;
const std::string good = "0222112222120000";
const std::string bad = "012012";

TEST_CASE("is_valid_dimensions works", "[is_valid_dimensions]") {
    REQUIRE(is_valid_dimensions(width, height, good) == true);
    REQUIRE(is_valid_dimensions(width, height, bad) == false);
}

TEST_CASE("Space_image constructs correctly", "[CTOR]") {
    Space_image img {width, height, good};

    REQUIRE(img.width() == width);
    REQUIRE(img.height() == height);

    try {
        Space_image img2 {width,height, bad};
    }
    catch (Bad_image_data) {
        std::cerr << "Image dimensions do not match data size\n";
    }
}
