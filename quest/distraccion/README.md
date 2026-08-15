# Distracción

Naves y estaciones remotas como SOL cuentan con sistemas para generar 
contenido audiovisual a partir de guiones. La tripulación os ha rogado
que creéis un sistema para producir guiones a partir de objetos que
representan personajes y atrezzo. Te retamos a componer escenas a partir
de estos elementos.

{{ img_badge("storyboard.png") }}

{{ goals(
    "Crea nuevas clases usando otras existentes (*composición*).",
    "Analiza el efecto de la composición en constructores y destructores.",
    "Aprende sobre el ciclo de vida de las variables."
) }}

## Guión
 
Los guiones soportados por el sistema de generación de contenido 
describen una o más escenas secuencialmente. Cada escena tiene las
siguientes partes:

1. Presentación de personajes.<br/>
   Para cada personaje de la escena, se muestra
   una única línea introduciéndolo. Cada línea empieza por `"Personaje: "`.

2. Apertura de escena.<br/>
   Una única línea describiendo el escenario y
   cualquier otra cosa relevante. Esta línea empieza por `"Escena: "`.

3. Acción (desarrollo de la escena).<br/>
   Un número variable de líneas.
   Cada línea puede ser una acción, algo que se dice, etc.
   No hay formato definido más allá de la lógica y la gramática.

4. Cierre de escena.<br/>
   Una única línea describiendo el escenario 
   en el momento en que comienza a bajar el telón/oscurecerse la pantalla 
   antes de la siguiente escena. Esta línea empieza por `"Escena: "`.
 
5. Salida de personajes.<br/>
   Para cada personas de la escena, se muestra 
   una única línea describiéndolo. Cada línea empieza por `"Personaje: "`.

!!! questions

     * Estudia el siguiente ejemplo mínimo de guión.

:::compile_and_run solution
:::

## Doblaje

!!! questions

      * Reproduce el guión anterior
        mediante un código que lo genere. Es importante que el código
        cumpla las especificaciones siguientes.

          * Todos los mensajes `"Escena: "` y `"Personaje :"` se generan únicamente
            en los constructores/destructores de las clases.
          
          * La clase `Scene`:
               - Está implementada en `scene.h|cpp`
               - Contiene un sólo método público,  
                 {{ snippet_signature("Scene::action", declaration=True) }}.
                 Este método genera un número variable de líneas describiendo la acción,
                 entre las líneas que comienzan por `"Escena: "` y `"Personaje: "`.

          * Cada personaje debe tener su propia clase. Esta clase debe publicar 
            únicamente los constructores, destructores y aquellos métodos usados
            por otros personajes durante la acción de la escena.

          * Todas las clases deben tener un *constructor por defecto*.

      * ¿Influye el orden de definición de los atributos en el orden de ejecución
        a los constructores y destructores?

      * ¿Influye el orden de inicialización directa
        en el orden de ejecución de los constructores y destructores?

!!! example "Ejemplo de inicialización directa"

     ```cpp linenums="1"
     class A : {

     public:
        // Direct initialization 
        A(int x, int y) : m_x(x), m_y(y) { }

     protected:
        int x, y;
     };
     ```

{{ snippet_box("Scene::action", declaration=True, open=False) }}

{{ codex_links("class_constructor", "class_destructor", "class_encapsulation", "class_composition") }}

## Piloto

!!! questions

      * Implementa el código que genera dos escenas consecutivas.  
 
         * Las clases `SceneBegin`, `SceneEnd` 
           (nudo y desenlace) se implementan en 
           `two_scenes.h|cpp` con las mismas reglas que en el ejemplo anterior.

         * La implementación de tu ficción usa al menos:
            - 2 clases ya implementadas en `src/atrezzo.h` y `src/atrezzo.cpp`
            - 2 clases nuevas que implementes `two_scenes.h|cpp`,
              junto a `SceneBegin` y `SceneEnd`.

         * Las clases que creen tienen estado, e interactuan con objetos
           y personajes para cambiar su estado. La salida que producen
           tus clases debe depender de su estado.

      * Implementa tu demo en el `main.cpp` del quest de manera que la salida
        de una escena se complete antes de empezar la siguiente. Discute 
        cómo lo has logrado.

{{ snippet_box("atrezzo.h", open=False) }}

{{ snippet_box("atrezzo.cpp", open=False) }}

# Tags
en_ruta:3
session:3
