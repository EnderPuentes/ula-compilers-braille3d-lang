# braille3d-lang interpreter

A C++ interpreter for the braille3d language, built for the university compilers course.

New here? Start with [`docs/getting-started.md`](docs/getting-started.md). The rules of the project are in [`CONTRIBUTING.md`](CONTRIBUTING.md).

## Prerequisites

Install these tools **before** anything else. This README assumes they are already on your machine and does not install them.

| Tool | Version | Needed for | Check |
| --- | --- | --- | --- |
| C++ compiler | `g++` 9+ or `clang++` 14+ | building the interpreter and DeSiGNAR | `g++ --version` or `clang++ --version` |
| GNU `make` | any recent version | building the interpreter | `make --version` |
| `git` | any recent version | cloning DeSiGNAR | `git --version` |
| `clang-format` | any recent version (optional) | `make format`, `make format-check` and the pre-commit hook | `clang-format --version` |
| CMake | 3.20+ | installing DeSiGNAR, **once** | `cmake --version` |
| DeSiGNAR | 4.0.1 (commit `ab9099c`) | the data structures library | see [Install DeSiGNAR](#install-designar) |

CMake is used only to build and install DeSiGNAR. If DeSiGNAR is already installed on your machine, you do not need CMake. Get CMake from your system package manager or from <https://cmake.org/download/>.

### Install DeSiGNAR

[DeSiGNAR](https://github.com/R3mmurd/DeSiGNAR) is a C++17 data structures and algorithms library. It is a **system prerequisite**: it is installed once, outside this repository, and the project only needs to know where it is.

Build and install it into a prefix you own, for example `~/.local`:

```bash
git clone https://github.com/R3mmurd/DeSiGNAR.git
cd DeSiGNAR
git checkout ab9099c
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DDESIGNAR_BUILD_SAMPLES=OFF -DDESIGNAR_BUILD_TESTS=OFF
cmake --build build --parallel
cmake --install build --prefix ~/.local
```

This installs the headers into `~/.local/include` and `libDesignar.a` into `~/.local/lib`.

### Compiler: g++ or clang++

Both compilers are supported. DeSiGNAR **must be built with the same compiler and standard library as the interpreter**; mixing them fails at link time. To use clang, build DeSiGNAR with it and use a separate prefix:

```bash
CC=clang CXX=clang++ cmake -S . -B build-clang -DCMAKE_BUILD_TYPE=Release -DDESIGNAR_BUILD_SAMPLES=OFF -DDESIGNAR_BUILD_TESTS=OFF
cmake --build build-clang --parallel
cmake --install build-clang --prefix ~/.local/designar-clang
```

## Configuring the DeSiGNAR path

The location of the library is defined in one place, `config.mk`:

| Variable | Default | Meaning |
| --- | --- | --- |
| `DESIGNAR_PREFIX` | `/usr/local` | installation prefix |
| `DESIGNAR_INCLUDEDIR` | `$(DESIGNAR_PREFIX)/include` | directory with the headers |
| `DESIGNAR_LIBDIR` | `$(DESIGNAR_PREFIX)/lib` | directory with `libDesignar.a` (use `lib64` where your distribution requires it) |
| `CXX` | `g++` | compiler (`clang++` also works) |

Set the path for your machine in `config.local.mk`, which is not versioned:

```bash
cp config.local.mk.example config.local.mk
```

Then edit `DESIGNAR_PREFIX` in it. You can also override any variable for one command:

```bash
make test DESIGNAR_PREFIX=$HOME/.local
make test CXX=clang++ DESIGNAR_PREFIX=$HOME/.local/designar-clang
```

The library is included with `-isystem`, so warnings inside DeSiGNAR headers never break a `-Werror` build.

## Verifying the setup

```bash
make help          # lists the targets
make show-config   # prints the compiler and the paths in use
make check-deps    # checks the compiler, the DeSiGNAR headers and libDesignar.a
make test          # builds and runs every test/*.test.cpp against DeSiGNAR
```

Each check stops at the first problem and names it: the compiler, the DeSiGNAR headers, `libDesignar.a`, or a test that does not build (wrong standard, or DeSiGNAR built with a different compiler).

Tests live in `test/` and are named `<name>.test.cpp`; each one is a program that exits with 0 when it passes. `test/prerequisites.test.cpp` is the basic prerequisites test: it inserts and searches one value in a DeSiGNAR `AVLTree`, so it only passes if the library compiles, links and runs with your compiler and paths.

## Building and running

```bash
make build
./build/braille3d
```

## Project layout

| Path | Content |
| --- | --- |
| `src/` | implementation files |
| `include/` | project headers |
| `test/` | tests, named `<name>.test.cpp` |
| `docs/` | documentation: [`getting-started.md`](docs/getting-started.md), [`architecture.md`](docs/architecture.md), [`lexer.md`](docs/lib/lexer.md), [`examples.md`](docs/examples/examples.md), [`releasing.md`](docs/releasing.md) || `CONTRIBUTING.md` | how to contribute: branches, commits, checks and pull requests |
| `config.mk` | compiler and DeSiGNAR settings |
| `Makefile` | build, lint, format and test targets |
| `.github/` | release workflows and their configuration, see [`docs/releasing.md`](docs/releasing.md) |
| `.clang-format` | code style |

## Make targets

| Target | What it does |
| --- | --- |
| `make build` | builds the interpreter into `build/braille3d` |
| `make lint` | compiles every file with warnings as errors, without output files |
| `make format` | formats every C++ file in place (needs `clang-format`) |
| `make format-check` | checks the format without changing files (needs `clang-format`) |
| `make test` | builds and runs every `test/*.test.cpp` |
| `make check-deps` | checks the compiler and DeSiGNAR |
| `make clean` | removes `build/` |

## Releases

Releases are created automatically from Conventional Commits with release-please. The version, the tags and `CHANGELOG.md` are generated, and the workflows use a personal access token stored as the repository secret `RELEASE_PLEASE_TOKEN`. See [`docs/releasing.md`](docs/releasing.md) for the process, the token and the rules.

## License

This project is distributed under the academic terms of use and copyright detailed in [`LICENSE.md`](LICENSE.md).