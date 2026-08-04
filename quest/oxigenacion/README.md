# Oxigenación

Las escuadrillas de reparación necesitarán un suministro fiable de oxígeno
cuando lleguen a la estación SOL. Nos han pedida ayuda para 
diseñar los tanques de aire comprimido y los puntos de dispensación.
Te guiamos para para hacer un diseño a prueba de fallos. 

{{ img_badge("oxygen.png") }}

??? objetivo

    * Implementa las clases `Tank` y `Dispenser` cumpliendo con los requisitos.
    * Produce una demo que muestre el funcionamiento de ambas clases.
    * Discute el funcionamiento de std::map y de los destructores.

## Tanques

### Creación
Los tanques pueden ser de varios tipos. Sólo se pueden crear de los siguientes modos:

- "micro", "normal", "jumbo": 100, 1000 y 10000 litros respectivamente.
  Estos se crean usando el nombre directamente, p.ej., `Tank tank1("micro");`

- a medida: la cantidad concreta de aire se indica directamente con un entero sin signo
  `Tank tank2(5000);`.

### Uso
Una vez creado un tanque, quien lo use sólo debe poder hacer dos cosas:

- Respirar. El retorno será `true` sólo si había suficiente
  aire en el tanque para la respiración completa. El aire se gasta al respirar,
  pero sólo si hay aire para la respiración completa.

{{ snippet_box("Tank::breathe", include_definitions=False, include_declarations=True) }}

- Comprobar la presión del aire. Debe ser 1.0 cuando 
  el tanque esté lleno, y 0.0 cuando esté vacío.

{{ snippet_box("Tank::get_pressure", include_definitions=False, include_declarations=True) }}

!!! questions

    * Implementa la clase `Tank` en `tank.h/cpp` considerando los requisitos anteriores.

    * Qué tipo de protección ofreces para evitar que se manipule la cantidad de aire sin permiso:
      ¿en tiempo de compilación, o en tiempo de ejecución? ¿Cuál es la diferencia?

{{ codex_links("class_constructor", "class_encapsulation") }}

## Puntos de dispensación

### Dispensación
Los puntos de dispensación (`Dispenser`) permiten distribuir los tanques 
entre el personal del proyecto SOL. El aire (ilimitado) se recoge en botellas,
indicando el nombre de la persona y del tipo de botella con:

{{ snippet_box("Dispenser::dispense", include_definitions=False, include_declarations=True) }}


### Contadores

Queremos saber cuántas botellas se ha llevado cada persona.
Te proponemos implementarlo con un mapa (diccionario) de string a unsigned int,
`std::map<std::string, unsigned int>` (aunque no es la única manera).

:::compile_and_run open=false
#include <map>
#include <iostream>
#include <string>
using namespace std;

int main() {
map<string,int> dict;

// Write
dict["nose"] = 1;
dict["eye"] = 2;
dict["ears"] = 2;

// Read
cout << "There are " << dict["eye"] << " eyes in this face" << endl;
if (! dict.contains("hand")) {
  cout << "Fortunately no hands in this face" << endl;
}

// Iterate
int face_parts = 0;
for (auto& [key, value] : dict) {
face_parts += value;
}
cout << "There are " << face_parts << " parts in total in this face" << endl;
}
:::

### Reciclaje

Al acabar la vida útil del dispensador, queremos mostrar cuántas botellas
se han entregado a cada persona, y el total. Para no olvidarnos de mostrar
este resumen y de reciclar los tanques, 
te proponemos utilizar el mecanismo de los destructores.

:::compile_and_run open=false
#include <iostream>
using namespace std;

class MyClass {
public:
  ~MyClass() { cout << "No!" << endl; }
};

int main() {
  MyClass my_object;
  cout << "Is it the end?" << endl;
  return 0;
}
:::

!!! questions

    * Implementa la clase `Dispenser` en `dispenser.h/cpp` cumpliendo con los requisitos anteriores.
    * Implementa una demo en `main.cpp` que muestre el funcionamiento de `Tank` y `Dispenser`.
    * ¿Es posible controlar exactamente cuándo se ejecuta el destructor de una clase?

:::compile_and_run solution
:::

{{ codex_links("std_map", "class_destructor") }}

# Tags
en_ruta:2
time:60
