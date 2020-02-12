#define CATCH_CONFIG_MAIN
#include <Catch2/catch.hpp>

#include "vector.hpp"

TEST_CASE("vectors can be default constructed") {
    auto v = vector<int>{};

    REQUIRE(v.capacity() == 0);
    REQUIRE(v.size() == 0);
}
