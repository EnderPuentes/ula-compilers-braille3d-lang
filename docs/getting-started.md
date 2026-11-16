# Getting started

This guide takes you from zero to a passing `make test`. It should take about 15 minutes, most of it compiling DeSiGNAR. After it, read [`CONTRIBUTING.md`](../CONTRIBUTING.md) for the rules of the project.

## Quick path

If you already have a C++17 compiler, `make`, CMake, `git` and DeSiGNAR installed:

```bash
git clone https://github.com/EnderPuentes/ula-compilers-braille3d-lang.git
cd ula-compilers-braille3d-lang
git config core.hooksPath .githooks
cp config.local.mk.example config.local.mk   # then set DESIGNAR_PREFIX in it
make test
```

If `make test` prints `test: all tests passed`, you are ready. If not, follow the steps below.

## 1. Install the tools

You need a C++ compiler (`g++` 9+ or `clang++` 14+), GNU `make`, `git` and CMake 3.20+. Check them:

```bash
g++ --version        # or: clang++ --version
make --version
git --version
cmake --version
```

`clang-format` is optional but recommended. It checks and fixes the code style:

```bash
clang-format --version
```

To open pull requests from the terminal you can also use the GitHub CLI (`gh`). It is optional: everything can be done in the browser.

## 2. Install DeSiGNAR

The interpreter uses [DeSiGNAR](https://github.com/R3mmurd/DeSiGNAR), a C++17 library of data structures. It is installed once on your machine, outside this repository. The commands are in the README, section [Install DeSiGNAR](../README.md#install-designar).

When it finishes, remember the prefix you used, for example `~/.local`.

## 3. Get the code

```bash
git clone https://github.com/EnderPuentes/ula-compilers-braille3d-lang.git
cd ula-compilers-braille3d-lang
```

Then turn on the project hooks. They check your commits and your branch names, and they run the checks before a push. Do this once per clone:

```bash
git config core.hooksPath .githooks
```

## 4. Tell the project where DeSiGNAR is

```bash
cp config.local.mk.example config.local.mk
```

Open `config.local.mk` and set `DESIGNAR_PREFIX` to the prefix from step 2. This file is not versioned, so your path stays on your machine. More options (including `clang++`) are in the README, section [Configuring the DeSiGNAR path](../README.md#configuring-the-designar-path).

## 5. Check that everything works

```bash
make check-deps
make test
```

`make check-deps` stops at the first missing piece and names it. `make test` builds and runs every test in `test/`.

## 6. Build and run

```bash
make build
./build/braille3d
```

## Your first change

1. Pick an issue from the [issue list](https://github.com/EnderPuentes/ula-compilers-braille3d-lang/issues). Ask to be assigned if someone else may be working on it.
2. Create the branch from the issue (see [CONTRIBUTING.md](../CONTRIBUTING.md#branches)) and switch to it.
3. Make your change in small steps. Before each commit, run:

   ```bash
   make format
   make lint
   make test
   ```

4. Commit with a [Conventional Commit](../CONTRIBUTING.md#commits) message in English, for example `feat(lexer): add token kinds`.
5. Push the branch and open a pull request into `develop`.

## When something fails

| Message | What it means | Fix |
| --- | --- | --- |
| `compiler 'g++' not found` | the compiler is not installed or not in `PATH` | install it, or run `make CXX=clang++ ...` |
| `DeSiGNAR headers not found in ...` | `DESIGNAR_PREFIX` points to the wrong place | check `config.local.mk` |
| `libDesignar.a not found in ...` | the library is in `lib64` or in another folder | set `DESIGNAR_LIBDIR` in `config.local.mk` |
| `did not build with ...` | DeSiGNAR was built with another compiler | build DeSiGNAR again with the same compiler |
| `'clang-format' not found` | the formatter is not installed | install it, or skip `make format` and `make format-check` |
| `commit-msg: subject must match ...` | the commit message does not follow Conventional Commits | fix the subject, see [Commits](../CONTRIBUTING.md#commits) |
| `branch-policy: '...' is not allowed` | the branch name is not `<issue-number>-<slug>` | create the branch from the issue |
| `pre-push: ... failed; push aborted` | `make build`, `make lint` or `make test` failed | fix the error shown above the message |
