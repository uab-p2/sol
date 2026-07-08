#include <chrono>
#include <iostream>

/// Quest entry point
int main(void) {
    constexpr int iteration_count = 250'000'000;
    constexpr double reference_seconds = 1.9360;

    double elapsed_seconds;
    {
        volatile double x = 1.0;
        const auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < iteration_count; ++i) {
            x *= 1.000001;
            x /= 1.000001;
        }
        const auto end = std::chrono::steady_clock::now();
        elapsed_seconds = static_cast<double>(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                end - start).count()) / 1000.0;
    }

    if (elapsed_seconds > reference_seconds) {
        std::cout << "Your machine is "
            << 100 * (elapsed_seconds / reference_seconds - 1)
            << "% faster than mine!" << std::endl;
    }
    else {
        std::cout << "Your machine is "
            << 100 * (reference_seconds / elapsed_seconds - 1)
            << "% slower than mine!" << std::endl;
    }

    return 0;
}
