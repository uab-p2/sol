---
tags: [vector_gym:3, homework:3-7]
---
# Recuento

Los nanobots no suelen viajar solos. Normalmente, lo hacen en riadas de un puñado de tipos.
En este ejercicio, practicaremos el recuento.

{{ img_badge("swarm.png") }}

La entrada para el recuento es un `std::vector<int>` con los números de modelo de todos los nanobots.
Múltiples bots de cada modelo pueden aparecer, y los números de modelo aparecen en culquier orden.

Necesitamos implementar la función `headcount_distribution` para realizar un recuento de las proporciones
de cada modelo. La función recibe también un número de modelo `model` que se aparta del recuento: es
como si diéramos un paso al lado con ese modelo antes de empezar a contar.
Para ello:

* Si `model` no aparece en absoluto en la entrada, no se retira ni cuenta nada:
  la entrada se deja intacta y se devuelve un vector vacío.
* En caso contrario, primero se retiran de la entrada todos los bots de `model` (permanentemente,
  la entrada se modifica), sin contarlos: quedan totalmente excluidos del recuento.
* De los bots restantes, todos los bots de un cierto modelo se retiran de la entrada permanentemente
  (esta se sigue modificando).
* Se calcula la probabilidad (frecuencia relativa) de cada modelo restante. 
  Recuerda, \( P(x) = \frac{N_x}{N_\text{total}} \).
* Se devuelven las probabilidades, ordenadas por número de modelo de menor a mayor. 
  Sólo se devuelven las probabilidades de los modelos en la entrada (después de las retiradas anteriores).

!!! questions

    * Identifica los métodos de `IntVectorSkills` que se pueden combinar
      para solucionar la función pedida. Impleméntalos y pruébalos.

    * Implementa la función `headcount_distribution` en `headcount.cpp`
      usando los métodos de `IntVectorSkills`, comprobando la presencia
      de `model` con `contains` al principio de la función y manteniendo
      un único punto de retorno.

Hemos preparado por ti la función `Histogram::print` en `src/histogram.h|cpp` para que puedas
visualizar la distribución de las entradas:

:::compile_and_run solution highlight=(17,18)
:::