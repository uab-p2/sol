# Juego peligroso

El camino será largo. ¿Por qué no fabricarte algo de entretenimiento?
Te guiamos para montar un juego peligroso: el del ahorcado.

{{ img_badge("hangman.png") }}

??? objetivo

    - Diseña y dibuja las entidades, datos e interacciones que describen el juego.
    - Nombra las acciones que tienen lugar en cada interacción.
    - Implementa el juego comenzando por las funciones más sencillas.
    - Analiza puntos fuertes y débiles de la implementación.

## Dibujo

Antes de programar necesitamos saber qué queremos programar. En primer lugar,
imagina el juego que quieres crear. Decide cuántos jugadores participarán,
de dónde saldrán las palabras secretas, que se mostrará por pantalla, etc.

!!! info
    Comienza por la versión más sencilla que sepas implementar.
    Tendremos tiempo para refinar el juego más adelante.

Ahora coge un papel y un boli (o un lápiz). ¿Podrías explicar tu versión
del juego usando únicamente diagramas? Intenta dejar claro: 

* ¿Qué entidades participan?
* ¿Cómo se relacionan?
* ¿Qué datos manejan?
* ¿Qué entradas y salidas hay? ¿Usarás teclado, pantalla, ficheros...?

Te proponemos que lo expliques utilizando diagramas de
*secuencia*, de *estado* y de *flujo* como los de {{ codex_link("diagrams") }}.

## Intercambio de idiomas

Elegir las palabras correctas nos ayuda a pensar mejor,
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

4. En el `main()` de `main.cpp`, ejecuta la lógica del juego usando
   únicamente las funciones que hayas implementado en `hangman.cpp`.

Quizá te pueda ayudar:

* {{ codex_link("open_quest") }}
* {{ codex_link("add_file_quest") }}
* {{ codex_link("run_quest") }}
* {{ codex_link("cin_cout") }}

## Debate

* Si publicas `hangman.h` y `hangman.cpp` para que puedan jugar otra persona,
  ¿cuántas funciones y structs necesita aprender a usar esa persona?

* Si queremos elegir la palabra secreta de manera automática.
  ¿De cuántas maneras podríamos hacerlo? ¿Entre cuántas palabras podríamos
  elegir como mucho?

# Tags

despegue:2
semana:0
time:90