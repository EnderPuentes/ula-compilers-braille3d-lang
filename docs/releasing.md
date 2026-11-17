# Releases

Los releases se automatizan con [release-please](https://github.com/googleapis/release-please). Lee los Conventional Commits de `main`, elige la siguiente versión y escribe el changelog.

## Cómo funciona

1. El trabajo se integra en `develop`, y `develop` se integra en `main`. Usa un merge commit y no un squash, para que cada Conventional Commit llegue a `main`.
2. Cuando llegan commits a `main`, el workflow `release-please.yml` abre o actualiza un pull request llamado `chore(release): vX.Y.Z`. **El release no se publica solo**: queda pendiente hasta que alguien integra ese pull request.
3. Ese pull request actualiza `CHANGELOG.md`, el manifiesto de versión y la versión en `include/version.hpp`.
4. Al integrarlo se crea la etiqueta `vX.Y.Z` y el release de GitHub con las notas.
5. El workflow `sync-develop-from-main.yml` integra `main` de vuelta en `develop`, para que `develop` también tenga los cambios del release.

## Versionado

La versión empezó en `0.0.0`. Sin una etiqueta previa, release-please propuso `1.0.0` y ese fue el primer release.

| Commit | Cambio de versión |
| --- | --- |
| `fix: ...` | parche (`1.0.0` a `1.0.1`) |
| `feat: ...` | menor (`1.0.0` a `1.1.0`) |
| `feat!: ...` o un pie `BREAKING CHANGE:` | mayor (`1.0.0` a `2.0.0`) |

La versión la deciden `feat`, `fix` y los cambios incompatibles. Los demás tipos solo aparecen en el changelog.

## Changelog

`CHANGELOG.md` se genera. Cada release agrupa sus commits por tipo:

| Tipo | Sección | Se muestra |
| --- | --- | --- |
| `feat` | Features | sí |
| `fix` | Bug Fixes | sí |
| `perf` | Performance Improvements | sí |
| `revert` | Reverts | sí |
| `refactor` | Code Refactoring | sí |
| `docs` | Documentation | sí |
| `build` | Build System | sí |
| `style` | Styles | sí |
| `chore` | Miscellaneous Chores | sí |
| `test` | Tests | no |
| `ci` | Continuous Integration | no |

Escribe asuntos de commit claros: se convierten en las líneas del changelog.

## El token de release

Ambos workflows usan un token de acceso personal guardado en el secreto de repositorio `RELEASE_PLEASE_TOKEN`. Con él, los pull requests y las etiquetas que crea release-please pueden iniciar otros workflows, y el workflow de sincronización puede hacer push a `develop`. Si falta el secreto, ambos workflows fallan con un mensaje que lo nombra.

Crea el token y el secreto una sola vez:

1. En GitHub, ve a Settings, Developer settings, Personal access tokens, Fine-grained tokens.
2. Crea un token solo para este repositorio, con estos permisos de repositorio: Contents (lectura y escritura), Pull requests (lectura y escritura), Issues (lectura y escritura) y Metadata (lectura).
3. Guárdalo como secreto. El comando pide el valor, así que no queda en el historial de tu shell:

```bash
gh secret set RELEASE_PLEASE_TOKEN --repo EnderPuentes/ula-compilers-braille3d-lang
```

Mantén el token en privado y renuévalo antes de que caduque. Si caduca, los workflows fallan hasta que se reemplace el secreto.

## Reglas

- No edites a mano `CHANGELOG.md`, `.github/config/release/release-please-manifest.json` ni la versión de `include/version.hpp`. Pertenecen al pull request de release.
- Conserva el comentario `x-release-please-version` en la línea de versión de `include/version.hpp`. Le indica a release-please qué línea actualizar.
- Usa solo las opciones que existen en el esquema de release-please. Una clave desconocida se ignora sin ninguna advertencia. Comprueba la configuración contra `https://raw.githubusercontent.com/googleapis/release-please/main/schemas/config.json` después de cada cambio.
- Los workflows solo publican y sincronizan. Los tests, el lint y las comprobaciones de formato se ejecutan en el hook `pre-push`, nunca en CI.

## Archivos

| Archivo | Propósito |
| --- | --- |
| `.github/workflows/release-please.yml` | abre el pull request de release y crea el release |
| `.github/workflows/sync-develop-from-main.yml` | integra `main` en `develop` después de un release |
| `.github/config/release/release-please.config.json` | tipo de release, formato de las etiquetas, secciones del changelog y archivos a actualizar |
| `.github/config/release/release-please-manifest.json` | la versión publicada actual |
