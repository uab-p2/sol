# Documentación de herramientas (`doc/`)

Este directorio contiene scripts y configuraciones para la generación (semi)automática de documentación
para el proyecto SOL.

## Estructura

- `tag/`: definición de títulos y descripciones de tags (categorías, campañas).
- `img/`: recursos gráficos usados por documentación y README.
- `guide/`: guía de estudio (compilada en html automáticamente).
- `tool/`: scripts internos para facilitar la generación de documentación.
  - `link_luna.py`: enlaza las carpetas `gradescope`, `solution` y `template` del proyecto LUNA en la raíz de este proyecto.

## Configuración inicial: enlazar el proyecto LUNA

Antes de compilar la documentación, es necesario enlazar el proyecto LUNA ejecutando:

```bash
python3 doc/tool/link_luna.py /ruta/al/proyecto/luna
```

## Compilación

Ejecutar `make` en este directorio para regenerar la documentación.

