# Simulación

Antes de cualquier actuación en el espacio, la tripulación realiza 
simulaciones de todas las naves involucradas. Esta vez el sistema les
funciona, pero no es compatible con nuestras naves. Preparémosles un
nuevo sistema de simulación que funcione con cualquier tipo de nave.

{{ img_badge("ships.png") }}

{{ goals(
    "Utiliza *herencia* para especializar clases de objetos.",
    "Diseña jerarquías de clases.",
    "Usa la clase base para manejar las especializaciones."
) }}

## Esencia

La clase `Ship` implementada en `src/ship.h|cpp` describe la "esencia" 
del concepto nave: aquellos atributos y funcionalidades comunes a todos los tipos
de nave posible.

{{ snippet_box("ship.h", open=False) }}

{{ snippet_box("ship.cpp", open=False) }}

!!! questions

    * Estudia el código en `ship.h|cpp`. ¿Qué elementos son nuevos, y cuáles conocidos?

    * ¿Es posible crear un objeto de tipo `Ship`? Explica el siguiente resultado:  

:::compile_and_run indent=2 fails box=false
#include <iostream>
#include "ship.h"
using namespace std;

int main() {
  Ship ship(100, 0, 0, 0);

  cout << (ship.is_working() ? "It works" : "It doesn't work") 
    << endl;

  return 0;
}
:::

{{ codex_links("class_constructor", "class_encapsulation") }}

## Especialización

Para ser útil, el sistema de simulación necesita dos cosas. 
La primera es poder considerar diversas naves especializadas en determinadas tareas.

Empecemos con la clase `Transport` como especialización de `Ship`,
usando la siguiente cabecera (puede extenderse con atributos y métodos adicionales).

{{ snippet_box("Transport") }}

!!! questions

    * ¿Podremos crear objetos tipo `Transport` directamente?

    * ¿Qué funcionalidad(es) de `Ship` hereda `Transport`?
      
    * ¿Qué funcionalidad(es) pueden diferir entre `Ship` y `Transport`?

    * Implementa el resto de la clase en `transport.cpp` considerando 
      los requisitos documentados en `transport.h` (ver arriba) y también:

        1. Creación:

               - Los transportes se crean con `100` unidades de bateria (`battery`)
               - Los transportes comienzan en \( \bar{v}_0 = (0,0,0) \) por defecto, 
                 aunque se puede especificar la posición inicial durante la creación.
               - Si se indica un número de personas a bordo
                 inferior a 10, el número se aumenta a 10 (¡la tripulación es necesaria!).
               - Un objeto `Transport` no puede llevar más de 25000 personas a bordo: si se
                 indica un número superior, se reduce a 25000.

        2. Embarque y desembarque.<br/>Un `Transport` ya creado también puede recoger más
           personas con `load_passengers(n)`. Si caben las `n` personas
           adicionales sin superar el máximo de la nave, sube todo el mundo y el método
           devuelve `true`; si no caben, no sube nadie y devuelve `false`.<br/><br/>
           Simétricamente, `unload_passengers(n)` desembarca personas: si pueden
           bajar las `n` personas indicadas sin dejar la nave por debajo de la tripulación
           mínima (10), baja todo el mundo y el método devuelve `true`; si no, no baja
           nadie y devuelve `false`.

        3. Movimiento.<br/>Una nave posicionada en \( \bar{v}_1 = (x_1,y_1,z_1) \)
           con destino \( \bar{v}_2 = (x_2,y_2,z_2) \) recorrerá la siguiente
           distancia, con el coste de batería asociado 
           (sólo si tiene batería para el viaje completo - si no, no se moverá):
            
           $$ \text{distancia}(\bar{v}_1, \bar{v}_2) = \sqrt{(x_1-x_2)^2 + (y_1-y_2)^2 + (z_1-z_2)^2}, $$

           $$ \text{coste}(\text{distancia}, n_\text{personas}) 
              = 10 + \text{distancia} \cdot \frac{n_\text{personas}}{250}. $$

        

Una vez tengas `Transport` funcionando y probada localmente, 
crea también un transporte ligero `LightTransport` como especialización de `Transport`.

!!! questions

    * Implementa `LightTransport` en `light_transport.h|cpp` con funcionalidad
      idéntica a `Transport` excepto en que:
    
        1. El número máximo de personas a bordo de un `LightTransport` 
           está limitado a 100 (el mínimo sigue siendo 10).

        2. El coste de movimiento por persona por unidad de movimiento es 1/1000.
           en lugar de 1/250.

:::compile_and_run title="Ejemplo `Transport` y `LightTransport`" open=false
#include <iostream>
#include "transport.h"
#include "light_transport.h"
using namespace std;

int main() {
  // Created with the same passage at the same position  
  Transport transport(100, 0, 0, 0);
  LightTransport light_transport(100, 0, 0, 0);

  // Test transport move to (300, 0, 0)
  {
      bool transport_moved = transport.move(300, 0, 0);
      double x, y, z;
      transport.get_position(x, y, z);
      cout << "Transport moved? " 
        << (transport_moved ? "yes" : "no") << endl;
      cout << "Transport position: " 
        << x << "," << y << "," << z << endl;
  }

  // Test light transport move to the same destination
  {
      bool transport_moved = light_transport.move(300, 0, 0);
      double x, y, z;
      light_transport.get_position(x, y, z);
      cout << "Light transport moved? " 
        << (transport_moved ? "yes" : "no") << endl;
      cout << "Light transport position: "
        << x << "," << y << "," << z << endl;
  }

  return 0;
}
:::

{{ codex_links("class_inheritance") }}

## Generalización
    
La segunda cosa que necesita el sistema de simulación para ser útil es poder
manejar todas las especializaciones que queramos definir. En este caso, queremos
poder manejar todos los tipos de `Transport`, sean ligeros o no.

Consideremos una estación de transporte: la clase `Station`.
Estudia la cabecera definida en `station.h` y responde:

{{ snippet_box("Station") }}

!!! questions

    * ¿Qué tipo de objetos pueden pasarse en el primer argumento de `send_passengers`?
        
{{ snippet_box("Station::send_passengers", indent=1, open=False, declaration=True) }}

    * Si creamos un transporte pesado `HeavyTransport` como especialización
      de `Transport`, ¿`snippet_box` funcionaría directamente o necesitaría algún cambio?

    * Si creamos otro tipo de transporte como especialización de `Ship`,
      ¿la podrá manejar tu estación?

Una vez perfilado el comportamiento de `Station`, podemos pasar a la implementación.

!!! questions

    * Implementa en `station.cpp` las funciones declaradas en `station.h`.

    * En `main.cpp`, implementa una demo que simule alguna operación
      utilizando todo el interfaz público de `Station`. 

:::compile_and_run solution title="Una demo trivial e incompleta" open=false
:::



# Tags
en_ruta:5
session:4
