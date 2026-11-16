# Releasing

Releases are automated with [release-please](https://github.com/googleapis/release-please). It reads the Conventional Commits on `main`, chooses the next version and writes the changelog.

## How it works

1. Work is merged into `develop`, and `develop` is merged into `main`. Use a merge commit, not a squash, so every Conventional Commit reaches `main`.
2. When commits reach `main`, the workflow `release-please.yml` opens or updates a pull request named `chore(release): vX.Y.Z`.
3. That pull request updates `CHANGELOG.md`, the version manifest and the version in `include/version.hpp`.
4. Merging it creates the tag `vX.Y.Z` and the GitHub release with the notes.
5. The workflow `sync-develop-from-main.yml` merges `main` back into `develop`, so `develop` also has the release changes.

## Versioning

The version starts at `0.0.0`. The first release is expected to be `0.1.0`.

| Commit | Version change |
| --- | --- |
| `fix: ...` | patch (`0.1.0` to `0.1.1`) |
| `feat: ...` | minor (`0.1.0` to `0.2.0`) |
| `feat!: ...` or a `BREAKING CHANGE:` footer | major, but minor while the version is below `1.0.0` |

The version is decided by `feat`, `fix` and breaking changes. The other types only appear in the changelog.

## Changelog

`CHANGELOG.md` is generated. Each release groups its commits by type:

| Type | Section | Shown |
| --- | --- | --- |
| `feat` | Features | yes |
| `fix` | Bug Fixes | yes |
| `perf` | Performance Improvements | yes |
| `revert` | Reverts | yes |
| `refactor` | Code Refactoring | yes |
| `docs` | Documentation | yes |
| `build` | Build System | yes |
| `style` | Styles | yes |
| `chore` | Miscellaneous Chores | yes |
| `test` | Tests | no |
| `ci` | Continuous Integration | no |

Write clear commit subjects: they become the lines of the changelog.

## The release token

Both workflows use a personal access token stored in the repository secret `RELEASE_PLEASE_TOKEN`. With it, the pull requests and tags that release-please creates can start other workflows, and the sync workflow can push to `develop`. If the secret is missing, both workflows fail with a message that names it.

Create the token and the secret once:

1. In GitHub, go to Settings, Developer settings, Personal access tokens, Fine-grained tokens.
2. Create a token for this repository only, with these repository permissions: Contents (read and write), Pull requests (read and write), Issues (read and write) and Metadata (read).
3. Store it as the secret. The command asks for the value, so it is not saved in your shell history:

```bash
gh secret set RELEASE_PLEASE_TOKEN --repo EnderPuentes/ula-compilers-braille3d-lang
```

Keep the token private and renew it before it expires. If it expires, the workflows fail until the secret is replaced.

## Rules

- Do not edit `CHANGELOG.md`, `.github/config/release/release-please-manifest.json` or the version in `include/version.hpp` by hand. The release pull request owns them.
- Keep the comment `x-release-please-version` on the version line of `include/version.hpp`. It tells release-please which line to update.
- Use only the options that exist in the release-please schema. An unknown key is ignored without any warning. Check the configuration against `https://raw.githubusercontent.com/googleapis/release-please/main/schemas/config.json` after each change.
- The workflows only release and sync. Tests, lint and format checks run in the `pre-push` hook, never in CI.

## Files

| File | Purpose |
| --- | --- |
| `.github/workflows/release-please.yml` | opens the release pull request and creates the release |
| `.github/workflows/sync-develop-from-main.yml` | merges `main` into `develop` after a release |
| `.github/config/release/release-please.config.json` | release type, tag format, changelog sections and files to update |
| `.github/config/release/release-please-manifest.json` | the current released version |
