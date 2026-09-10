/// Catch2-based testing for a gradescope assignment
#include "gradescope.hpp"

// Test weight between brackets (interpreted outside catch2)
TEST_CASE("No code tests for this quest") {
    REQUIRE(1 + 1 == 2);
}

int main(int argc, char* argv[]) {
    return gradescope_main(argc, argv);
}
