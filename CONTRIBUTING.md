# Cómo contribuir

Gracias por ayudar con el intérprete de braille3d. Esta página reúne las reglas del proyecto. Para preparar tu máquina primero, lee [`docs/getting-started.md`](docs/getting-started.md).

## Flujo de trabajo

1. Todo cambio parte de un issue. Abre uno si no existe.
2. Trabaja en una rama creada desde ese issue, a partir de `develop`.
3. Abre un pull request hacia `develop`.
4. Tras la revisión, el pull request se integra. Los releases son automáticos, ver [Releases](#releases).

No hagas commits directamente en `main` ni en `develop`.

## Ramas

Solo se permiten tres tipos de rama:

- `main`: código publicado.
- `develop`: la rama de integración.
- Ramas de issue creadas desde `develop`, con el nombre que genera GitHub: `<número-del-issue>-<slug>`, por ejemplo `12-add-lexer`.

La forma más sencilla de crearlas es desde el propio issue, así el nombre es correcto y la rama queda vinculada al issue.

Con la CLI de GitHub:

```bash
git switch develop && git pull
gh issue develop 12 --base develop --checkout
```

En el navegador: abre el issue, ve a Development, elige Create a branch, selecciona `develop` como origen y luego tráela a tu máquina:

```bash
git fetch
git switch <nombre-de-la-rama>
```

Los hooks rechazan cualquier otro nombre de rama al hacer commit o push, y exigen que una rama de issue descienda de `develop`.

## Commits

Los commits siguen [Conventional Commits](https://www.conventionalcommits.org/es/v1.0.0/), escritos en inglés:

```
<type>(<scope>): <short description>
```

- Tipos: `feat`, `fix`, `docs`, `style`, `refactor`, `perf`, `test`, `build`, `ci`, `chore`, `revert`.
- El alcance es opcional: `feat(lexer): add token kinds`.
- Añade `!` después del tipo para un cambio incompatible: `feat(parser)!: change the grammar entry point`.
- El asunto usa caracteres ASCII simples (sin tildes) y describe una sola idea.
- Mantén los commits pequeños. Cada commit debe compilar y pasar los tests.

Las notas de cada release se generan a partir de estos mensajes, así que escríbelos pensando en quien lea el changelog.

## Comprobaciones antes de hacer push

```bash
make format
make lint
make test
```

| Comando | Qué hace |
| --- | --- |
| `make format` | da formato a los archivos C++ con `clang-format` (estilo en `.clang-format`) |
| `make lint` | compila cada archivo con las advertencias como errores |
| `make test` | compila y ejecuta cada `test/*.test.cpp` |

Los hooks de Git en `.githooks/` ejecutan estas comprobaciones por ti:

| Hook | Ejecuta |
| --- | --- |
| `commit-msg` | comprueba el formato del mensaje del commit |
| `pre-commit` | busca secretos y comprueba el formato de los archivos C++ en el área de preparación |
| `pre-push` | ejecuta `make build`, `make lint` y `make test` y detiene el push si alguno falla |

Las comprobaciones se ejecutan en tu máquina y no en CI, así que los hooks deben estar activos: `git config core.hooksPath .githooks`.

## Estilo del código

- C++17. Las opciones del compilador están en `config.mk`.
- El formato lo define `.clang-format`, que sigue el estilo de DeSiGNAR: 4 espacios, llaves Allman, 80 columnas y contenido de los `namespace` con sangría. Ejecuta `make format` y no reformatees código que no cambiaste.
- El código, los nombres de archivos, los comentarios del código y la salida del programa se escriben en inglés. Los documentos Markdown (`.md`) se escriben en español.
- Las fuentes están en `src/`, las cabeceras en `include/` (los módulos en `include/braille3d/`), los ejemplos en `examples/`, los tests en `test/` y la documentación en `docs/`.
- No copies DeSiGNAR dentro del repositorio. Es un requisito del sistema y su ruta se define en `config.local.mk`.

## Tests

Los tests están en `test/` y se llaman `<nombre>.test.cpp`. Cada archivo es un programa que devuelve `0` cuando el test pasa y cualquier otro valor cuando falla. `make test` encuentra cada archivo con ese nombre, así que añadir un test no requiere cambiar el `Makefile`.

```cpp
int main()
{
    int sum = 1 + 1;
    return sum == 2 ? 0 : 1;
}
```

Un cambio que añade comportamiento debe añadir o actualizar un test. La guía completa, con lo que cubre cada prueba y cómo ejecutar una sola, está en [`docs/testing.md`](docs/testing.md).

## Pull requests

- Abre el pull request hacia `develop`.
- El título y la descripción se escriben en inglés. El título es corto y dice qué cambia.
- Enlaza el issue en la descripción, por ejemplo `Part of #12`.
- Explica qué cambió y cómo lo comprobaste (qué comandos de `make` ejecutaste).
- Mantenlo centrado en un solo issue. Si necesita más commits, no hay problema; si necesita más issues, divídelo.

## Releases

Los releases son automáticos y usan los mensajes de los commits. No edites a mano:

- `CHANGELOG.md`
- `.github/config/release/release-please-manifest.json`
- la línea de versión de `include/version.hpp` (tiene un comentario `x-release-please-version`; consérvalo)

El proceso está en [`docs/releasing.md`](docs/releasing.md).

## Preguntas

Abre un issue o pregunta en el pull request.
