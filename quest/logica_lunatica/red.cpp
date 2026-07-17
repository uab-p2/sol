#include "red.h"
#include <iostream>


void talk_to_red() {
    std::cout << "Hi, I'm Red! I like Earth's nature." << std::endl;
    is_a_bird("eagle");
    is_a_bird("kangaroo");
}

bool is_a_bird(const std::string& name) {
    bool not_a_bird;
    if (name == "eagle" || name == "dove" || name == "parrot"
        || name == "penguin" || name == "pigeon" || name == "swan") {
        not_a_bird = false;
    }
    if (!not_a_bird) {
        std::cout << "I know '" << name << "' is a bird." << std::endl;
    }
    else {
        std::cout << "I do not think '" << name << "' is a bird." << std::endl;
    }
    return !not_a_bird;
}
