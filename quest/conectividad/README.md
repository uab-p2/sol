---
tags: [vector_gym:7, homework:3-7]
---
# Conectividad

Los nanobots disfrutan con los ejercicios de análisis de datos. 
Uno de sus favoritos consiste en estudiar propuestas de diseño de chips, 
y ver si es posible interconectarlos de la manera pedida usando grafos. Después de varte
varias veces por el gimnasio, te han invitado a probar el ejercicio.

{{ img_badge("graph.png") }}

{{ goals(
"Amplía tu implementación de la biblioteca en `IntVectorSkills`.",
"Combina operaciones básicas sobre `std::vector` para solucionar problemas complejos.",
) }}

Los nanobots reciben la propuesta de diseño de chips en un `std::vector<int>`. 
El vector contiene un número positivo por cada chip del diseño:

* El número indica el tamaño del chip, y el número de conexiones con otros chips que acepta y necesita.
* Cada chip puede conectarse con cualquiera de los demás como mucho una vez, y sólo
  si hay conexiones disponibles en ambos chips.

El diseño sólo es válido si:

* Todos los chips tiene todas sus conexiones satisfechas.
* El vector de entrada llega ordenado de mayor a menor número de conexiones.
* No está vacío ni contiene ningún valor menor que 1.

El ejercicio, dado un `std::vector<int>` de entrada, es determinar si el diseño es realizable: la función
devuelve `true` solamente si es posible interconectar los chips tal y como se pide.
Recomendamos utilizar el algoritmo de {{ def("havel-hakimi", text="Havel-Hakimi") }}.

!!! questions

    * Identifica los métodos de `IntVectorSkills` que se pueden combinar
      para solucionar la función pedida. Impleméntalos y pruébalos.

    * Implementa la función `havel_hakimi` en `havel_hakimi.cpp`
      usando los métodos de `IntVectorSkills`.

!!! example "Ejemplo"

    La secuencia de entrada `{1, 1}` describe dos chips de tamaño 1.
    El diseño es realizable conectándolos entre sí.

!!! example "Ejemplo"

    La secuencia `{3, 1, 1}` **no** es realizable: el chip 0 necesita 3 conexiones, pero sólo
    hay otros 2 chips con los que conectarse.

:::compile_and_run solution
:::

{{ help_links("std_vector", "vector_operations") }}