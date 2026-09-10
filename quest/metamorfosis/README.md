---
tags: [ vector_gym:8, session:7 ]
---

# Metamorfosis

Lo mejor de los nanobots es que pueden mutar delante de tus ojos para adaptarse al problema que tienen delante.
Si saben ser un martillo para clavos, saben ser una maza para rocas.
Hoy por fin te explican cómo lo hacen.

{{ img_badge ("hammers.png") }}

{{ goals (
"Descubre el concepto de template.",
"Transforma tu librería `IntVectorSkills` en una genérica, `VectorSkills`.",
) }}

## Generalización

Durante los últimos quests, has desarrollado y probado la librería `IntVectorSkills`
con utilidades para `std::vector<int>`.

La misma lógica, y casi el mismo código, te puede servir para trabajar con 
`std::vector<float>`, `std::vector<unsigned>`, etc., usando {{ def("template", text="templates") }}.

!!! questions

   * Crea la {{ def("template", text="plantilla") }} `VectorSkills` en `vector_skills.hpp`
     con todos los métodos definidos en `IntVectorSkills`. La extensión es `.hpp` porque
     la implementación de una plantilla tiene que estar en la cabecera: no hay `.cpp`.

   * Porta también las clases auxiliares `IntPredicate` e `IntOperation` a plantillas,
     `Predicate` y `Operation`: `filter` y `transform` las necesitan.

   * Porta tus tests para ejercitar vectores 
     `std::vector<int>`, `std::vector<float>` y `std::vector<std::string>`.


{{ help_links("template") }}


## Concreción



## TODO

- Enunciado: convertir cada operación de `sequence.h`/.cpp a `template <typename T>`, mismos nombres.
- Enlazar el artículo de ayuda de templates (`site/sections/help/template.md`, revisar si hace falta ampliarlo).
- test.cpp sistemático sobre las operaciones pedidas.

