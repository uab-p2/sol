#include "histogram.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>

void Histogram::print(const std::vector<float>& frequencies,
                      unsigned width, unsigned height,
                      float tolerance) {
    float sum = 0;
    for (float frequency : frequencies) {
        sum += frequency;
    }

    unsigned count = static_cast<unsigned>(frequencies.size());
    unsigned gaps = count > 0 ? count - 1 : 0;
    unsigned inner_width = width * count + gaps;
    if (std::abs(sum - 1.0f) > tolerance) {
        std::string message = "Error: frequencies add up to "
            + std::to_string(sum) + ", not 1 (tolerance "
            + std::to_string(tolerance) + ").";
        inner_width = std::max(inner_width,
            static_cast<unsigned>(message.size()));
        message.resize(inner_width, ' ');
        std::cout << "+" << std::string(inner_width, '-')
            << "+" << std::endl;
        std::cout << "|" << message << "|" << std::endl;
    } else {
        std::cout << "+" << std::string(inner_width, '-')
            << "+" << std::endl;
        std::vector<unsigned> filled_rows(frequencies.size(), 0);
        for (unsigned i = 0; i < frequencies.size(); i++) {
            float normalized = std::max(0.0f, frequencies[i]);
            filled_rows[i] = static_cast<unsigned>(
                std::round(normalized * height));
        }

        for (unsigned row = height; row-- > 0;) {
            std::cout << "|";
            for (unsigned i = 0; i < filled_rows.size(); i++) {
                if (i > 0) {
                    std::cout << " ";
                }
                char glyph = filled_rows[i] > row ? '#' : ' ';
                std::cout << std::string(width, glyph);
            }
            std::cout << "|" << std::endl;
        }
    }
    std::cout << "+" << std::string(inner_width, '-')
        << "+" << std::endl;
}
