---
tags: [ vector_gym:10, session:10 ]
---

# Metamorfosis

Lo mejor de los nanobots es que pueden mutar delante de tus ojos para adaptarse al problema que tienen delante.
Si saben ser un martillo para clavos, saben ser una maza para rocas.
Hoy por fin te explican cómo lo hacen.

{{ img_badge ("hammers.png") }}

{{ goals (
"Descubre el concepto de template.",
"Implementa plantillas de funciones y clases.",
"Transforma tu librería `IntVectorSkills` en una genérica, `VectorSkills`.",
) }}


## Vieja escuela

Los robots de la vieja escuela sabían hacer cada uno una única cosa. 
Si querían buscar elementos en secuencias de `int`, de `float` y de `std::string`,
necesitaban tres funciones:

:::compile_and_run highlight=((7,7), (20,20), (33,33), (46,48))
#include <iostream>
#include <vector>
#include <string>
using namespace std;

/// Search an int
bool contains_int(const vector<int>& sequence, int target) {
     bool contains = false;

     for (int value : sequence) {
          if (value == target) {
               contains = true;
          }
     }

     return contains;
}

/// Search a float
bool contains_float(const vector<float>& sequence, float target) {
     bool contains = false;

     for (float value : sequence) {
          if (value == target) {
               contains = true;
          }
     }

     return contains;
}

/// Search a string
bool contains_string(const vector<string>& sequence, string target) {
     bool contains = false;

     for (string value : sequence) {
          if (value == target) {
               contains = true;
          }
     }

     return contains;
}

int main() {
     bool int_inside = contains_int({1,2,3}, 3);
     bool string_inside = contains_string({"hello", "byte"}, "red");
     bool float_inside = contains_float({0.5,1.5,2.5}, 1.5);

     cout << "Contains int: " 
          << (int_inside ? "yes" : "no") << endl;
     cout << "Contains string: " 
          << (string_inside ? "yes" : "no") << endl;
     cout << "Contains float: " 
          << (float_inside ? "yes" : "no") << endl;

     return 0;
}
:::

!!! questions

     * ¿Qué alternativa ofrecerían las {{ def("template", text="plantillas") }} en este contexto?
       Implementa y prueba esta alternativa en tu `main.cpp`.

     * Propón ventajas e inconvenientes de cada una de las estrategias.

     * Si creamos un nuevo tipo `Student` y queremos buscar en secuencias de objetos `Student`,
       ¿qué necesitamos hacer en cada una de las estrategias?

{{ help_links("data_types", "template") }}


## Nueva escuela

Es hora de demostrarles a los nanobots que tú también eres de la nueva escuela. 
Quieren que generalices tu biblioteca `IntVectorSkills` y que prepares un 
ejercicio de gimnasio usando cuatro {{ def("tipo", text="tipos de dato") }} 
distintos (`int` puede ser uno de ellos).

Tienes libertad en cuanto al contenido del ejercicio, pero te proponemos seguir
el estilo de los quests que has trabajado hasta ahora en esta campaña {{ campaign_link("vector_gym") }}.

!!! questions

    * Implementa una biblioteca genérica `VectorSkills` como {{ def("template", text="plantilla") }} 
      en `vector_skills.hpp`. Revisa el apartado sobre {{ def("template-in-hpp", text="ficheros hpp") }}
      si lo necesitas.

    * Porta tus tests, probando para secuencias de dos {{ def("tipo", text="tipos de dato") }} diferentes.
      Garantiza una cobertura del 100% de todos tus métodos.

    * Esmérate con un ejercicio original e implementa una demo en `main.cpp`.

    * ¿Qué diferencias ves entre el mecanismo de {{ def("template", text="plantillas") }}
       y el de {{ def("herencia", text="herencia") }}/{{ def("polimorfismo", text="polimorfismo") }}?

:::compile_and_run solution highlight=(7,12)
:::