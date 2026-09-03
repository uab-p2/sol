---
tags: [vector_gym:2, homework:5-6]
---
# Ensamblaje

El ensamblaje de piezas suele ser uno de los primeros empleos para los nanobots.
En este ejercicio, practicarás una de sus tareas principales: la reorganización.

{{ img_badge("assembly.png") }}

{{ goals(
"Identifica e implementa los métodos de `IntVectorSkills` relevantes.",
"Soluciona el problema presentado reusando `IntVectorSkills` tanto como sea posible.",
) }}

## Reorganización

El nanobot recibe un `std::vector<int>` que representa los IDs de las piezas
necesarias para el ensamblaje:

* Los IDs pueden llegar en cualquier orden.
* Los IDs pueden aparecer repetidos cualquier número de veces.

El nanobot realiza la función `void prepare_assembly(std::vector<int>& ids)`,
la cual modifica `ids` de tal manera que:

* Los IDs aparezcan en orden.
* Los IDs aparezcan como mucho una vez.

!!! questions

    * Identifica los métodos de `IntVectorSkills` que se pueden combinar
      para implementar la función pedida.

    * Implementa y prueba esos métodos.

    * Implementa la función `prepare_assembly` usando los métodos de `IntVectorSkills`. 