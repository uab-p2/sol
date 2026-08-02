# Mirando estrellas

El sistema de navegación del proyecto SOL usa la versión moderna
del método más antiguo: las estrellas. Te acompañamos en el manejo
básico de clases y objetos (celestes, en este caso).

{{ img_badge("observation.png") }}

??? objetivo
    - Estudia la clase `Star` y el concepto y sintaxis de clase y objeto.
    - Manipula la clase `Star` y descubre el concepto de encapsulamiento.
    - Implementa dos funciones que usan y crean objetos de la clase `Star`.

## Telescopio

¿Por qué y para qué usar clases en programación?
¿Cuál es la diferencia entre una clase y un objeto?
¿Cómo se crea una clase? ¿Cómo se maneja un objeto?
Descifrémoslo mirando una estrella.

La clase `Star` se define en `src/star.h` y `src/star.cpp`.
Un ejemplo de uso de objetos de esta clase se muestra en el `main.cpp`
de este quest (`quest/mirando_estrellas`). 

1. Lista los elementos conocidos y desconocidos en estos ficheros
   (añade ejemplos del código como "star.h:13").

2. ¿Qué hay en `star.h` que no hay en `star.cpp`?

3. ¿Qué hay en `main.cpp` que no hay ni en `star.h` ni en `star.cpp`?

4. Deduce la sintaxis para:

    - Definir una nueva clase.
    - Crear un objeto de una clase.
    - Invocar los métodos de una clase

5. Propón una definición para "clase" y "objeto" que se ajuste
   a tus observaciones.

{{ snippet_box("star.h") }}

{{ snippet_box("star.cpp") }}

{{ snippet_box("main.cpp") }}

Quizá te pueda ayudar:

* {{ codex_links("class_why", "class_syntax") }}

## Eclipse

Si no lo has hecho ya, abre el quest `mirando_estrellas` en tu IDE. 
Juega con el contenido del `main.cpp` del quest 
(pero sin modificar `src/star.h` ni `src/star.cpp`)
y trata de responder a lo siguiente:

1. ¿Es posible crear estrellas de cualquier tipo ("yellow", "red", etc.)?
2. Una vez creada, 
   * ¿Es posible cambiar el tipo de una estrella?
   * ¿Es posible renombrar una estrella a "SOL"?
3. ¿Es posible cambiar la longitud de onda (`wavelelength`) de una estrella 
   a 150 nm? ¿Y a 145 nm?
4. El mecanismo que regula estas respuestas se llama *encapsulamiento*.

   * ¿Qué reglas rigen este mecanismo?
   * ¿Para qué puede ser útil? 

Quizá te pueda ayudar:

* {{ codex_links("open_quest", "class_encapsulation") }}

## Constelación

Tres estrellas forman un triángulo en el espacio.
¿Cuantos años luz de longitud tienen sus lados en total?

1. Implementa la función {{ snippet_ref("triangle_length") }}
   en `star_util.h/cpp` en tu copia de trabajo del quest
   que resuelva este problema.

2. Completa el `main.cpp` de tu quest de manera que confirme
   el funcionamiento correcto de la función.

## Supernova

¿Qué pasa si dos estrellas colisionan? Te pedimos una respuesta
netamente original en {{ snippet_ref("smash") }}. Las únicas condiciones son:

1. `smash` se ha de implementar en `star_util.h/cpp`.
2. `smash` ha de mover las estrellas `s1` y `s2` (los dos argumentos)
    al origen `(0,0,0)`.
3. El resultado de `smash` ha de ser una estrella. Sus atributos deben
   depender de los atributos de `s1` y `s2` de una manera única que elijas.
  

# Tags
time:60