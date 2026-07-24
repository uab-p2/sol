# Juego peligroso

El camino será largo. ¿Por qué no fabricarte algo de entretenimiento?
Te guiamos para montar un juego peligroso: el del ahorcado.

??? objetivo

    - Diseña y dibuja las entidades, datos e interacciones que describen el juego.
    - Nombra las acciones que tienen lugar en cada interacción.
    - Implementa el juego comenzando por las funciones más sencillas.

## Dibujo

Antes de programar necesitamos saber qué queremos programar. En primer lugar,
imagina el juego que quieres crear. Decide cuántos jugadores participarán,
de dónde saldrán las palabras secretas, que se mostrará por pantalla, etc.

Coge un papel. Ahora, coge un boli (o un lápiz). ¿Podrías explicar tu versión
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

¿Ya lo tienes todo preparado para comenzar a teclear? Te proponemos los siguientes pasos:

1. Abre `quest/juego_peligroso` y comprueba que compila sin problema.

2. Crea el fichero `hangman.h` en la raíz del quest.
   Crea aquí las *declaraciones* de *funciones* y *structs* con la nomenclatura del punto anterior
   antes de ponerte con la implementación.

3. Crea el fichero `hangman.cpp`, también en la raíz del quest.
   Una a una, implementa las funciones de hangman.h de la siguiente manera:

    1. Elige la función más sencilla pendiente de implementar.
    2. Crea una primera versión de esa función en `hangman.cpp`.
    3. Prueba esa función desde `test.cpp` hasta que funcione. No olvides el `#include "hangman.h"`.

4. En el `main()` de `main.cpp`, implementa la lógica del juego usando
   únicamente las funciones que hayas implementado en `hangman.cpp`.

!!! info
      En todos los quests puedes hacer `#include "file.h"` de todos los ficheros `*.h`
      en la carpeta del propio quest, y también de los ficheros en la carpeta
      `src/` del proyecto SOL.

Quizá te pueda ayudar:

* {{ codex_link("open_quest") }}
* {{ codex_link("add_file_quest") }}
* {{ codex_link("run_quest") }}
* {{ codex_link("cin_cout") }}

# Tags

despegue:2
semana:0
time:90