---
tags: [vector_gym:9, session:9]
---
# Fraude

Durante la última prueba tipo `linear_race`
en el quest anterior ({{ quest_link("carrera") }})
hubo acusaciones de fraude.
Únete a la investigación y forma tu propia opinión.

{{ img_badge("cheating.png") }}

{{ goals(
    "Compara casos mejor, peor, medio.",
    "Diseña casos ideales, normales y patológicos."
) }}


## Reglamento

En esta prueba, juego limpio significa vectores de entrada
con valores aleatorios *uniformemente distribuidos*
en \( [-V,V] \), con \( V = 10000 \).

El primer sospechoso es el generador de números aleatorios:
podría estar viciado. La primera tarea de investigación
es reproducir la generación de secuencias y analizarlas estadísticamente.

La frecuencia relativa de cada valor se calcula igual que en
{{ quest_link("recuento") }}.

{{ snippet_box("generate_random", declaration=True, open=False) }}

{{ snippet_box("analyze_random", declaration=True, open=False) }}

!!! questions

    * Utilizando el `Random::get_int` de `src/random.h`,
      implementa `generate_random` en `investigation.cpp`.

    * Implementa `analyze_random` (también en `investigation.cpp`),
      que calcula la frecuencia relativa de cada valor de
      \( [-V,V] \) en la secuencia generada, en orden.

    * Obtén la frecuencia mínima y máxima del resultado. ¿Cuánto se
      distancian entre sí? ¿Qué te dice esto sobre el generador?

{{ help_links("std_vector", "vector_operations") }}

## Cámara lenta

La investigación continúa. Necesitamos estudiar los tiempos de ejecución
de `linear_search` parecido al quest {{ quest_link("carrera") }}.

Genera secuencias aleatorias de N elementos, \( V = 10000 \),
para valores crecientes de N. Después:

* Obtén los tiempos de ejecución para encontrar el valor \( x = 0 \) en cada secuencia.
* Obtén también los tiempos para el valor 88888 (siempre ausente).
* Compara los resultados con `LinePlot` disponible en `src/plot.h`.

!!! questions

    * ¿Qué {{ def("complejidad", text="complejidad") }} observas?
      ¿Cuál esperabas?
    * ¿Qué relación hay entre las dos líneas?
    * ¿Qué papel juega \( x \) en los tiempos?
    * ¿Qué papel juega \( V \) en los tiempos?

## Fraude

Por descarte, el último elemento bajo sospecha es la elección de los elementos de búsqueda.

Queremos repetir las mediciones, eligiendo el valor de búsqueda de tres maneras:

* En un caso, se elige el valor menos conveniente para `linear_search`.
* En otro caso, se elige el valor más conveniente para `linear_search`.
* En el último caso, se elige un valor aleatorio en \( [-V,V] \) cada vez.

!!! questions

    * ¿Qué valores son los más y menos convenientes?

    * ¿Qué relación hay entre las líneas que muestras?
      ¿Es posible hacer trampas en la carrera?

    * ¿Qué ocurre si cambiamos `linear_search` por `is_ascending`?
      ¿Cambian nuestras opciones de hacer trampa o sabotear?

    * ¿Qué tiene más sentido analizar:
      el {{ def("caso peor", text="caso peor") }},
      el {{ def("caso medio", text="caso medio") }}
      o el {{ def("caso ideal", text="caso ideal") }}?
