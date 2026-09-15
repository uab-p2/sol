---
tags: [ vector_gym:5, session:6 ]
---

# Escondite

A veces un nanobot se vuelve travieso y juega al escondite. En el gimnasio nos proponen dos ejercicios de búsqueda
y captura.

{{ img_badge ("hunt.png") }}

{{ goals (
"Identifica e implementa los métodos de `IntVectorSkills` relevantes.",
"Soluciona el problema presentado reusando `IntVectorSkills` tanto como sea posible.",
) }}

Las entradas de los dos ejercicios comparten el mismo formato:

* Un `std::vector<int>` que representa los IDs de una fila de nanobots.
  Un mismo ID puede aparecer varias veces en la fila.
* Un ID de búsqueda, que puede estar o no en la fila.

En los dos casos, la función devuelve `true` si el ID buscado está en la fila, y `false` si no está.
Cuando no está, la fila se queda intacta. Lo que cambia entre los dos ejercicios es a cuántos
nanobots hay que capturar.

## Primer ejercicio: `hunt_unordered`

La fila puede estar en cualquier orden. Sólo se captura un nanobot: el de la primera aparición
del ID buscado. Los demás IDs, incluidas las otras apariciones del ID buscado, se quedan en la fila
y mantienen su orden relativo.

## Segundo ejercicio: `hunt_ascending`

Los IDs de la fila llegan ordenados de manera creciente, así que las apariciones repetidas de un
mismo ID quedan juntas. Aquí se capturan todos los nanobots con el ID buscado, no sólo uno.
La fila resultante tiene que seguir estando ordenada.

Importante: utiliza una búsqueda optimizada aprovechando el orden
de la entrada. Considera los siguientes tiempos:

:::compile_and_run title="Comparativa tiempos" highlight=((26,29),(37,40)) quest
:::


!!! questions

    * Identifica los métodos de `IntVectorSkills` que se pueden combinar
      para solucionar las funciones pedidas. Impleméntalos y pruébalos.

    * Implementa las funciones `hunt_unordered` y `hunt_ascending` en `hunt.h|cpp`,
      usando los métodos de `IntVectorSkills`.

    * Mide, con `WallClock`, los tiempos de tus dos búsquedas sobre filas
      de \(10^n\) nanobots, y explica cómo evolucionan al crecer \(n\).

:::compile_and_run solution
:::

{{ help_links ("std_vector", "vector_operations") }}
