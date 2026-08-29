#include "random.h"
#include <random>

static std::mt19937 engine{std::random_device{}()};

int Random::get_int(int a, int b) {
    std::uniform_int_distribution<int> distribution(a, b);
    return distribution(engine);
}

bool Random::get_bool(double probability) {
    std::bernoulli_distribution distribution(probability);
    return distribution(engine);
}
