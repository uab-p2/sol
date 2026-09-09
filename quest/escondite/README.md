---
tags: [ vector_gym:6, homework:3-7 ]
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

La idea de este ejercicio es usar un método de búsqueda más optimizado aprovechando el orden
de la entrada.

!!! questions

    * Identifica los métodos de `IntVectorSkills` que se pueden combinar
      para solucionar las funciones pedidas. Impleméntalos y pruébalos.

    * Implementa las funciones `hunt_unordered` y `hunt_ascending` en `hunt.h|cpp`,
      usando los métodos de `IntVectorSkills`.

:::compile_and_run solution
:::

{{ codex_links ("std_vector", "vector_operations") }}
