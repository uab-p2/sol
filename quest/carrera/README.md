---
tags: [ vector_gym:8, session:8 ]
---

# Carreras

A los nanobots les gusta tomar "prestadas" naves y competir en carreras nocturnas por los mejores tiempos.
Hoy van a hacer una prueba cronometrada, ¿te unes?

{{ img_badge ("complexity.png") }}

{{ goals (
"Mide tiempos de ejecución reales de varios algoritmos en función del tamaño de la entrada.",
"Relaciona los tiempos medidos con la complejidad teórica de cada algoritmo.",
) }}

## Primera noche

La primera noche no te dejan competir, sólo tomar tiempos - son las reglas.
En la primera carrera, compite `prepare_assembly`
(la trabajaste en {{ quest_link("ensamblaje") }}).

{{ snippet_box("prepare_assembly", declaration=True, open=False) }}

Usando las clases `WallClock` y `LinePlot` que trabajaste
en {{ quest_link("escondite") }} y {{ quest_link("recuento") }},
mide el tiempo de `prepare_assembly` para entradas cada vez más grandes.

!!! questions

    * Compara en un único plot los tiempos de ejecución para entradas
      de \(N\) elementos, con \(N \in \{1000,\,2000,\,\ldots,\,10\;000\}\).

    * Considera el tiempo para \(N=5000\) entradas. Calcula también los tiempos para
      \(N=10000\) (el doble), y \(N=15000\) (el triple). ¿Qué patrón observas?

    * ¿Qué {{ def("complejidad", text="complejidad") }} asignarías
      a tu implementación de `prepare_assembly`? Revisa tu código si lo necesitas.

:::compile_and_run quest title="Ejemplo de plot"
:::

{{ help_links("complexity") }}

## Segunda noche

La segunda noche sí podrás competir, pero necesitas ganarte el derecho.

Implementa la función `sum_race` que repite \( R = 3000 \) veces:

* Toma un `std::vector<int>` y calcula su suma \( S \).
* Suma \( i \) a cada elemento del vector, donde \( i \) es el
  índice de repetición (\(i = 0,\,1,\,\ldots,\,R-1\)).

{{ snippet_box("sum_race", declaration=True, open=False) }}

!!! questions

    * Implementa la función `sum_race` en `race.h|cpp`
      y pruébala en `test.cpp`. Emplea `IntVectorSkills` allá donde sea posible.

    * Compara los tiempos de `sum_race` y `prepare_assembly`
      para \( N = 5000 \) elementos. ¿Cuál es más rápida?

    * Compara los tiempos de `prepare_assembly` y `sum_race` en el mismo plot
      (llama al método `plot` dos veces). ¿Es la evolución que esperabas?

    * ¿Qué ocurre para \( N = 10\;000 \) y \( N = 15\;000 \) (el doble y el triple) elementos de entrada?
      ¿Qué {{ def("complejidad", text="complejidad") }} le asignarías?

## Tercera noche

La tercera noche es la gran carrera. Implementa `linear_race` y
`binary_race`, usando `linear_search` y `binary_search` respectivamente
(las trabajaste en {{ quest_link("escondite") }}).

Ambas funciones repiten \( R \) veces los siguientes pasos:

1. La entrada es un `std::vector<int>` y un número \( x \) a buscar.
   Se calcula la posición \( p \) de la primera aparición de \( x \) en el vector.
2. Se actualiza el elemento buscado: \( x = p^2 + 1 \).

La salida es la última posición \( p \) calculada.

{{ snippet_box("linear_race", declaration=True, open=False) }}

{{ snippet_box("binary_race", declaration=True, open=False) }}

Sabemos que `binary_search` es más rápida, así que su carrera corre con una
pinza atada a la espalda:

* `linear_race` corre con \( R=1 \)
* `binary_race` corre con \( R = 300 \)

!!! questions

    * Implementa `linear_race` y `binary_race` en `race.h|cpp` y pruébalas
      en `test.cpp`.

    * ¿Qué {{ def("complejidad", text="complejidad") }} asignarías a cada una de
      estas implementaciones?