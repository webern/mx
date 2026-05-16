# mx build/test driver
# ============================================================================
#
# This Makefile is a thin, portable convenience wrapper around CMake. It does
# not replace CMake; it just encodes the handful of build/test configurations
# this project actually uses so you don't have to remember the option combos.
#
# It assumes a POSIX shell and that `cmake` is on PATH. On Windows it is
# best-effort: install CMake plus GNU make and a POSIX shell (Git Bash, MSYS2,
# or WSL) and it works there too. The underlying compiler/generator can still
# be MSVC -- builds go through `cmake --build`, which is generator-agnostic.
#
# Requires CMake >= 3.13 (for `cmake -S/-B` and `--build --parallel`).
#
# ----------------------------------------------------------------------------
# Build modes
# ----------------------------------------------------------------------------
#
# The project exposes three CMake options: MX_BUILD_TESTS, MX_BUILD_CORE_TESTS,
# and MX_BUILD_EXAMPLES. Only three points on that matrix are workflows the
# project actually documents, so those are the three build targets:
#
#   lib   TESTS=off  CORE=off  EXAMPLES=off
#
#   dev   TESTS=on   CORE=off  EXAMPLES=on
#         The README's "recommended configuration for development"
#
#   core  TESTS=on   CORE=on   EXAMPLES=on
#         "if you make changes in the mx::core namespace" --
#         e.g. regenerating mx::core for a new MusicXML version. Slow compile.
#
# ----------------------------------------------------------------------------
# Build directory layout
# ----------------------------------------------------------------------------
#
# Each mode builds into build/<mode>/<BUILD_TYPE> with its own CMake cache and
# incremental state, e.g. build/dev/Debug, build/core/Debug. Because the modes
# do not share a directory, switching from `core` back to `dev` (or flipping
# BUILD_TYPE) never reconfigures and never recompiles the slow core tests.
# `build/` is already in .gitignore.
#
# ----------------------------------------------------------------------------
# Knobs (environment / make variables -- these are overrides, not modes)
# ----------------------------------------------------------------------------
#
#   JOBS        Parallel compile jobs. Auto-detected; override: JOBS=8 make dev
#   BUILD_TYPE  CMake build type, default Debug. Passed as -DCMAKE_BUILD_TYPE
#               (single-config generators) AND --config (multi-config: MSVC,
#               Xcode), so it is correct on every generator.
#   GENERATOR   CMake generator. Unset = CMake's platform default (Unix
#               Makefiles on Linux/macOS, Visual Studio on Windows). Override:
#               GENERATOR=Ninja make dev
#   ARGS        Forwarded to the mxtest (Catch2) binary, e.g.
#               make test ARGS='[core]'  or  make test ARGS='--list-tests'
#
# ============================================================================

CMAKE      ?= cmake
BUILD_TYPE ?= Debug
BUILD_ROOT := build

# Portable CPU-count detection. Tried in order; the final echo always succeeds
# (Windows cmd/PowerShell exports NUMBER_OF_PROCESSORS; otherwise fall back 4).
JOBS ?= $(shell nproc 2>/dev/null \
          || sysctl -n hw.ncpu 2>/dev/null \
          || getconf _NPROCESSORS_ONLN 2>/dev/null \
          || echo "$${NUMBER_OF_PROCESSORS:-4}")

# Optional -G flag. Generator names contain spaces, so quote when set.
ifneq ($(strip $(GENERATOR)),)
GEN_ARG := -G "$(GENERATOR)"
endif

# build/<mode>/<BUILD_TYPE> for the given mode ($1).
mode_dir = $(BUILD_ROOT)/$(1)/$(BUILD_TYPE)

# Configure + build a mode. $1 = mode name, then the three MX_BUILD_* values.
define cmake_build
	$(CMAKE) -S . -B $(call mode_dir,$(1)) \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DMX_BUILD_TESTS=$(2) \
		-DMX_BUILD_CORE_TESTS=$(3) \
		-DMX_BUILD_EXAMPLES=$(4) \
		$(GEN_ARG)
	$(CMAKE) --build $(call mode_dir,$(1)) --parallel $(JOBS) --config $(BUILD_TYPE)
endef

# Locate and run a built binary. CMake places it at <dir>/<name> for
# single-config generators but <dir>/<BUILD_TYPE>/<name>(.exe) for
# multi-config ones (MSVC, Xcode), so probe both. $1 = mode dir, $2 = binary
# name, $3 = extra args.
define run_bin
	@d='$(1)'; b='$(2)'; found=''; bindir=''; \
	for p in "$$d/$$b" "$$d/$$b.exe" "$$d/$(BUILD_TYPE)/$$b" "$$d/$(BUILD_TYPE)/$$b.exe"; do \
		if [ -x "$$p" ]; then found="$$p"; bindir="$$(dirname $$p)"; break; fi; \
	done; \
	if [ -z "$$found" ]; then echo "error: $$b not found under $$d" >&2; exit 1; fi; \
	echo ">> $$found $(3)"; \
	cd "$$bindir" && "$$found" $(3)
endef

.DEFAULT_GOAL := help
.PHONY: help lib dev core test test-core examples-run all clean

help:
	@echo 'mx build/test targets (see comments at the top of the Makefile for rationale):'
	@echo ''
	@echo '  make lib            Build just the static library (no tests, no examples).'
	@echo '  make dev            Build tests (no slow core tests) + examples. Dev loop.'
	@echo '  make core           Build the full suite incl. slow mx::core tests.'
	@echo ''
	@echo '  make test           Build dev, then run mxtest.        ARGS= forwarded.'
	@echo '  make test-core      Build core, then run full mxtest.  ARGS= forwarded.'
	@echo '  make examples-run   Build dev, then run mxread/mxwrite/mxhide.'
	@echo '  make all            Build core, run examples, run full mxtest.'
	@echo ''
	@echo '  make clean          Remove the entire $(BUILD_ROOT)/ tree.'
	@echo ''
	@echo 'Knobs:  JOBS (=$(JOBS))  BUILD_TYPE (=$(BUILD_TYPE))  GENERATOR  ARGS'
	@echo 'Layout: $(BUILD_ROOT)/<mode>/$(BUILD_TYPE)/'

# --- Compile-only targets ---------------------------------------------------

lib:
	$(call cmake_build,lib,off,off,off)

dev:
	$(call cmake_build,dev,on,off,on)

core:
	$(call cmake_build,core,on,on,on)

# --- Run targets ------------------------------------------------------------

test: dev
	$(call run_bin,$(call mode_dir,dev),mxtest,$(ARGS))

test-core: core
	$(call run_bin,$(call mode_dir,core),mxtest,$(ARGS))

examples-run: dev
	$(call run_bin,$(call mode_dir,dev),mxread,)
	$(call run_bin,$(call mode_dir,dev),mxwrite,)
	$(call run_bin,$(call mode_dir,dev),mxhide,)

# Behavioral replacement for the old build.sh: full build + run everything.
all: core
	$(call run_bin,$(call mode_dir,core),mxread,)
	$(call run_bin,$(call mode_dir,core),mxwrite,)
	$(call run_bin,$(call mode_dir,core),mxhide,)
	$(call run_bin,$(call mode_dir,core),mxtest,$(ARGS))

# --- Housekeeping -----------------------------------------------------------

clean:
	rm -rf $(BUILD_ROOT)
