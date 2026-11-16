include config.mk

# No implicit rules, and no half-written outputs after a failed recipe.
.SUFFIXES:
MAKEFLAGS += --no-builtin-rules
.DELETE_ON_ERROR:

BUILD_DIR := build
TEST_DIR  := test
TEST_SRCS := $(wildcard $(TEST_DIR)/*.test.cpp)
TEST_BINS := $(patsubst $(TEST_DIR)/%.test.cpp,$(BUILD_DIR)/%.test,$(TEST_SRCS))

COMPILE.cpp = $(CXX) $(CPPFLAGS) $(CXXFLAGS)
LINK.cpp    = $(COMPILE.cpp) $(LDFLAGS)

.DEFAULT_GOAL := help
.PHONY: help show-config check-deps check-compiler check-designar-files test clean FORCE

help: ## List the available targets
	@grep -hE '^[a-z-]+:.*## ' $(MAKEFILE_LIST) | \
		awk -F':.*## ' '{ printf "  %-12s %s\n", $$1, $$2 }'

show-config: ## Print the compiler and the DeSiGNAR paths in use
	@echo "CXX                 = $(CXX)"
	@echo "CXXFLAGS            = $(CXXFLAGS)"
	@echo "CPPFLAGS            = $(CPPFLAGS)"
	@echo "LDFLAGS             = $(LDFLAGS)"
	@echo "LDLIBS              = $(LDLIBS)"
	@echo "DESIGNAR_PREFIX     = $(DESIGNAR_PREFIX)"
	@echo "DESIGNAR_INCLUDEDIR = $(DESIGNAR_INCLUDEDIR)"
	@echo "DESIGNAR_LIBDIR     = $(DESIGNAR_LIBDIR)"

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

test: check-deps $(TEST_BINS) ## Build and run every test/*.test.cpp
	@for test in $(TEST_BINS); do \
		$$test || { echo "test: $$test failed."; exit 1; }; \
	done
	@echo "test: all tests passed"

# FORCE rebuilds every test on each run, so a changed compiler or path
# cannot reuse a stale binary.
$(BUILD_DIR)/%.test: $(TEST_DIR)/%.test.cpp FORCE
	@mkdir -p $(BUILD_DIR)
	@$(LINK.cpp) $< $(LDLIBS) -o $@ || \
		{ echo "test: $< did not build with $(CXX) -std=$(CXXSTD). DeSiGNAR needs C++17 and must be built with the same compiler and standard library."; exit 1; }

FORCE:

clean: ## Remove build output
	$(RM) -r $(BUILD_DIR)
