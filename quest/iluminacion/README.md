# Iluminación

Tu equipo es el encargado de prototipar un nuevo sistema de control de luces
para el proyecto SOL. Para ser compatible, deberás usar el mismo paradigma:
la *programación orientada a objetos (OOP)*. Te invitamos a tomar contacto 
con la potencia de este paradigma.

{{ img_badge("light_string.png") }}

??? Objetivo
    - Descubre las ventajas de la programación orientada a objetos.
    - Implementa un sistema de control de luces utilizando clases ya implementadas.

## Puntos de luz

Toda la iluminación de la estación SOL es mediante puntos de luz individuales
e intercambiables. Cada uno:

* Puede encenderse y apagarse individualmente (on/off).
* Permite cambiar el color de la luz que emite.
* Si está encendido, se puede graduar la intensidad (0-1).

### Candelabros

Sin programación orientada a objetos, podríamos modelar los puntos de luz
con unas pocas variables y algunas funciones asociadas:

```cpp
int main(void) {
    // This is the light point
    bool is_on;
    string color;
    float intensity;
}

/// Turn on a light point
void turn_on(bool& is_on, string& color, float& intensity);
/// Turn off a light point
void turn_off(bool& is_on, string& color, float& intensity);
```

Tienes el ejemplo completo en el `main.cpp` de este quest:
{{ snippet_box("main.cpp") }}

Sin programación orientada a objetos:

!!! questions

    * ¿Cuántas variables necesitamos para 2 puntos de luz? ¿Y para 10?
    * ¿Cómo podemos garantizar que no cruzamos los argumentos (`is_on`, `color`, ...)
     de dos puntos de luz diferentes al llamar a `turn_on` y luego a `turn_off`?
    * Queremos contar cuántas veces se enciende y se apaga cada luz.
          * ¿Cómo lo modelarías?
          * ¿En cuántos sitios necesitamos cambiar nuestro código si tenemos 2 puntos de luz?
          * ¿Y si tenemos 10?
    * ¿Es posible confundir el `turn_on` the una lámpara con el de una lavadora?
    * ¿Cómo podríamos hacer que todas las luces comiencen encendidas?
     Nos gustaría evitar algo como lo siguiente:


:::compile_and_run
#include <iostream>
#include <string>
using namespace std;

int main(void) {
    bool is_on;
    string color;
    float intensity;

    cout << (is_on ? "On" : "Off") << ":" << color << intensity << endl;

    return 0;
}
:::

### Bombillas

Si usamos programación orientada a objetos, podemos agrupar los datos
(las variables de estado) y las funciones (los métodos) para crear *clases*,
como por ejemplo la clase `LightPoint`.

La *cabecera* de la clase describe qué datos y métodos tiene esa clase:
{{ snippet_box("LightPoint") }}

La *implementación* de los métodos de la clase se incluyen en el `.cpp` correspondiente:
{{ snippet_box("light_point.cpp") }}

Considera el siguiente ejemplo de *uso* de la clase `LightPoint`:

:::compile_and_run
#include <iostream>
#include "light_point.h"

int main(void) {
    LightPoint light(true, "white", 0.5);
    std::cout << "Light 1, before: " << light.status() << std::endl;
    light.turn_off();
    std::cout << "Light 1, after:  " << light.status() << std::endl;
    
    LightPoint light2(true, "red", 0.2);
    std::cout << "Light 2, before: " << light2.status() << std::endl;
    light2.turn_off();
    std::cout << "Light 2, after:  " << light2.status() << std::endl;

    return 0;
}
:::

Estudia el código anterior y extrapola:

!!! questions

     * ¿Cómo se crea un objeto (o *instancia*) de una clase?
     * ¿Cómo se invocan los métodos de un objeto?
     * ¿Qué variables usan los métodos de un objeto?
     * Si quisiéramos contar cuántas veces se enciende y se apaga cada luz,
        ¿qué ficheros necesitaríamos cambiar? ¿Más o menos que sin programación orientada a objetos?

Quizá pueda ayudarte:

* {{ codex_link("class_syntax") }}

### Tiras LED

El resto de clases funcionan igual que `LightPoint`. Por tanto, ya puedes aprovechar
la potencia del código escrito y mantenido por miles de personas en todo el mundo.
Hoy proponemos que uses:

* Clase `std::string` (`#include <string>`): representa una cadena de caracteres,
   por ejemplo `"hola"`.

       - Método `string::substr(int pos, int len)`: 
         permite extraer una parte de una cadena, comenzando en la posición `pos` y obteniendo
         `len` caracteres.

* Clase `std::vector<string>`: permite almacenar una secuencia de cadenas
   por ejemplo `{"hola", "hasta luego"}`.

       - Método `vector<string>::push_back(const string& s)`: añade una cadena al final de la secuencia.
        (No hay límite de tamaño para la secuencia).

       - Método `vector<string>::size()`: obtiene la longitud actual de la secuencia. 

       - Corchetes `[i]`: permiten acceder directamente al i-ésimo elemento de la secuencia.

* Clase `std::vector<LightPoint>`: como `std::vector<string>`, pero almacena 
  objetos de la clase `LightPoint` en lugar de cadenas de texto.

El siguiente ejemplo los pone todos en uso:

:::compile_and_run
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main(void) {
    string color1 = "red";
    string color2 = "blue";

    vector<string> colors;
    colors.push_back(color1);
    colors.push_back(color2);

    cout << "variables: " << color1 << ", " << color2 << endl;
    cout << "vector:    " << colors[0] << ", " << colors[1] << endl;
    
    colors[1] = colors[1].substr(1, 2);
    cout << "variables: " << color1 << ", " << color2 << endl;
    cout << "vector:    " << colors[0] << ", " << colors[1] << endl;

    return 0;
}
:::

!!! questions

    * Implementa la función {{ snippet_ref("receive_lights", include_declarations=True) }} 
      en `light_jockey.h/cpp`. Esta función recibe una secuencia de códigos indicando
      el color y estado on/off de las luces existentes y devuelve un array de objetos `LightPoint`
      con esos colores y estados.

{{ snippet_box("receive_lights", include_declarations=True, default_open=True) }}

Quizá pueda ayudarte:

* {{ codex_link("std_vector") }}

## Light Jockey

En este punto, lo tienes todo listo para conectar con el sistema de control
de luces de la estación SOL y convertirte en su light jockey.
     
* El método `control_lights` comienza recibiendo una secuencia con el mismo formato 
  y significado que la esperada por `receive_lights` de la sección anterior.

* Tras recibir la secuencia de luces, el método `control_lights` puede recibir
  uno o más comandos de control. ¿Qué comandos? ¿Con qué formato? Decídelo tú
  y enséñanoslo. Algunas ideas:
  
    * encender/apagar todas las luces rojas
    * añadir más luces
    * cambiar intensidades

!!! questions

      * Implementa el método {{ snippet_ref("control_lights", include_declarations=True) }} 
        en `light_jockey.h/cpp` y una demo en `main.cpp` que muestre su funcionamiento.


{{ snippet_box("control_lights", include_declarations=True) }}





!!! questions
        
                

# Tags
en_ruta:0
semana:1
time:60