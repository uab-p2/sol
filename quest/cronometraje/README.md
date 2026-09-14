---
tags: [vector_gym:8, session:8]
---
# Cronometraje

A los nanobots les gusta tomar prestadas naves y competir por los mejores tiempos. 
Hoy van a hacer una prueba cronometrada, ¿te unes?

{{ img_badge("complexity.png") }}

{{ goals(
    "Mide tiempos de ejecución reales de varios algoritmos en función del tamaño de la entrada.",
    "Relaciona los tiempos medidos con la complejidad teórica de cada algoritmo.",
) }}

## Análisis de tiempos

En {{ quest_link("escondite") }}, 

## Prueba: plot.h

Prueba temporal del mecanismo de archivos de salida de `:::compile_and_run`
(ver la pestaña "Archivos"), usando `plot.h|cpp` para generar un gráfico
de líneas.

:::compile_and_run title="sin(x) con plot.h" collapsible=false
#include <cmath>
#include <iostream>
#include "plot.h"

int main() {
    std::vector<float> x;
    std::vector<float> sine;
    for (int i = 0; i <= 40; i++) {
        float value = i / 40.0f * 2 * 3.14159265f;
        x.push_back(value);
        sine.push_back(std::sin(value));
    }

    std::vector<float> xticks = {0, 1.5708f, 3.1416f, 4.7124f, 6.2832f};
    LinePlot plot("sine.svg", "sin(x)", "x (rad)", "sin(x)", false, false, xticks);
    plot.plot(x, sine, "sin(x)");
    plot.save();

    std::cout << "sine.svg generado." << std::endl;
    return 0;
}
:::

## TODO

- PIN: framework sencillo de medición de tiempos + plotting (decidir formato/herramienta).
- 4 algoritmos con tiempo medio: búsqueda lineal (lineal), búsqueda binaria (logarítmico), suma máxima de subarray fuerza bruta reutilizando `balance` en bucle anidado (cuadrático) vs. versión de una pasada (lineal) — mismo problema, dos complejidades.
- Quest largo (~90min).
