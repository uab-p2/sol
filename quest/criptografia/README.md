---
tags: [vector_gym:7, homework:3-8]
---
# Cripto

Hoy los nanobots quieren jugar con la criptografía.
Están reorganizando datos hasta hacerlos irreconocibles,
y el reto es recuperar los datos originales. 
¿Te animas a jugar una ronda?

{{ img_badge("bit_spin.png") }}

{{ goals(
    "Identifica e implementa los métodos de `IntVectorSkills` relevantes.",
    "Soluciona el problema presentado reusando `IntVectorSkills` tanto como sea posible.",
) }}

Los nanobots reciben los datos de entrada en un `std::vector<int>` con una cantidad 
\(N\) de elementos, \(N\) impar, \(N \ge 3\). Sobre este vector pueden aplicar las
siguientes operaciones:

* Operación A.<br/>
  El elemento en la posición central queda fijo. Los dos bloques de \( (N-1)/2 \) elementos
  se intercambian, manteniendo el orden dentro de los bloques.<br/>
  Por ejemplo, `{1, 2, 3, 4, 5}` pasa a ser `{4, 5, 3, 1, 2}`.

* Operación B.<br/>
  Todos los elementos se desplazan 1 posición a la derecha.
  El elemento que estaba en la última posición pasa a la primera.
  El tamaño del vector no cambia.<br/>
  Por ejemplo, `{1, 2, 3, 4, 5}` pasa a ser `{5, 1, 2, 3, 4}`.

* Operación C.<br/>
  El elemento en la posición central no cambia. Los bloques izquierdo y derecho
  (de \( (N-1)/2 \) elementos cada uno) se suman uno a uno (primero con primero, etc.) 
  y el resultado se almacena en el bloque derecho. Así, el bloque izquierdo no cambia,
  y el derecho se sobrescribe.<br/>
  Por ejemplo, `{1, 2, 3, 4, 5}` pasa a ser `{1, 2, 3, 5, 7}`.

Junto con la entrada, los bots reciben la clave de cifrado, que indica las
operaciones realizadas y cuántas veces. Algo como `A1C2A1B3`:

* Las operaciones pueden aparecer en cualquier orden, siempre en mayúscula.
* A cada letra le sigue un número entre 1 y 9.

:::compile_and_run solution
:::

Puedes asumir que las claves están bien formadas y
que el tamaño de la entrada es acorde a la descripción.

El ejercicio consiste en implementar las tres operaciones, sus inversas
y las primitivas de cifrado y descifrado.

{{ snippet_box("crypto.h") }}

!!! questions

    * Identifica los métodos de `IntVectorSkills` que se pueden combinar
      para solucionar la función pedida. Impleméntalos y pruébalos.

    * Implementa las tres operaciones y sus inversas en `crypto.cpp`
      usando los métodos de `IntVectorSkills`.

    * Implementa las funciones `nanobot_encrypt` y `nanobot_decrypt`
      combinando las operaciones anteriores.

{{ help_links("std_vector", "vector_operations") }}