# Guía de estudio

Campañas de quests y documentos de referencia.

## Compilación

- `make`: compilado estático
- `make serve`: servicio dinámico

### Requisitos previos

sudo apt install python3 python3-pip make

pip install mkdocs mkdocs-material pymdown-extensions mkdocs-minify-plugin mkdocs-macros-plugin clang

## Añadir contenido

### Nuevos apuntes (códice)

* Añadir documento markdown en `doc/guide/sections/<name>.md`
* Añadir `- <name>.md` en `doc/guide/mkdocs.yml`, bajo `Codex:`

### Nuevas campañas

* Añadir título y descripción de la nueva tag en `doc/tag/<tag>.md`.
* Añadir `- <title> : <tag>.md` en `doc/guide/mkdocs.yml`, bajo `Campañas: `

### Nuevos quests a una campaña existente

* Añadir quests directamente con las tags apropiadas (`name:<index>`).

### Compilar guía

Generada automáticamente al hacer commit. 

Se puede compilar manualmente, desde la raíz del proyecto:

```bash
cd doc && make
```
