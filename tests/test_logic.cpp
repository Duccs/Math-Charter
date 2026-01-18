#include <catch2/catch_test_macros.hpp>

int Add(int a, int b) {
	return a + b;
}

TEST_CASE("Addition works", "[math]") {
	REQUIRE(Add(2, 2) == 4);
	REQUIRE(Add(1, -1) == 0);
}
