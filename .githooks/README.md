# Hooks de Git

| Hook | Ejecuta |
| --- | --- |
| pre-commit, pre-push | política de ramas (ver abajo) |
| commit-msg | asunto en Conventional Commits, en inglés y en ASCII simple |
| pre-commit | búsqueda de secretos y comprobación de `clang-format` en los archivos C/C++ en el área de preparación (requiere `.clang-format`) |
| pre-push | `make build`, `make lint`, `make test` (solo los targets que existan) |

## Política de ramas

Solo se permiten `main`, `develop` y las ramas de issue creadas desde `develop`. Las ramas de issue usan el nombre que GitHub genera a partir del issue: `<número-del-issue>-<slug>` (por ejemplo `12-add-lexer`). Créalas con `gh issue develop <n>` o desde la página del issue. Una vez que existe `develop`, una rama de issue debe descender de ella.

Los tests, el lint y la compilación se ejecutan solo en pre-push, nunca en CI.

Actívalos una vez por clon:

```bash
git config core.hooksPath .githooks
```
