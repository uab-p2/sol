#include <iostream>

#include "red.h"
#include "green.h"
#include "blue.h"

/// Use this main for your demo
int main(int argc, char* argv[]) {
    talk_to_red();

    std::cout << std::endl;
    talk_to_green();

    std::cout << std::endl;
    talk_to_blue();
    return 0;
}
