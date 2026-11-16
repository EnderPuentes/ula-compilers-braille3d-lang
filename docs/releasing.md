# Releasing

Releases are automated with [release-please](https://github.com/googleapis/release-please). It reads the Conventional Commits on `main` and prepares the next version.

## How it works

1. Work is merged into `develop`, and `develop` is merged into `main`. Use a merge commit, not a squash, so every Conventional Commit reaches `main`.
2. When commits reach `main`, the workflow `.github/workflows/release-please.yml` opens or updates a pull request named `chore(main): release X.Y.Z`.
3. That pull request updates `CHANGELOG.md`, `.release-please-manifest.json` and the version in `include/version.hpp`.
4. Merging it creates the tag `vX.Y.Z` and the GitHub release with the notes.

## Which commits change the version

| Commit | Version change |
| --- | --- |
| `fix: ...` | patch (`0.1.0` to `0.1.1`) |
| `feat: ...` | minor (`0.1.0` to `0.2.0`) |
| `feat!: ...` or a `BREAKING CHANGE:` footer | major |

Other types (`docs`, `build`, `test`, `chore`, `ci`, `style`, `refactor`) do not create a release by themselves.

## Rules

- Do not edit `CHANGELOG.md`, `.release-please-manifest.json` or the version in `include/version.hpp` by hand. The release pull request owns them.
- Keep the comment `x-release-please-version` on the version line of `include/version.hpp`. It tells release-please which line to update.
- The workflow only deploys releases. Tests, lint and format checks run in the `pre-push` hook, never in CI.

## Repository setting

The repository must allow GitHub Actions to create pull requests: Settings, Actions, General, Workflow permissions, "Allow GitHub Actions to create and approve pull requests".

## Files

| File | Purpose |
| --- | --- |
| `.github/workflows/release-please.yml` | the workflow, pinned to a commit of the action |
| `release-please-config.json` | release type and the files to update |
| `.release-please-manifest.json` | the current released version |
