#include <iostream>
#include "cat.h"

/// Quest entry point
int main(int argc, char* argv[]) {
    Cat cat("Nyan");

    std::cout << cat.meow() << std::endl;

    {
        std::cout << std::endl << "Petting...";
        const bool success = cat.pet();
        if (success) {
            std::cout << " it worked!" << std::endl;
        }
        else {
            std::cout << " it didn't work." << std::endl;
        }
        std::cout << cat.meow() << std::endl;
    }

    std::cout << std::endl << "Ok ok, have some food!" << std::endl;
    cat.feed();
    std::cout << cat.meow() << std::endl;

    {
        std::cout << std::endl << "Petting...";
        const bool success = cat.pet();
        if (success) {
            std::cout << " it worked!" << std::endl;
        }
        else {
            std::cout << " it didn't work." << std::endl;
        }
        std::cout << cat.meow() << std::endl;
    }

    std::cout << std::endl << "Ok let's let him sleep..." << std::endl;
    cat.sleep();
    std::cout << cat.meow() << std::endl;

    std::cout << std::endl << "Ok ok, have some more food!" << std::endl;
    cat.feed();
    std::cout << cat.meow() << std::endl;

    return 0;
}
