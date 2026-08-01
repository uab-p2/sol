#include <iostream>
#include "star.h"

int main(int argc, char* argv[]) {
    Star sol;
    Star alpha_centauri("Alpha Centauri", -0.503, -1.839, -3.714, 580);
    Star proxima_centauri("Proxima Centauri", -0.471, -1.851, -3.816, "red");

    std::cout << "The distance between " << sol.get_name()
        << " and " << alpha_centauri.get_name()
        << " is " << sol.distance(alpha_centauri) << " light years." << std::endl;

    std::cout << "The distance between " << sol.get_name()
        << " and " << proxima_centauri.get_name()
        << " is " << sol.distance(proxima_centauri) << " light years." << std::endl;

    return 0;
}
