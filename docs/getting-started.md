# Primeros pasos

Esta guía te lleva de cero a un `make test` que pasa. Después, lee [`CONTRIBUTING.md`](../CONTRIBUTING.md) para conocer las reglas del proyecto.

## Camino rápido

Si ya tienes instalados un compilador de C++17, `make`, CMake, `git` y DeSiGNAR:

```bash
git clone https://github.com/EnderPuentes/ula-compilers-braille3d-lang.git
cd ula-compilers-braille3d-lang
git config core.hooksPath .githooks
cp config.local.mk.example config.local.mk   # luego define DESIGNAR_PREFIX en ese archivo
make test
```

Si `make test` muestra `test: all tests passed`, estás listo. Si no, sigue los pasos de abajo.

## 1. Instalar las herramientas

Necesitas un compilador de C++ (`g++` 9+), GNU `make`, `git` y CMake 3.20+. Compruébalos:

```bash
g++ --version
make --version
git --version
cmake --version
```

`clang-format` es opcional pero recomendado. Comprueba y corrige el estilo del código:

```bash
clang-format --version
```

Para abrir pull requests desde la terminal también puedes usar la CLI de GitHub (`gh`). Es opcional: todo se puede hacer desde el navegador.

## 2. Instalar DeSiGNAR

El intérprete usa [DeSiGNAR](https://github.com/R3mmurd/DeSiGNAR), una biblioteca de estructuras de datos en C++17. Se instala una vez en tu máquina, fuera de este repositorio. Los comandos están en el README, sección [Instalar DeSiGNAR](../README.md#instalar-designar).

Al terminar, recuerda el prefijo que usaste, por ejemplo `~/.local`.

## 3. Obtener el código

```bash
git clone https://github.com/EnderPuentes/ula-compilers-braille3d-lang.git
cd ula-compilers-braille3d-lang
```

Luego activa los hooks del proyecto. Comprueban tus commits y los nombres de tus ramas, y ejecutan las comprobaciones antes de un push. Hazlo una vez por clon:

```bash
git config core.hooksPath .githooks
```

## 4. Indicar al proyecto dónde está DeSiGNAR

```bash
cp config.local.mk.example config.local.mk
```

Abre `config.local.mk` y define `DESIGNAR_PREFIX` con el prefijo del paso 2. Este archivo no se versiona, así que tu ruta se queda en tu máquina. Hay más opciones en el README, sección [Configurar la ruta de DeSiGNAR](../README.md#configurar-la-ruta-de-designar).

## 5. Comprobar que todo funciona

```bash
make check-deps
make test
```

`make check-deps` se detiene en la primera pieza que falta y la nombra. `make test` compila y ejecuta cada test de `test/`.

## 6. Compilar y ejecutar

```bash
make build
./build/braille3d examples/health-center.b3d
```

## Tu primer cambio

1. Elige un issue de la [lista de issues](https://github.com/EnderPuentes/ula-compilers-braille3d-lang/issues). Pide que te lo asignen si alguien más puede estar trabajando en él.
2. Crea la rama desde el issue (ver [CONTRIBUTING.md](../CONTRIBUTING.md#ramas)) y cámbiate a ella.
3. Haz tu cambio en pasos pequeños. Antes de cada commit, ejecuta:

   ```bash
   make format
   make lint
   make test
   ```

4. Haz el commit con un mensaje [Conventional Commit](../CONTRIBUTING.md#commits) en inglés, por ejemplo `feat(lexer): add token kinds`.
5. Sube la rama con push y abre un pull request hacia `develop`.

## Cuando algo falla

Los mensajes de error de las herramientas se muestran tal como aparecen, en inglés.

| Mensaje | Qué significa | Solución |
| --- | --- | --- |
| `compiler 'g++' not found` | el compilador no está instalado o no está en el `PATH` | instálalo |
| `DeSiGNAR headers not found in ...` | `DESIGNAR_PREFIX` apunta a un lugar incorrecto | revisa `config.local.mk` |
| `libDesignar.a not found in ...` | la biblioteca está en `lib64` o en otra carpeta | define `DESIGNAR_LIBDIR` en `config.local.mk` |
| `did not build with ...` | DeSiGNAR se compiló con otro compilador | vuelve a compilar DeSiGNAR con el mismo compilador |
| `'clang-format' not found` | el formateador no está instalado | instálalo, o no uses `make format` ni `make format-check` |
| `commit-msg: subject must match ...` | el mensaje del commit no sigue Conventional Commits | corrige el asunto, ver [Commits](../CONTRIBUTING.md#commits) |
| `branch-policy: '...' is not allowed` | el nombre de la rama no es `<número-del-issue>-<slug>` | crea la rama desde el issue |
| `pre-push: ... failed; push aborted` | falló `make build`, `make lint` o `make test` | corrige el error que aparece sobre el mensaje |
