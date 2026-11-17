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

- `make build` compila el intérprete en `build/braille3d`.
- `make lint` compila cada archivo con las advertencias tratadas como errores.
- `make format-check` comprueba el estilo definido en `.clang-format`.
- `make test` compila y ejecuta cada `test/*.test.cpp`.

El hook `pre-push` ejecuta `make build`, `make lint` y `make test`.

Los releases se automatizan con release-please (`.github/workflows/release-please.yml`), y `develop` se mantiene al día con `main` después de cada release (`.github/workflows/sync-develop-from-main.yml`). Ver `docs/releasing.md`.

## Cómo se agregan las siguientes etapas

Hoy solo existe el lexer (`include/braille3d/lexer.hpp`, `src/lexer.cpp`) y `src/main.cpp`. Ninguno de los módulos siguientes existe todavía; esta sección es un plan, no código.

- Cada etapa es un par `include/braille3d/<módulo>.hpp` y `src/<módulo>.cpp`.
- La salida del lexer, un `Designar::DynArray<Designar::Token>`, es la entrada del parser: `parse()` de `llparser.hpp` recibe un `DynArray<Token>`.
- `Diagnostic` y `DiagnosticSink` (futuros): errores y registros con un formato normalizado, que todas las etapas reportan al mismo lugar.
- `Source` (futuro): carga el archivo y convierte desplazamientos en bytes a línea y columna, porque DeSiGNAR informa las posiciones como desplazamientos en bytes.
- `Interpreter` (futuro): clase que ejecuta las etapas en orden y reporta al sink.
- `main.cpp` solo carga el archivo (por defecto `examples/health-center.b3d`), crea el `Interpreter` e imprime los diagnósticos.

## Puntos abiertos

- La especificación del lenguaje: sintaxis y significado de braille3d.
- Si el lexer y el parser usan el módulo de compiladores de DeSiGNAR o se escriben a mano.
- Las etapas posteriores al parser.
