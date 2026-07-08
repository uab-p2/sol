# Guía de estudio P2

La guía de estudio de P2 combina dos elementos principales:

* Un "códice" de apuntes (chuletas, referencias, explicaciones detallades, etc.)
  sobre los diferentes contenidos de la asignatura.
* Una serie de "quests", agrupados en campañas, que plantean retos de programación
  en torno a esos contenidos.

El sistema usa los contenidos de `doc/` y de `quest/` del
[proyecto sol](https://github.com/uab-p2/sol) (principalmente los ficheros `README.md`)
para generar automáticamente una [interfaz web](https://p2-uab.github.io/sol).

Las siguientes secciones explican cómo añadir contenidos (apuntes y retos),
y cómo compilar y publicar actualizaciones.

## Añadir contenido

### Nuevos apuntes (códice)

Para añadir nuevas secciones al códice de apuntes:

* Añadir documento markdown en `doc/guide/sections/<name>.md`
* Añadir `- <name>.md` en `doc/guide/mkdocs.yml`, bajo `Codex:`

### Nuevos quests (retos)

Para crear un nuevo quest:

* Copiar la carpeta `template/quest_tempalte` del
  [proyecto luna](https://github.com/uab-p2/luna) en `quest/<nuevo_quest>` del
  proyecto sol.
* Editar `quest/<nuevo_quest>/README.md` con las instrucciones, modificar
  `main.cpp` y añadir los `*.h` y `*.cpp` necesarios.
* Incluir las categorías a las que pertenece el quest en `quest/<nuevo_quest>/README.md`,
  sección `Tags`. Para hacerlo parte de una secuencia de quests (una campaña),
  incluir `<tag>:<index>` en la lista de tags.

Para crear una solución a un quest.

* Copiar la carpeta `template/quest_template` del
  [proyecto luna](https://github.com/uab-p2/luna) en `solution/solution_<nuevo_quest>` del
  proyecto luna o, si es una solución pública, en `quest/solution_<nuevo_quest>` del
  proyecto sol.
* Modificar `main.cpp` y añadir el resto del código de la solución.
* Opcionalmente, editar el `test.cpp` de la nueva carpeta si se quieren crear
  un coderunner para gradescope. Ver `gradescope/README.md` del
  [proyecto luna](https://github.com/uab-p2/luna) para más información sobre
  cómo generar y publicar coderunners.

### Nuevas campañas

* Añadir título y descripción de la nueva tag en `doc/tag/<tag>.md`.
* Añadir `- <title> : <tag>.md` en `doc/guide/mkdocs.yml`, bajo `Campañas: `
* Añadir quests directamente con las tags apropiadas (`<tag>:<index>`).

## Compilar guía

Generada automáticamente y publicada en `github.io` al hacer push.

También se puede compilar y acceder localmente con `make serve`.

Requisitos para la compilación:

```bash
sudo apt install python3 python3-pip make

pip install mkdocs mkdocs-material pymdown-extensions mkdocs-minify-plugin mkdocs-macros-plugin clang libclang
```
