# Navegación

El sistema de navegación de la estación SOL usa la versión moderna
de uno de los métodos más antiguos: las estrellas. Te invitamos a
estudiar primero la clase `Star` para profundizar en cómo se crean los objetos,
y qué partes quedan accesibles u ocultas. Después, te proponemos
implementar un elemento clave del sistema de navegación de la estación.

{{ img_badge("observation.png") }}

??? objetivo
    WIP

## Estrellas

### Finitas

Estudia el código de la clase `Star` y un ejemplo de su uso.

{{ snippet_box("star.h") }}

{{ snippet_box("star.cpp") }}

{{ snippet_box("main.cpp") }}

!!! questions

      * ¿Cuál es la diferencia entre clase y objeto? 
      * ¿Cómo se crea una clase y cómo se crea un objeto?
      * ¿Qué tienen de especial los "métodos" que se llaman igual que la clase?
      * Explica la salida del siguiente código y su relación con esos "métodos".   

:::compile_and_run
#include <iostream>
using namespace std;

class Planet { 
public:
   int radius;
   bool radioactive;

   void report() {
      cout << radius << "m ";
      cout << (radioactive ? "danger" : "safe") << endl;
   }
};

int main() {
   Planet pluto;
   pluto.report();
   return 0;
}
::: 

{{ codex_links("class_syntax", "class_constructor") }}

### Inalcanzables

`Star`, como todos los objetos, muestra una parte accesible para todas 
las demás clases y oculta otra, accesible sólo para la propia clase `Star`.
Te invitamos a jugar con {{ snippet_tag("main.cpp") }} para deducir las
reglas y sintaxis de este sistema llamado *encapsulamiento*.

!!! questions

      * ¿Es posible obtener una salida bizarra como la de más arriba (pluto)
        con la clase `Star`?
      * ¿Es posible crear una estrella de cualquier tipo?
      * Una vez creada una estrella, ¿es posible renombrar una estrella a "SOL"?
      * ¿y cambiar su `wavelength a 150 nm? ¿Y a 145 nm?
      * Discute ventajas e inconvenientes de usar encapsulamiento.

{{ codex_links("class_encapsulation") }}

## Brújula celeste

!!! questions

      * La estación SOL nos ha pedido producir una clase `Compass` en `compass.h/cpp`.
        Impléntala cumpliendo los siguientes requisitos.

* Se puede crear un objeto clase `Compass` pasándole un catálogo
   de estrellas de tipo `const std::vector<Star>`.
   
* Se puede buscar la estrella del catálogo más cercana (en posición) a una buscada
   con un método `Star Compass::find_closest(const Star& search) const`.
  
* Se puede encontrar el *diámetro* del catálogo, definido como la mayor
   distancia entre dos estrellas dentro del mismo, e implementado
   en el método `double diameter() const`.
   
:::compile_and_run solution
::: 
   

# Tags
time:60
en_ruta:1