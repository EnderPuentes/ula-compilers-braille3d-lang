# Contributing

Thank you for helping with the braille3d interpreter. This page lists the rules of the project. To set up your machine first, read [`docs/getting-started.md`](docs/getting-started.md).

## Workflow

1. Every change starts from an issue. Open one if it does not exist.
2. Work on a branch created from that issue, cut from `develop`.
3. Open a pull request into `develop`.
4. After review, the pull request is merged. Releases are automatic, see [Releases](#releases).

Do not commit directly to `main` or `develop`.

## Branches

Only three kinds of branch are allowed:

- `main`: released code.
- `develop`: the integration branch.
- Issue branches cut from `develop`, named as GitHub generates them: `<issue-number>-<slug>`, for example `12-add-lexer`.

The easiest way to create one is from the issue itself, so the name is right and the branch is linked to the issue.

With the GitHub CLI:

```bash
git switch develop && git pull
gh issue develop 12 --base develop --checkout
```

In the browser: open the issue, go to Development, choose Create a branch, select `develop` as the source, then get it locally:

```bash
git fetch
git switch <branch-name>
```

The hooks reject any other branch name when you commit or push, and they require an issue branch to descend from `develop`.

## Commits

Commits follow [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/), in English:

```
<type>(<scope>): <short description>
```

- Types: `feat`, `fix`, `docs`, `style`, `refactor`, `perf`, `test`, `build`, `ci`, `chore`, `revert`.
- The scope is optional: `feat(lexer): add token kinds`.
- Add `!` after the type for a breaking change: `feat(parser)!: change the grammar entry point`.
- The subject uses plain ASCII characters (no accents) and describes one idea.
- Keep commits small. Each commit should build and pass the tests.

The release notes are generated from these messages, so write them for the reader of the changelog.

## Checks before you push

```bash
make format
make lint
make test
```

| Command | What it does |
| --- | --- |
| `make format` | formats the C++ files with `clang-format` (style in `.clang-format`) |
| `make lint` | compiles every file with warnings as errors |
| `make test` | builds and runs every `test/*.test.cpp` |

The Git hooks in `.githooks/` run these checks for you:

| Hook | Runs |
| --- | --- |
| `commit-msg` | checks the commit message format |
| `pre-commit` | looks for secrets and checks the format of the staged C++ files |
| `pre-push` | runs `make build`, `make lint` and `make test` and stops the push if one fails |

The checks run on your machine and not in CI, so the hooks must be active: `git config core.hooksPath .githooks`.

## Code style

- C++17. The compiler flags are in `config.mk`.
- The format is defined by `.clang-format`: 4 spaces, Allman braces, 80 columns. Run `make format` and do not reformat code you did not change.
- Everything is written in English: code, file names, comments, documentation and program output.
- Sources are in `src/`, headers in `include/`, tests in `test/` and documentation in `docs/`.
- Do not copy DeSiGNAR into the repository. It is a system prerequisite, and its path is set in `config.local.mk`.

## Tests

Tests are in `test/` and are named `<name>.test.cpp`. Each file is a program that returns `0` when the test passes and any other value when it fails. `make test` finds every file with that name, so adding a test needs no change in the `Makefile`.

```cpp
int main()
{
    int sum = 1 + 1;
    return sum == 2 ? 0 : 1;
}
```

A change that adds behavior should add or update a test.

## Pull requests

- Open the pull request into `develop`.
- The title and the description are in English. The title is short and says what changes.
- Link the issue in the description, for example `Part of #12`.
- Explain what changed and how you checked it (which `make` commands you ran).
- Keep it focused on one issue. If it needs more commits, that is fine; if it needs more issues, split it.

## Releases

Releases are automatic and use the commit messages. Do not edit by hand:

- `CHANGELOG.md`
- `.github/config/release/release-please-manifest.json`
- the version line in `include/version.hpp` (it has an `x-release-please-version` comment; keep it)

The process is in [`docs/releasing.md`](docs/releasing.md).

## Questions

Open an issue or ask in the pull request.
