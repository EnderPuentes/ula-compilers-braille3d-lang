# Architecture

This document describes how the interpreter is organized. It only states what is already decided. Open points are listed at the end.

## Goal

The interpreter reads a program written in the braille3d language and runs it. It is a university project for the compilers course, written in C++17.

## Directory structure

| Directory | Content |
| --- | --- |
| `src/` | implementation files (`.cpp`) |
| `include/` | project headers (`.hpp`) |
| `test/` | tests, named `<name>.test.cpp` |
| `docs/` | project documentation |
| `build/` | build output (not versioned) |

Files that configure the build and the tools are in the root: `Makefile`, `config.mk`, `.clang-format`.

## Pipeline

The interpreter is built in stages. Each stage has its own issue:

1. **Lexer** (issue #2): reads the source text and produces tokens.
2. **Parser** (issue #3): reads the tokens and checks the grammar.

The stages after the parser are defined when the language specification is written.

## Dependencies

The interpreter uses [DeSiGNAR](https://github.com/R3mmurd/DeSiGNAR), a C++17 library of data structures and algorithms. It is a system prerequisite and is not copied into this repository. See the README for how to install it and how to set its path.

## Build and quality

- `make build` compiles the interpreter into `build/braille3d`.
- `make lint` compiles every file with warnings treated as errors.
- `make format-check` checks the code style defined in `.clang-format`.
- `make test` builds and runs every `test/*.test.cpp`.

The `pre-push` hook runs `make build`, `make lint` and `make test`.

Releases are automated with release-please (`.github/workflows/release-please.yml`), and `develop` is kept up to date with `main` after each release (`.github/workflows/sync-develop-from-main.yml`). See `docs/releasing.md`.

## Open points

- The language specification: syntax and meaning of braille3d.
- Whether the lexer and the parser use the compiler module of DeSiGNAR or are written by hand.
- The stages after the parser.
