#include "blue.h"
#include <iostream>

void talk_to_blue() {
    std::cout << "Hi, I'm Blue! I like Earth's function g." << std::endl;
    std::cout << "If you ask me about g(10), the answer is "
        << (g(10) ? "yes" : "no") << std::endl;
    std::cout << "If you ask me about g(11), the answer is "
        << (g(11) ? "yes" : "no") << std::endl;
    std::cout << "Do you agree?" << std::endl;
}

bool g(int x) {
    bool s = true;
    for (int y = x - 1; y >= 2; y--) {
        const int r = x - y * (x / y);
        switch (r) {
        case 0:
            s = false;
        default:
            s = true;
        }
    }
    return s;
}
