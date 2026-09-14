---
tags: [vector_gym:9, session:9]
---
# Sabotaje

Ya no basta con medir: ahora los nanobots deben sabotear sus propios algoritmos, construyendo la entrada más adversa posible para cada uno.

{{ img_badge("placeholder.png") }}

{{ goals(
    "Construye entradas de tamaño N que fuercen el peor caso de un algoritmo dado.",
    "Distingue algoritmos cuyo peor caso es muy distinto del caso medio de aquellos en los que coinciden.",
) }}

## TODO

- 3 algoritmos: fácil = búsqueda lineal (`rastreo`); difícil = inserción ordenada (`ensamblaje`); imposible (peor≈medio) = búsqueda binaria (`rastreo`).
- Reutilizar el framework de medición de `carrera`.
- PIN: futuro cuestionario/juego sobre complejidades (fuera de esta campaña por ahora).
