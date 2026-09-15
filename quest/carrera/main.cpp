#include <iostream>
#include "plot.h"

/// Ejemplo de plot con la clase LinePlot
int main () {
    std::vector<float> x = {-2, -1, 0, 1, 2};
    std::vector<float> y = {-2, 0, 2, -2, 0};

    LinePlot plot("plot.svg");
    plot.plot(x, y, "Data");
    
    return 0;
}