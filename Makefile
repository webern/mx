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
# Quality gates run in Docker
# ----------------------------------------------------------------------------
#
# `make fmt` and `make check` run inside a Docker container with a pinned
# toolchain (Ubuntu 24.04 + clang-18 + libc++) so formatting and compiler
# warnings are deterministic on any machine regardless of the floating CI
# runner image. The build/test targets (`make test`, `make test-all`, ...)
# run natively with the local compiler.
# See docs/ai/project/build-and-ci-design.md.
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
#   DOCKER      Docker executable (default: docker).
#
# ============================================================================

CMAKE      ?= cmake
DOCKER     ?= docker
BUILD_TYPE ?= Debug
BUILD_ROOT := build

# Coverage report output (exported from the container to the host) and the
# gcov tool gcovr drives. Default gcov-14 matches the pinned g++-14 toolchain
# inside the container, where coverage actually runs.
COV_DIR := data/testOutput/coverage
GCOV    ?= gcov-14

# In GitHub Actions, crazy-max/ghaction-github-runtime exports
# ACTIONS_RUNTIME_TOKEN. When present, push/pull the Docker layer cache to the
# GitHub Actions cache so linux-gate does not reinstall the toolchain every
# run. Absent (local, fork PRs) -> no flags, plain build. Same `make check`
# everywhere.
ifneq ($(ACTIONS_RUNTIME_TOKEN),)
DOCKER_CACHE := --cache-from type=gha --cache-to type=gha,mode=max
endif

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

# Configure + build a mode. $1 = mode name, then the three MX_BUILD_* values
# followed by MX_CORE_DEV. core-dev passes MX_CORE_DEV=on with the three
# MX_BUILD_* flags off; every other mode passes MX_CORE_DEV=off.
define cmake_build
	$(CMAKE) -S . -B $(call mode_dir,$(1)) \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DMX_BUILD_TESTS=$(2) \
		-DMX_BUILD_CORE_TESTS=$(3) \
		-DMX_BUILD_EXAMPLES=$(4) \
		-DMX_CORE_DEV=$(5) \
		$(GEN_ARG)
	$(CMAKE) --build $(call mode_dir,$(1)) --parallel $(JOBS) --config $(BUILD_TYPE)
endef

# Locate and run a built binary. CMake places it at <dir>/<name> for
# single-config generators but <dir>/<BUILD_TYPE>/<name>(.exe) for
# multi-config ones (MSVC, Xcode), so probe both. $1 = mode dir, $2 = binary
# name, $3 = extra args.
define run_bin
	@d='$(1)'; b='$(2)'; found=''; \
	for p in "$$d/$$b" "$$d/$$b.exe" "$$d/$(BUILD_TYPE)/$$b" "$$d/$(BUILD_TYPE)/$$b.exe"; do \
		if [ -x "$$p" ]; then found="$$p"; break; fi; \
	done; \
	if [ -z "$$found" ]; then echo "error: $$b not found under $$d" >&2; exit 1; fi; \
	echo ">> $$found $(3)"; \
	"$$found" $(3)
endef

# Run the three example programs from the given mode dir ($1). The test
# targets run these too, so the examples are exercised everywhere tests run.
# mxwrite is told to write into data/testOutput so we don't leave an untracked
# example.musicxml at the repo root (issue #150).
define run_examples
	@mkdir -p data/testOutput
	$(call run_bin,$(1),mxread,)
	$(call run_bin,$(1),mxwrite,./data/testOutput/example.musicxml)
	$(call run_bin,$(1),mxhide,)
endef

.DEFAULT_GOAL := help
.PHONY: help lib dev core test test-all examples-run all clean clean-docker \
        check-docker fmt check core-dev check-core-dev test-core-dev \
        coverage-core-dev xcode-gen xcode-build xcode-test

help:
	@echo 'mx build/test targets (see the comments at the top of the Makefile):'
	@echo ''
	@echo 'Done with a code change? Run:'
	@echo '  make fmt && make check && make test'
	@echo '  (use make test-all instead of make test if you touched mx/core)'
	@echo ''
	@echo 'Quality gates (run in Docker, pinned toolchain):'
	@echo '  make fmt            Format all C++ files under src/.'
	@echo '  make check          fmt-check + warning-free build.'
	@echo ''
	@echo 'Build (native):'
	@echo '  make lib            Build just the static library (no tests/examples).'
	@echo '  make dev            Build tests (no slow core tests) + examples.'
	@echo '  make core           Build the full suite incl. slow mx::core tests.'
	@echo ''
	@echo 'Run (native):'
	@echo '  make test           Build dev, run examples + mxtest.   ARGS= forwarded.'
	@echo '  make test-all       Build core, run examples + full mxtest. ARGS= fwd.'
	@echo '  make examples-run   Build dev, then run mxread/mxwrite/mxhide.'
	@echo '  make all            Build core, run examples + full mxtest.'
	@echo ''
	@echo 'Housekeeping:'
	@echo '  make clean          Remove the entire $(BUILD_ROOT)/ tree.'
	@echo '  make clean-docker   Remove the Docker build cache.'
	@echo ''
	@echo 'Xcode:'
	@echo '  make xcode-gen      Generate Xcode project in build/xcode/.'
	@echo '  make xcode-build    Build the Xcode project.'
	@echo '  make xcode-test     Run tests via xcodebuild.'
	@echo ''
	@echo 'Core development (codegen):'
	@echo '  make core-dev           Build trimmed library (mx/core + ezxml + utility) and'
	@echo '                          mxtest-core-dev. No mx/api or mx/impl compiled.'
	@echo '  make check-core-dev     fmt-check + warning-free build for core-dev (Docker).'
	@echo '  make test-core-dev      Build core-dev then run the core roundtrip suite.'
	@echo '                          Each file under data/ is a separate Catch2 test case.'
	@echo "                          Filter: make test-core-dev ARGS='[core-roundtrip] lysuite/*'"
	@echo '  make coverage-core-dev  Instrumented core-dev build (Docker), runs the core'
	@echo '                          roundtrip suite and gcovr. Report -> $(COV_DIR)/.'
	@echo ''
	@echo 'Knobs:  JOBS (=$(JOBS))  BUILD_TYPE (=$(BUILD_TYPE))  GENERATOR  ARGS  DOCKER'
	@echo 'Layout: $(BUILD_ROOT)/<mode>/$(BUILD_TYPE)/'

# --- Compile-only targets ---------------------------------------------------

lib:
	$(call cmake_build,lib,off,off,off,off)

dev:
	$(call cmake_build,dev,on,off,on,off)

core:
	$(call cmake_build,core,on,on,on,off)

# Core-dev mode: trimmed build (mx/core + ezxml + utility) plus the core
# roundtrip test binary. mx/api and mx/impl are not compiled. Intended for
# codegen iteration on mx/core. See AGENTS.md and docs/ai/projects/core-dev/.
core-dev:
	$(call cmake_build,core-dev,off,off,off,on)

# --- Run targets ------------------------------------------------------------

test: dev
	$(call run_examples,$(call mode_dir,dev))
	$(call run_bin,$(call mode_dir,dev),mxtest,$(ARGS))

test-all: core
	$(call run_examples,$(call mode_dir,core))
	$(call run_bin,$(call mode_dir,core),mxtest,$(ARGS))

# Run the core roundtrip suite from the trimmed build. ARGS is forwarded to
# the binary (e.g., make test-core-dev ARGS='[core-roundtrip] lysuite/').
# --allow-running-no-tests keeps the target green when ARGS filters to an
# empty set, and also during Phase A before any test cases are registered.
test-core-dev: core-dev
	$(call run_bin,$(call mode_dir,core-dev),mxtest-core-dev,--allow-running-no-tests $(ARGS))

examples-run: dev
	$(call run_examples,$(call mode_dir,dev))

# Behavioral replacement for the old build.sh: full build + run everything.
all: core
	$(call run_examples,$(call mode_dir,core))
	$(call run_bin,$(call mode_dir,core),mxtest,$(ARGS))

# --- Housekeeping -----------------------------------------------------------

clean:
	rm -rf $(BUILD_ROOT)

clean-docker:
	-$(DOCKER) buildx prune -af
	@echo "Removed Docker build cache."

# --- Quality targets --------------------------------------------------------
#
# fmt/check run inside a pinned Docker toolchain. The Makefile detects
# MX_RUNNING_IN_DOCKER (set by the Dockerfile): inside the container it runs
# the tools directly; outside it builds the image and runs the target inside
# it via `docker buildx build`.

FIND_CPP := find src \
	-path 'src/private/cpul' -prune -o \
	-name 'pugixml.cpp' -prune -o \
	-name 'pugixml.hpp' -prune -o \
	-name 'pugiconfig.hpp' -prune -o \
	-type f \( -name '*.cpp' -o -name '*.h' -o -name '*.hpp' \) -print

check-docker:
	@command -v $(DOCKER) >/dev/null 2>&1 || \
		{ echo "Docker not found. The quality gates (fmt/check/lint) run in"; \
		  echo "Docker with a pinned toolchain. Install Docker to continue:"; \
		  echo "  https://docs.docker.com/get-docker/"; \
		  exit 1; }

ifdef MX_RUNNING_IN_DOCKER

# ===== Inside the container: run the pinned tools directly ==================

fmt:
	@$(FIND_CPP) | xargs clang-format -i
	@echo "Formatted all C++ files under src/"

check:
	@echo "=== fmt-check ==="
	@$(FIND_CPP) | xargs clang-format --dry-run --Werror
	@echo "=== build (warning-free) ==="
	@mkdir -p $(BUILD_ROOT)
	@$(CMAKE) -S . -B $(call mode_dir,dev) \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DMX_BUILD_TESTS=on \
		-DMX_BUILD_CORE_TESTS=off \
		-DMX_BUILD_EXAMPLES=on \
		-DMX_CORE_DEV=off \
		$(GEN_ARG) > $(BUILD_ROOT)/build.log 2>&1 \
		|| { cat $(BUILD_ROOT)/build.log; \
		     echo "ERROR: cmake configure failed (see above)"; exit 1; }
	@$(CMAKE) --build $(call mode_dir,dev) --parallel $(JOBS) --config $(BUILD_TYPE) \
		>> $(BUILD_ROOT)/build.log 2>&1; status=$$?; \
		cat $(BUILD_ROOT)/build.log; \
		if [ $$status -ne 0 ]; then \
			echo "ERROR: build failed (see above)"; exit $$status; \
		fi; \
		if grep -q 'warning:' $(BUILD_ROOT)/build.log; then \
			echo "ERROR: build emitted warnings (see above)"; exit 1; \
		fi
	@echo "=== check passed ==="

# fmt-check + warning-free build for core-dev mode. Same shape as `check`
# but configures with MX_CORE_DEV=on and the three MX_BUILD_* flags off.
check-core-dev:
	@echo "=== fmt-check ==="
	@$(FIND_CPP) | xargs clang-format --dry-run --Werror
	@echo "=== build (warning-free, core-dev) ==="
	@mkdir -p $(BUILD_ROOT)
	@$(CMAKE) -S . -B $(call mode_dir,core-dev) \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DMX_BUILD_TESTS=off \
		-DMX_BUILD_CORE_TESTS=off \
		-DMX_BUILD_EXAMPLES=off \
		-DMX_CORE_DEV=on \
		$(GEN_ARG) > $(BUILD_ROOT)/build.log 2>&1 \
		|| { cat $(BUILD_ROOT)/build.log; \
		     echo "ERROR: cmake configure failed (see above)"; exit 1; }
	@$(CMAKE) --build $(call mode_dir,core-dev) --parallel $(JOBS) --config $(BUILD_TYPE) \
		>> $(BUILD_ROOT)/build.log 2>&1; status=$$?; \
		cat $(BUILD_ROOT)/build.log; \
		if [ $$status -ne 0 ]; then \
			echo "ERROR: build failed (see above)"; exit $$status; \
		fi; \
		if grep -q 'warning:' $(BUILD_ROOT)/build.log; then \
			echo "ERROR: build emitted warnings (see above)"; exit 1; \
		fi
	@echo "=== check-core-dev passed ==="

# Instrumented core-dev coverage. One fused recipe because build/ is an
# ephemeral Docker cache mount: the .gcda files written while running the tests
# must be consumed by gcovr in the same RUN, and the report must land outside
# build/ (in data/testOutput/coverage) so the Docker export stage can copy it
# out. Compiler is the pinned g++-14, so gcov-14 matches the .gcda format.
# Filtered strictly to src/private/mx/core/ -- the codegen target -- excluding
# the ezxml/utility deps and the test harness.
coverage-core-dev:
	@echo "=== build (core-dev, instrumented) ==="
	$(CMAKE) -S . -B $(call mode_dir,cov-core-dev) \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DMX_BUILD_TESTS=off \
		-DMX_BUILD_CORE_TESTS=off \
		-DMX_BUILD_EXAMPLES=off \
		-DMX_CORE_DEV=on \
		-DMX_COVERAGE=on \
		$(GEN_ARG)
	$(CMAKE) --build $(call mode_dir,cov-core-dev) --parallel $(JOBS) --config $(BUILD_TYPE)
	@echo "=== run core roundtrip suite ==="
	$(call run_bin,$(call mode_dir,cov-core-dev),mxtest-core-dev,--allow-running-no-tests $(ARGS))
	@echo "=== gcovr report (src/private/mx/core/) ==="
	@mkdir -p $(COV_DIR)
	gcovr --root . \
		--gcov-executable '$(GCOV)' \
		--filter 'src/private/mx/core/' \
		--print-summary \
		--txt $(COV_DIR)/coverage.txt \
		--xml $(COV_DIR)/coverage.xml \
		--html-self-contained --html $(COV_DIR)/index.html \
		$(call mode_dir,cov-core-dev) | tee $(COV_DIR)/summary.txt
	@echo "=== coverage written to $(COV_DIR)/ ==="

else

# ===== Outside the container: delegate to Docker ===========================

fmt: check-docker
	$(DOCKER) buildx build --target fmt-out \
		--output type=local,dest=. $(DOCKER_CACHE) .
	@echo "Formatted all C++ files under src/"

check: check-docker
	$(DOCKER) buildx build --target run \
		--output type=cacheonly $(DOCKER_CACHE) .

# Core-dev mirror of `check`: delegates to a parallel `run-core-dev` Docker
# stage that hardcodes `make check-core-dev` against the pinned toolchain.
check-core-dev: check-docker
	$(DOCKER) buildx build --target run-core-dev \
		--output type=cacheonly $(DOCKER_CACHE) .

# Build instrumented core-dev in the pinned container, run the core roundtrip
# suite, run gcovr, and export the report tree to ./data/testOutput/coverage on
# the host. Same command runs identically in CI. The `coverage-out` scratch
# stage carries only the report, so type=local writes just that subtree.
#
# -f Dockerfile.coverage (a symlink to Dockerfile) selects the data-inclusive
# Dockerfile.coverage.dockerignore: the corert suite scans data/ at runtime, so
# the corpus must be in this context. The gate targets keep the tiny
# .dockerignore. Same single Dockerfile body either way.
coverage-core-dev: check-docker
	@rm -rf $(COV_DIR)
	$(DOCKER) buildx build -f Dockerfile.coverage --target coverage-out \
		--output type=local,dest=. $(DOCKER_CACHE) .
	@echo "Coverage written to $(COV_DIR)/ (open $(COV_DIR)/index.html)"

endif

# --- Xcode targets ----------------------------------------------------------

XCODE_DIR := $(BUILD_ROOT)/xcode

xcode-gen:
	$(CMAKE) -G Xcode -S . -B $(XCODE_DIR) \
		-DMX_BUILD_TESTS=on \
		-DMX_BUILD_CORE_TESTS=off \
		-DMX_BUILD_EXAMPLES=on

xcode-build: xcode-gen
	$(CMAKE) --build $(XCODE_DIR) --config $(BUILD_TYPE)

xcode-test: xcode-build
	@found=''; \
	for p in "$(XCODE_DIR)/$(BUILD_TYPE)/mxtest" "$(XCODE_DIR)/Debug/mxtest"; do \
		if [ -x "$$p" ]; then found="$$p"; break; fi; \
	done; \
	if [ -z "$$found" ]; then echo "error: mxtest not found under $(XCODE_DIR)" >&2; exit 1; fi; \
	echo ">> $$found"; \
	"$$found"
