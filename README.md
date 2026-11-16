# Intérprete de braille3d-lang

Un intérprete en C++ para el lenguaje braille3d, desarrollado para la cátedra universitaria de Compiladores.

## Sobre el proyecto

braille3d es un lenguaje para describir placas con texto y relieve braille listas para impresión 3D. El intérprete se construye por etapas, y cada una se entrega como una fase. La arquitectura general está en [`docs/architecture.md`](docs/architecture.md).

## Fases de entrega

### Fase 1: analizador léxico

**Fecha de entrega:** 16 de noviembre de 2026

El analizador léxico es la primera etapa del intérprete. Lee el texto de un archivo `.b3d` y lo convierte en una secuencia de tokens, que es la entrada del analizador sintáctico. Está construido con la clase `Designar::Lexer` de la biblioteca [DeSiGNAR](https://github.com/R3mmurd/DeSiGNAR).

**Qué reconoce**

- 23 palabras reservadas (`PLATE`, `BRAILLE`, `TEXT`, `CONTENT`, `TEMPLATE`, `USE`, `WIDTH`, ...) y la unidad `mm`.
- Identificadores, números enteros y decimales, y cadenas de texto con tildes y `ñ`.
- El operador `=` y los delimitadores `{ } ( ) , ;`.
- Comentarios de una línea con `#` y espacios en blanco, que se descartan antes de entregar los tokens.

**Cómo funciona**

La clase `LexerB3D` registra todas las reglas en su constructor. `tokenize()` devuelve los tokens sin espacios ni comentarios, y `tokenize_raw()` devuelve todos, para depuración. `Designar::Lexer` consume en cada posición el lexema más largo que acepte alguna regla y, si dos reglas empatan, gana la registrada primero. Por eso las palabras reservadas se registran antes que los identificadores. DeSiGNAR no tiene clases de caracteres como `[a-z]` ni negaciones como `[^"]`, así que los rangos se escriben como alternaciones explícitas.

**Estructura**

| Archivo | Contenido |
| --- | --- |
| [`include/braille3d/lexer.hpp`](include/braille3d/lexer.hpp) | declaración de la clase `LexerB3D` |
| [`src/lexer.cpp`](src/lexer.cpp) | reglas de los tokens y modos de escaneo |
| [`src/main.cpp`](src/main.cpp) | programa que imprime los tokens de un archivo |
| [`examples/`](examples/README.md) | programas `.b3d` de prueba |

**Cómo probarlo**

```bash
make build
./build/braille3d examples/health-center.b3d
```

**Para más información**

- [Implementación del analizador léxico](docs/lexer.md): clase, reglas, resolución de conflictos y adaptación a DeSiGNAR.
- [Sintaxis del lenguaje](docs/syntax.md): cómo escribir un archivo `.b3d`.
- [Ejemplos](examples/README.md): qué prueba cada programa.

¿Primera vez aquí? Empieza con [`docs/getting-started.md`](docs/getting-started.md). Las reglas del proyecto están en [`CONTRIBUTING.md`](CONTRIBUTING.md).

## Requisitos previos

Instala estas herramientas **antes** de cualquier otro paso. Este README asume que ya están en tu máquina y no las instala.

| Herramienta | Versión | Para qué se necesita | Comprobación |
| --- | --- | --- | --- |
| Compilador de C++ | `g++` 9+ o `clang++` 14+ | compilar el intérprete y DeSiGNAR | `g++ --version` o `clang++ --version` |
| GNU `make` | cualquier versión reciente | compilar el intérprete | `make --version` |
| `git` | cualquier versión reciente | clonar DeSiGNAR | `git --version` |
| `clang-format` | 14 o superior (opcional) | `make format`, `make format-check` y el hook `pre-commit` | `clang-format --version` |
| CMake | 3.20+ | instalar DeSiGNAR, **una sola vez** | `cmake --version` |
| DeSiGNAR | 4.0.1 (commit `ab9099c`) | la biblioteca de estructuras de datos | ver [Instalar DeSiGNAR](#instalar-designar) |

CMake solo se usa para compilar e instalar DeSiGNAR. Si DeSiGNAR ya está instalado en tu máquina, no necesitas CMake. Obtén CMake con el gestor de paquetes de tu sistema o en <https://cmake.org/download/>.

### Instalar DeSiGNAR

[DeSiGNAR](https://github.com/R3mmurd/DeSiGNAR) es una biblioteca de estructuras de datos y algoritmos en C++17. Es un **requisito del sistema**: se instala una vez, fuera de este repositorio, y el proyecto solo necesita saber dónde está.

Compílala e instálala en un prefijo propio, por ejemplo `~/.local`:

```bash
git clone https://github.com/R3mmurd/DeSiGNAR.git
cd DeSiGNAR
git checkout ab9099c
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DDESIGNAR_BUILD_SAMPLES=OFF -DDESIGNAR_BUILD_TESTS=OFF
cmake --build build --parallel
cmake --install build --prefix ~/.local
```

Esto instala las cabeceras en `~/.local/include` y `libDesignar.a` en `~/.local/lib`.

### Compilador: g++ o clang++

El compilador verificado hasta ahora es `g++`. `clang++` está previsto, pero todavía no se ha probado.

Ambos compiladores están soportados. DeSiGNAR **debe compilarse con el mismo compilador y la misma biblioteca estándar que el intérprete**; mezclarlos falla al enlazar. Para usar clang, compila DeSiGNAR con él y usa un prefijo distinto:

```bash
CC=clang CXX=clang++ cmake -S . -B build-clang -DCMAKE_BUILD_TYPE=Release -DDESIGNAR_BUILD_SAMPLES=OFF -DDESIGNAR_BUILD_TESTS=OFF
cmake --build build-clang --parallel
cmake --install build-clang --prefix ~/.local/designar-clang
```

## Configurar la ruta de DeSiGNAR

La ubicación de la biblioteca se define en un solo lugar, `config.mk`:

| Variable | Valor por defecto | Significado |
| --- | --- | --- |
| `DESIGNAR_PREFIX` | `/usr/local` | prefijo de instalación |
| `DESIGNAR_INCLUDEDIR` | `$(DESIGNAR_PREFIX)/include` | directorio con las cabeceras |
| `DESIGNAR_LIBDIR` | `$(DESIGNAR_PREFIX)/lib` | directorio con `libDesignar.a` (usa `lib64` donde tu distribución lo requiera) |
| `CXX` | `g++` | compilador (`clang++` está previsto, aún sin probar) |

Define la ruta de tu máquina en `config.local.mk`, que no se versiona:

```bash
cp config.local.mk.example config.local.mk
```

Luego edita `DESIGNAR_PREFIX` en ese archivo. También puedes sobrescribir cualquier variable en un solo comando:

```bash
make test DESIGNAR_PREFIX=$HOME/.local
make test CXX=clang++ DESIGNAR_PREFIX=$HOME/.local/designar-clang
```

La biblioteca se incluye con `-isystem`, de modo que las advertencias dentro de las cabeceras de DeSiGNAR nunca rompen una compilación con `-Werror`.

## Verificar la configuración

```bash
make help          # lista los targets
make show-config   # muestra el compilador y las rutas en uso
make check-deps    # comprueba el compilador, las cabeceras de DeSiGNAR y libDesignar.a
make test          # compila y ejecuta cada test/*.test.cpp contra DeSiGNAR
```

Cada comprobación se detiene en el primer problema y lo nombra: el compilador, las cabeceras de DeSiGNAR, `libDesignar.a`, o un test que no compila (estándar incorrecto, o DeSiGNAR compilado con otro compilador).

Los tests viven en `test/` y se llaman `<nombre>.test.cpp`; cada uno es un programa que termina con código 0 cuando pasa. Cómo ejecutarlos, qué cubren y cómo añadir uno: [`docs/testing.md`](docs/testing.md). `test/prerequisites.test.cpp` es el test básico de requisitos: inserta y busca un valor en un `AVLTree` de DeSiGNAR, así que solo pasa si la biblioteca compila, enlaza y se ejecuta con tu compilador y tus rutas.

## Compilar y ejecutar

```bash
make build
./build/braille3d examples/health-center.b3d
```

El programa recibe como argumento el archivo `.b3d` y muestra sus tokens. Los detalles están en [`docs/lexer.md`](docs/lexer.md).

## Estructura del proyecto

| Ruta | Contenido |
| --- | --- |
| `src/` | archivos de implementación |
| `include/` | cabeceras del proyecto (`include/braille3d/` para los módulos) |
| `examples/` | programas `.b3d` de ejemplo |
| `test/` | tests, con el nombre `<nombre>.test.cpp` |
| `docs/` | documentación: [`getting-started.md`](docs/getting-started.md), [`architecture.md`](docs/architecture.md), [`lexer.md`](docs/lexer.md), [`syntax.md`](docs/syntax.md), [`testing.md`](docs/testing.md), [`releasing.md`](docs/releasing.md) |
| `CONTRIBUTING.md` | cómo contribuir: ramas, commits, comprobaciones y pull requests |
| `config.mk` | configuración del compilador y de DeSiGNAR |
| `Makefile` | targets de compilación, lint, formato y tests |
| `.github/` | workflows de release y su configuración, ver [`docs/releasing.md`](docs/releasing.md) |
| `.githooks/` | hooks de Git del proyecto, ver [`.githooks/README.md`](.githooks/README.md) |
| `.clang-format` | estilo del código (el de DeSiGNAR) |

## Targets de make

| Target | Qué hace |
| --- | --- |
| `make build` | compila el intérprete en `build/braille3d` |
| `make lint` | compila cada archivo con las advertencias como errores, sin generar archivos de salida |
| `make format` | da formato a cada archivo C++ en el sitio (requiere `clang-format`) |
| `make format-check` | comprueba el formato sin modificar archivos (requiere `clang-format`) |
| `make test` | compila y ejecuta cada `test/*.test.cpp` |
| `make check-deps` | comprueba el compilador y DeSiGNAR |
| `make clean` | elimina `build/` |

## Releases

Los releases se crean automáticamente a partir de Conventional Commits con release-please. La versión, las etiquetas y `CHANGELOG.md` se generan, y los workflows usan un token de acceso personal guardado en el secreto de repositorio `RELEASE_PLEASE_TOKEN`. Consulta [`docs/releasing.md`](docs/releasing.md) para el proceso, el token y las reglas.

## Licencia

Este proyecto se distribuye bajo los términos académicos de uso y derechos de autor detallados en [`LICENSE.md`](LICENSE.md).
