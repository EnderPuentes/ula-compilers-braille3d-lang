# Arquitectura

Este documento describe cómo está organizado el intérprete. Solo recoge lo que ya está decidido. Los puntos abiertos se listan al final.

## Objetivo

El intérprete lee un programa escrito en el lenguaje braille3d y lo ejecuta. Es un proyecto universitario de la cátedra de Compiladores, escrito en C++17.

## Estructura de directorios

| Directorio | Contenido |
| --- | --- |
| `src/` | archivos de implementación (`.cpp`), en un solo nivel: un archivo por módulo |
| `include/` | cabeceras del proyecto (`.hpp`); los módulos están en `include/braille3d/` |
| `examples/` | programas `.b3d` de ejemplo |
| `test/` | tests, con el nombre `<nombre>.test.cpp` |
| `docs/` | documentación del proyecto |
| `build/` | resultado de la compilación (no se versiona) |

Los archivos que configuran la compilación y las herramientas están en la raíz: `Makefile`, `config.mk`, `.clang-format`.

El diseño sigue el de DeSiGNAR: cada módulo es un par `include/braille3d/<módulo>.hpp` y `src/<módulo>.cpp`. Las cabeceras viven en el subdirectorio `braille3d/` para que nunca se confundan con las de DeSiGNAR que tienen el mismo nombre (`lexer.hpp`, `parser.hpp`, `grammar.hpp`).

## Pipeline

El intérprete se construye por etapas. Cada etapa tiene su propio issue:

1. **Lexer** (issue #2): lee el texto fuente y produce tokens.
2. **Parser** (issue #3): lee los tokens y comprueba la gramática.

Las etapas posteriores al parser se definen cuando se escriba la especificación del lenguaje.

## Dependencias

El intérprete usa [DeSiGNAR](https://github.com/R3mmurd/DeSiGNAR), una biblioteca de estructuras de datos y algoritmos en C++17. Es un requisito del sistema y no se copia en este repositorio. Consulta el README para ver cómo instalarla y cómo indicar su ruta.

## Compilación y calidad

Los targets de `make` están en el [README](../README.md#targets-de-make). El hook `pre-push` ejecuta `make build`, `make lint` y `make test`. Los releases se automatizan con release-please; ver [`releasing.md`](releasing.md).

## Puntos abiertos

- La especificación del lenguaje: sintaxis y significado de braille3d.
- Si el parser usa el módulo de compiladores de DeSiGNAR (`llparser.hpp`) o se escribe a mano. El lexer ya usa `Designar::Lexer`.
- Las etapas posteriores al parser.
