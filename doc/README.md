# Documentación de herramientas (`doc/`)

Este directorio contiene scripts y configuraciones para la generación (semi)automática de documentación
para el proyecto SOL.

## Herramientas disponibles

- `tag.py`: utilidades para leer `README.md` de cada quest y extraer metadatos (`título`, `descripción`, `tags`).
- `autoindex.py`: genera automáticamente `quest/README.md` a partir de los quests detectados.
  - Construye secciones por categorías y por quests.
  - Soporta filtros (`--tag`) y exclusiones (`--ignore`).

## Archivos de configuración y datos

- `categories.md`: definición de categorías y su descripción.
  - Formato: cada categoría como sección `# nombre_categoría`.
  - El contenido bajo cada sección es su descripción (puede quedar vacío).
  - Si se mueve o renombra un tag en los README de un módulo en `quest/*`,
    conviene revisar también `doc/categories.md`.
- `img/`: recursos gráficos usados por documentación y README.

## Flujo recomendado

1. Editar o crear quests en `quest/<módulo>/README.md`.
2. Ajustar descripciones de categorías en `doc/categories.md` si hace falta.
3. Regenerar índice con `./doc/autoindex.py`.
4. Revisar salida en `quest/README.md`.

