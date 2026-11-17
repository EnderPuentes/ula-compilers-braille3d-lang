# Project configuration, shared by every build target.
# Machine-specific values go in config.local.mk (not versioned); copy
# config.local.mk.example to start one. Any variable can also be set on
# the command line, for example: make DESIGNAR_PREFIX=$HOME/.local

-include config.local.mk

# --- DeSiGNAR library (system prerequisite, see README.md) -------------------
# Installation prefix of DeSiGNAR. Headers live in $(DESIGNAR_PREFIX)/include
# and libDesignar.a in $(DESIGNAR_PREFIX)/lib (lib64 on some distributions).
DESIGNAR_PREFIX     ?= /usr/local
DESIGNAR_INCLUDEDIR ?= $(DESIGNAR_PREFIX)/include
DESIGNAR_LIBDIR     ?= $(DESIGNAR_PREFIX)/lib

# --- Compiler ----------------------------------------------------------------
# The project is built and tested with g++ only. DeSiGNAR must be built with
# the same compiler and standard library.
# DeSiGNAR must be built with the same compiler and standard library.
CXXSTD   ?= c++17
WARNINGS ?= -Wall -Wextra -Wpedantic

# -isystem (not -I) keeps warnings from DeSiGNAR headers out of our build.
CPPFLAGS += -isystem $(DESIGNAR_INCLUDEDIR)
CXXFLAGS += -std=$(CXXSTD) $(WARNINGS)
LDFLAGS  += -L$(DESIGNAR_LIBDIR)
LDLIBS   += -lDesignar -pthread
