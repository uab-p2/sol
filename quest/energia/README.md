---
tags: [vector_gym:7, homework:3-8]
---
# Energía

Los nanobots entrenan tan duro que hasta la nutrición, basada en tokens, es un ejercicio.
Hoy te han invitado a comer.

{{ img_badge("tokens.png") }}

{{ goals(
"Amplía tu implementación de la biblioteca en `IntVectorSkills`.",
"Combina operaciones básicas sobre `std::vector` para solucionar problemas complejos.",
) }}

Estos nanobots se alimentan de dos tipos de token: de subida y de bajada.
Los consumen por separado, y los tokens grandes son más fáciles de digerir.

Los nanobots reciben los tokens para alimentarse en un `std::vector<int>`.
Números positivos indican tokens de subida, números negativos tokens de bajada,
y ceros indican tiempos muertos a ignorar.
El valor absoluto indica el tamaño del token.

La alimentación la realizan a través de la función `nourish` que recibe
una secuencia de tokens y calcula la ganancia de energía.
Esta ganancia es la suma de los tamaños de todos los tokens, multiplicada por el tamaño
medio de los tokens de bajada (¡sin contar los tiempos muertos!).

El valor de retorno es la energía total, y los tamaños de los tokens de subida y bajada
se almacenan en los otros dos parámetros de la función.

{{ snippet_box("nourish", declaration=True) }}

!!! questions

    * Identifica los métodos de `IntVectorSkills` que se pueden combinar
      para solucionar la función pedida. Impleméntalos y pruébalos.

    * Implementa la función `nourish` en `nourish.cpp`
      usando los métodos de `IntVectorSkills`.

:::compile_and_run solution
:::

{{ help_links("std_vector", "vector_operations") }}