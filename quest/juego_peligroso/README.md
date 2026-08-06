# Juego peligroso

El camino será largo. ¿Por qué no fabricarte algo de entretenimiento?
Te guiamos para montar un juego peligroso: el del ahorcado.

{{ img_badge("hangman.png") }}

??? objetivo

    - Diseña, dibuja y nombra las entidades, datos e interacciones que describen el juego.
    - Implementa el juego comenzando por las funciones más sencillas
      y acabando for la función {{ snippet_ref("play_hangman_game") }} 
    - Analiza puntos fuertes y débiles de la implementación.

## Dibujo al natural

!!! questions

    * Antes de programar necesitamos saber qué queremos programar. En primer lugar,
      imagina el juego que quieres crear exactamente. Las únicas restricciones son:
      
        * Recibes la palabra secreta como un argumento (no necesitas generarla tú).
        * La persona que juega introduce las letras una una a través del teclado.
        * Comienza pensando en la versión más sencilla que sepas implementar.
          Tendremos tiempo para refinar el juego más adelante.



!!! questions

    * Ahora coge un papel y un boli (o un lápiz). ¿Podrías explicar tu versión
      del juego usando únicamente diagramas? Te proponemos 
      diagramas de *secuencia*, de *estado* y de *flujo* para dejar claro:

          * ¿Qué entidades participan?
          * ¿Cómo se relacionan?
          * ¿Qué datos manejan?
          * ¿Qué entradas y salidas hay? ¿Usarás teclado, pantalla, ficheros...?

{{ codex_links("diagrams") }}

## Intercambio de idiomas

!!! questions

    * Elegir las palabras correctas nos ayuda a pensar mejor,
      producir un código más expresivo y barato de mantener.
      Repasa los diagramas del punto anterior y escribe una lista 
      con las palabras idóneas para:
        
        * *Acciones* (verbos)
        * *Entidades* (sustantivos)
        * *Datos* (sustantivos)
        
        !!! info
            El standard en el sector es utilizar inglés para todo el código y documentación. 
            Te recomendamos utilizarlo en tus implementaciones del proyecto SOL,
            pero puedes usar castellano o catalán si prefieres.

## Mecanografía

¿Ya lo tienes todo preparado para comenzar a teclear?
Estupendo pero, ¿por dónde comenzar y por dónde seguir?
Te proponemos el siguiente plan de acción:

!!! questions

    1. Abre `quest/juego_peligroso` y comprueba que compila sin problema.
 
    2. Crea el fichero `hangman.h` en la raíz del quest.
       Antes implementar nada:
 
        - Añade en `hangman.h` las *declaraciones* de *funciones* y *structs* 
          usando las palabras de la sección anterior.
        - Asegúrate de documentar todas estas funciones.
          Te explicamos cómo en {{ codex_link("documentation") }}
 
    3. Crea el fichero `hangman.cpp`, también en la raíz del quest.
       Una a una, implementa las funciones de hangman.h de la siguiente manera:
 
        1. Elige la función más sencilla pendiente de implementar.
        2. Crea una primera versión de esa función en `hangman.cpp`.
        3. Prueba esa función desde `test.cpp` hasta que funcione. No olvides el `#include "hangman.h"`.
 
    4. Por último, añade la función {{ snippet_ref("play_hangman_game") }}
        a `hangman.h/cpp` e impleméntala usando únicamente las otras funciones
        que has implementado. Invoca esta función desde el `main()` de `main.cpp`.
        Algo como lo siguiente podría ser suficiente:

:::compile_and_run solution input="a\nx\nn\nt\np\nr\ni\nc"
::: 

{{ codex_links(
    "open_quest", 
    "add_file_quest", 
    "run_quest", 
    "cin_cout", 
    "documentation",
) }}

## Debate

!!! questions

    * Si publicas `hangman.h` y `hangman.cpp` para que puedan jugar otra persona,
      ¿cuántas funciones y structs necesita aprender a usar esa persona?
  
    * Si queremos elegir la palabra secreta de manera automática.
      ¿De cuántas maneras podríamos hacerlo? ¿Entre cuántas palabras podríamos
      elegir como mucho?

# Tags

despegue:1
semana:0
day:0
time:60