---
tags: [vector_gym:8]
---
# Metamorfosis

Los nanobots aprenden a operar sobre secuencias de cualquier tipo de pieza, no sólo enteros.

{{ img_badge("placeholder.png") }}

{{ goals(
    "Generaliza la librería de secuencias a cualquier tipo mediante templates.",
) }}

## TODO

- Enunciado: convertir cada operación de `sequence.h`/.cpp a `template <typename T>`, mismos nombres.
- Enlazar codex de templates (`site/sections/codex/template.md`, revisar si hace falta ampliarlo).
- test.cpp sistemático sobre las operaciones pedidas.
- PIN: decidir qué operaciones necesitan clase comparadora/predicado y si van al final de este mismo quest o en uno propio (evitar lambdas).
