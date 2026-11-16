# Git hooks

| Hook | Runs |
| --- | --- |
| pre-commit, pre-push | branch policy (see below) |
| commit-msg | Conventional Commits subject, English, plain ASCII |
| pre-commit | secret grep, `clang-format` check on staged C/C++ files (needs `.clang-format`) |
| pre-push | `make build`, `make lint`, `make test` (only the targets that exist) |

## Branch policy

Only `main`, `develop` and issue branches cut from `develop` are allowed. Issue branches use the name GitHub generates from the issue: `<issue-number>-<slug>` (for example `12-add-lexer`). Create them with `gh issue develop <n>` or from the issue page. Once `develop` exists, an issue branch must descend from it.

Tests, lint and build run in pre-push only, never in CI.

Activate once per clone:

```bash
git config core.hooksPath .githooks
```
