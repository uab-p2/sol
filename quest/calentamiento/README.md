---
tags: [ vector_gym:1, session:3 ]
---

# Calentamiento

Acabas de activarte: eres un nanobot recién ensamblado. Te dedicarás a procesar secuencias kilométricas de elementos,
pero necesitas pasar por el gimnasio para aprender.
Hoy es tu primer día - así que sólo haremos el tour y un ejercicio de calentamiento.

{{ img_badge ("reception_desk.png") }}

{{ goals (
"Descubre la biblioteca de habilidades que crearás.",
"Elige e implementa tus primeras habilidades.",
"Utiliza la biblioteca para solucionar un ejercicio simple.",
) }}

## Biblioteca de habilidades

Durante tu entrenamiento, implementarás una biblioteca de habilidades básicas para {{ def ("vector", text="vectores") }}
de enteros (`std::vector<int>`).

{{ snippet_box ("IntVectorSkills", open=False) }}

Te damos la cabecera de la clase `IntVectorSkills`
en `src/`, pero no la implementación. Hemos agrupado las habilidades en categorías, y las hemos descrito en detalle
en {{ codex_link ("vector_operations") }}. 

El objetivo es completar todos los ejercicios del gimnasio usando 
los métodos apropiados de `IntVectorSkills` en cada caso.
Tú decidirás qué {{ def ("método", text="método(s)") }} necesitas para
solucionar cada quest.

!!! questions

    * Familiarízate con las habilidades y los grupos en `IntVectorSkills`.

    * Todavía no implementes `IntVectorSkills`: lo harás  
      a medida que las necesites en los quests siguientes.


{{ codex_links ("std_vector", "vector_operations") }}

## Calentamiento 

Tu primer ejercicio en este gimnasio es ponerte en el lugar de un 
horno robotizado. 

Es un ejercicio suave. La entrada será una serie temporal de temperaturas.
Sólo queremos saber si el horno se está enfriando y en ningún
momento de la serie sube de temperatura. 

{{ snippet_box("is_oven_cooling", declaration=True) }}

!!! questions

    * Elige e implementa las funciones de IntVectorSkills útiles para este quest.
    * Implementa `is_oven_cooling` usando sólo estas funciones.
    * Añade tus tests y pruébalo todo en gradescope.

:::compile_and_run solution input="200\n200\n150\n100\n160\n90" title="Ejemplo de uso de `is_oven_cooling`" highlight=(15,15)
:::
