---
tags: [vector_gym:3, homework:3-7]
---
# Recuento

Los nanobots no suelen viajar solos. Normalmente, lo hacen en riadas de un puñado de tipos.
En este ejercicio, practicaremos el recuento.

{{ img_badge("swarm.png") }}

{{ goals(
"Amplía tu implementación de la biblioteca en `IntVectorSkills`.",
"Combina operaciones básicas sobre `std::vector` para solucionar problemas complejos.",
) }}

La entrada para el recuento es un `std::vector<int>` con los números de modelo de todos los nanobots.
Múltiples bots de cada modelo pueden aparecer, y los números de modelo aparecen en cualquier orden.

Cuando necesitan contarse, los nanobots hacen lo siguiente:

1. Todos los bots con el número de modelo más bajo se apartan de la población (permanentemente).
   Son los encargados de contar al resto y no forman parte de las estadísticas. Estos bots apartados no vuelven
   a la población después del recuento.

2. Considerando la población de nanobots restantes, se calcula la probabilidad (frecuencia relativa)
   de cada modelo. Se devuelve en un `std::vector<float>` por orden creciente de número de modelo. Recuerda:
   \( P(x) = \frac{N_x}{N_\text{total}} \)

!!! questions

    * Identifica los métodos de `IntVectorSkills` que se pueden combinar
      para solucionar la función pedida. Impleméntalos y pruébalos.

    * Implementa la función `headcount_distribution` en `headcount.cpp`
      usando los métodos de `IntVectorSkills`.

Hemos preparado por ti la función `Histogram::print` en `src/histogram.h|cpp` para que puedas
visualizar la distribución de las entradas como aquí:

:::compile_and_run solution highlight=(17,18)
:::

{{ codex_links("std_vector", "vector_operations") }}