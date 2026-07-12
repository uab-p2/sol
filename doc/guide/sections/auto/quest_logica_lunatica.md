# Lógica lunática

A veces nuestro código parece desafiar la razón: la ejecución del programa
no parece consistente con el código que hemos escrito. Te retamos a resolver 
un caso monstruoso de lógica lunática. No te asustes, también te decimos dónde 
está el desinfectante.

{{ img_badge("astronaut_ant.png") }}

## Bichos de colores

Tienes ante ti tres oponentes en traje espacial idéntico salvo por el casco.

???quote "El del casco rojo te dice:"
        Hi, I'm Red! I like Earth's nature.
        I know 'eagle' is a bird.
        I know 'kangaroo' is a bird.

???quote "El del casco verde te dice:"
        Hi, I'm Green! I like Earth's money.
        If the minimum grade is 5, then you pass if you get a 6.5
        If the minimum grade is 5, then you pass if you get a 3.5
        If you paid me 10 euros every day for 30 days, I would get 300 euros in total.
        If you paid me 10 euros every day for 10 days, I would get 1827553899 euros in total.
        Maybe you can bribe me with 10 euros for 60 days instead?


???quote "El del casco azul te dice:"
        Hi, I'm Blue! I like Earth's function g.
        If you ask me about g(10), the answer is yes
        If you ask me about g(11), the answer is yes
        Do you agree?

Notas algo raro en lo que dicen? No pierdas la cordura y trata de restaurar 
la lógica de sus afirmaciones.

Si no lo has hecho ya, abre el quest `logica_lunatica` en tu IDE
siguiendo {{ quest_link("silicio_y_titanio") }}.

Si te atascas o quieres ampliar tus habilidades de desinfección, 
tienes {{ codex_link("debugging") }}.

## Mr. Red

* ¿Qué tiene de raro lo que dice Mr. Red?
* ¿Qué funciones están involucradas en este caso?
* ¿Hay algún problema en `is_a_bird`?
* ¿Serviría con cambiar `!not_a_bird` por `not_a_bird` en el segundo if?
* ¿Qué herramientas tienes para encontrar y solucionar este error?
* ¿Qué hábitos le hubieran permitido detectar y solucionar este error a Mr. Red?

{{ snippet_box("is_a_bird") }}

## Mr. Green

Parece que Mr. Green no tiene muy claro lo que es un aprobado.

* ¿Qué problemas tiene esta parte de su lógica?
* ¿Nos había avisado el compilador?

También parece que Mr. Green tiene problemas de contabilidad.

* ¿Por qué hace bien el cálculo para 30 días pero no para 10?
* ¿Nos había avisado el compilador?
* ¿Qué herramientas tienes para inspeccionar el funcionamiento de `propose_bribe`?

Por último, acepta el reto de Mr. Green e intenta sobornarlo 
con 10 euros durante 60 meses.

* ¿Se deja sobornar?
* ¿Sabrías explicar qué está pasando?
* ¿Hay algún hábito que le hubiera podido ahorrar el error a Mr. Green?

{{ snippet_box("check_passing_grade") }}

{{ snippet_box("propose_bribe") }}

## Mr. Blue

No sé si me fio de la lógica de Mr. Blue. 

* ¿Qué intenta hacer su misteriosa función `g`?
* ¿Lo hace correctamente?
* ¿Qué herramientas te pueden ayudar con este caso?
* ¿Qué habitos le hubieran ahorrado este error a Mr. Blue?
* ¿Cuál de todos los errores te ha llevado más tiempo solucionar?

{{ snippet_box("g") }}

<br/><br/>
[&rightarrow; Repositorio](https://github.com/uab-p2/sol/tree/main//quest/logica_lunatica)
