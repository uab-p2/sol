# Entre gatos

El objetivo de este quest es familiarizarte con la mentalidad felina
a través del uso de *clases*, también conocido como *programación orientada a objetos (OOP)*.
Te ofrecemos un puñado de preguntas guiadas, tras las cuales te proponemos un reto final.

## Preguntas guiadas

Descarga el proyecto SOL y abre el quest en la carpeta `quest/entre_gatos`.

Revisa el quest [Silicio y titanio](quest_silicio_y_titanio.md) si necesitas ayuda para
obtener el código o configurar tu entorno de desarrollo.

1. Ejecuta la demo proporcionada y estudia su salida.
    - ¿En qué estados puede estar un gato?
    - ¿Qué acciones afectan el estado de un gato?
    - Dibuja el diagrama de estados de un gato.

2. Estudia el contenido de `main.cpp`.
    - ¿Qué elementos del código conocías ya?
    - ¿Qué partes no conoces todavía?
    - ¿Hay alguna parte repetitiva que puedas mejorar?

3. Juega con `main.cpp`.
    - Crea otro gato llamado "Gatélite".
    - ¿Se comporta Gatélite igual que Nyan?
    - Predice: ¿es posible tener un gato satisfecho (ni hambriendo ni somnoliento)?
    - Predice: ¿es posible crear un gato sin nombre?

4. Estudia el contenido de `src/cat.h` y `src/cat.cpp`.
    - ¿Qué hay en el `.h` que no hay en el `.cpp`?
    - ¿Qué hay en el `.cpp` que no hay en el `.h`?
    - Corrobora o desmiente las dos predicciones del punto anterior.
    - ¿Es posible conocer el estado del gato sin jugar ni darle comida?

5. Reflexiona sobre lo que has observado:
    - Haz una tabla resumen con (a) la nueva sintaxis aprendida, y (b) para qué sirve.
    - ¿Hay algún aspecto todavía misterioso?

{{ snippet_box("Cat", default_open=False) }}

## Reto final

Casi todos los gatos prefieren tener una persona ~~esclava~~ cuidadora.
¿Puedes crear una clase `Caregiver` que interactúe con la clase `Cat`?

Antes de programar nada:

* Crea un diagrama de estados de `Caregiver`.
* Dibuja un diagrama de carriles (swimlane) con el guión de tu demo.

Para la implementación:

* Crea al menos `caregiver.h`, `caregiver.cpp` y define una nueva clase `Caregiver`.
* Modifica `main.cpp` para que siga tu guión.
* No modifiques ningún fichero de `../src` (en ningún quest).

---
- [Código](https://github.com/uab-p2/sol/tree/main//quest/entre_gatos)
- [&leftarrow;&nbsp;Objetos celestes](campaign_oop.md)
