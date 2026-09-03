---
tags: [vector_gym:2, homework:3-7]
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

El nanobot realiza la función `prepare_assembly`,
la cual devuelve una nueva secuencia en la que:
 
* Los IDs aparezcan en orden.
* Los IDs aparezcan como mucho una vez.
* Si un ID aparecía en `ids`, también aparece en la salida.
* Si un ID no aparecía en `ids`, tampoco aparece en la salida.

{{ snippet_box("prepare_assembly", declaration=True, open=True) }}

!!! questions

    * Identifica los métodos de `IntVectorSkills` que se pueden combinar
      para solucionar la función pedida. Impleméntalos y pruébalos.

    * Implementa la función `prepare_assembly` en `assembly.cpp`
      usando los métodos de `IntVectorSkills`. 

:::compile_and_run solution highlight=(10,10)
:::