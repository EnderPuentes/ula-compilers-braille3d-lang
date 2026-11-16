include config.mk

# No implicit rules, and no half-written outputs after a failed recipe.
.SUFFIXES:
MAKEFLAGS += --no-builtin-rules
.DELETE_ON_ERROR:

# --- Directories ---------------------------------------------------------
SRC_DIR     := src
INCLUDE_DIR := include
TEST_DIR    := test
BUILD_DIR   := build
OBJ_DIR     := $(BUILD_DIR)/obj

# --- Files ---------------------------------------------------------------
SRCS      := $(shell find $(SRC_DIR) -name '*.cpp' | sort)
OBJS      := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
DEPS      := $(OBJS:.o=.d)
TARGET    := $(BUILD_DIR)/braille3d
TEST_SRCS := $(wildcard $(TEST_DIR)/*.test.cpp)
TEST_BINS := $(patsubst $(TEST_DIR)/%.test.cpp,$(BUILD_DIR)/%.test,$(TEST_SRCS))
CXX_FILES := $(SRCS) $(shell find $(INCLUDE_DIR) -name '*.hpp' | sort) $(TEST_SRCS)

CLANG_FORMAT ?= clang-format

# --- Compiler commands ---------------------------------------------------
CPPFLAGS += -I$(INCLUDE_DIR)
COMPILE.cpp = $(CXX) $(CPPFLAGS) $(CXXFLAGS)
LINK.cpp    = $(COMPILE.cpp) $(LDFLAGS)

.DEFAULT_GOAL := help
.PHONY: help show-config check-deps check-compiler check-designar-files \
        check-clang-format build lint format format-check test clean FORCE

# --- Information ---------------------------------------------------------
help: ## List the available targets
	@grep -hE '^[a-z-]+:.*## ' $(MAKEFILE_LIST) | \
		awk -F':.*## ' '{ printf "  %-13s %s\n", $$1, $$2 }'

show-config: ## Print the compiler and the DeSiGNAR paths in use
	@echo "CXX                 = $(CXX)"
	@echo "CXXFLAGS            = $(CXXFLAGS)"
	@echo "CPPFLAGS            = $(CPPFLAGS)"
	@echo "LDFLAGS             = $(LDFLAGS)"
	@echo "LDLIBS              = $(LDLIBS)"
	@echo "DESIGNAR_PREFIX     = $(DESIGNAR_PREFIX)"
	@echo "DESIGNAR_INCLUDEDIR = $(DESIGNAR_INCLUDEDIR)"
	@echo "DESIGNAR_LIBDIR     = $(DESIGNAR_LIBDIR)"

# --- Checks --------------------------------------------------------------
check-deps: check-compiler check-designar-files ## Verify the compiler, the DeSiGNAR headers and the library
	@echo "check-deps: OK ($(CXX), DeSiGNAR in $(DESIGNAR_PREFIX))"

check-compiler:
	@command -v $(CXX) >/dev/null 2>&1 || \
		{ echo "check-deps: compiler '$(CXX)' not found."; exit 1; }

check-designar-files: | check-compiler
	@test -f $(DESIGNAR_INCLUDEDIR)/types.hpp || \
		{ echo "check-deps: DeSiGNAR headers not found in $(DESIGNAR_INCLUDEDIR). See README.md, Prerequisites."; exit 1; }
	@test -f $(DESIGNAR_LIBDIR)/libDesignar.a || \
		{ echo "check-deps: libDesignar.a not found in $(DESIGNAR_LIBDIR). See README.md, Prerequisites."; exit 1; }

check-clang-format:
	@command -v $(CLANG_FORMAT) >/dev/null 2>&1 || \
		{ echo "format: '$(CLANG_FORMAT)' not found. Install clang-format or set CLANG_FORMAT."; exit 1; }

# --- Build ---------------------------------------------------------------
build: $(TARGET) ## Build the interpreter

$(TARGET): $(OBJS) | check-deps
	@mkdir -p $(@D)
	$(LINK.cpp) $(OBJS) $(LDLIBS) -o $@

# -MMD -MP write header dependencies next to each object file.
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | check-deps
	@mkdir -p $(@D)
	$(COMPILE.cpp) -MMD -MP -c $< -o $@

-include $(DEPS)

# --- Quality -------------------------------------------------------------
lint: | check-deps ## Check every C++ file with warnings as errors (no output files)
	@for file in $(SRCS) $(TEST_SRCS); do \
		$(COMPILE.cpp) -Werror -fsyntax-only $$file || exit 1; \
	done
	@echo "lint: OK"

format: check-clang-format ## Format every C++ file in place
	$(CLANG_FORMAT) -i $(CXX_FILES)

format-check: check-clang-format ## Check the format without changing files
	$(CLANG_FORMAT) --dry-run --Werror $(CXX_FILES)

# --- Tests ---------------------------------------------------------------
test: check-deps $(TEST_BINS) ## Build and run every test/*.test.cpp
	@for test in $(TEST_BINS); do \
		$$test || { echo "test: $$test failed."; exit 1; }; \
	done
	@echo "test: all tests passed"

# FORCE rebuilds every test on each run, so a changed compiler or path
# cannot reuse a stale binary.
$(BUILD_DIR)/%.test: $(TEST_DIR)/%.test.cpp FORCE
	@mkdir -p $(@D)
	@$(LINK.cpp) $< $(LDLIBS) -o $@ || \
		{ echo "test: $< did not build with $(CXX) -std=$(CXXSTD). DeSiGNAR needs C++17 and must be built with the same compiler and standard library."; exit 1; }

FORCE:

# --- Cleaning ------------------------------------------------------------
clean: ## Remove build output
	$(RM) -r $(BUILD_DIR)
