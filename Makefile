# mx build/test driver
# ============================================================================
#
# This Makefile is a thin, portable convenience wrapper around CMake. It does
# not replace CMake but rather encodes the commands to drive it.
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
# `make fmt`, `make check`, `make check-core-dev`, and `make coverage-core-dev`
# run inside a Docker container. The Makefile builds it once (`mx-sdk`) and
# rebuilds if the Dockerfile changes
#
# `docker run` uses an `mx-build` volume mounted so source edits and
# incremental build state (objects + ccache) persist between runs.
#
# ----------------------------------------------------------------------------
# Build modes
# ----------------------------------------------------------------------------
#
# MX_BUILD_TESTS, MX_BUILD_CORE_TESTS, and MX_BUILD_EXAMPLES.:
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
# Each mode builds into build/<mode>/<BUILD_TYPE> with its own CMake cache.
# The Docker-run gates build into the mx-build volume (mounted at build/) with
# ccache, keeping the pinned-GCC artifacts separate from the native ones.
#
# ----------------------------------------------------------------------------
# Knobs (environment / make variables -- these are overrides, not modes)
# ----------------------------------------------------------------------------
#
#   JOBS        Parallel compile jobs. Auto-detected; override: JOBS=8 make dev
#   BUILD_TYPE  CMake build type, default Debug. Passed as -DCMAKE_BUILD_TYPE
#               (single-config generators) AND --config (multi-config: MSVC,
#               Xcode), so it is correct on every generator.
#   GENERATOR   CMake generator. Unset = CMake's platform default.
#   ARGS        Forwarded to the mxtest (Catch2) binary, e.g.
#               make test ARGS='[core]'  or  make test ARGS='--list-tests'
#   DOCKER      Docker executable (default: docker).
#
# ============================================================================

CMAKE      ?= cmake
DOCKER     ?= docker
BUILD_TYPE ?= Debug
BUILD_ROOT := build

# Coverage report output. gcov-14 matches the pinned g++-14 toolchain
COV_DIR := data/testOutput/coverage
GCOV    ?= gcov-14

# gen-quality / gen-lint: static analysis of the gen/ generator. The floors are
# the CI gate -- a build fails below them; ratchet them upward as the generator
# improves (see docs/ai/projects/gen). gen-quality scores design (size +
# complexity); gen-lint enforces genuine lint defects. Pinned analyzers live in
# the mx-sdk venv (see Dockerfile). GEN_PY is every gen/*.py except the measurer.
QUALITY_VENV      := /opt/quality-venv
QUALITY_DIR       := data/testOutput/gen-quality
GEN_PY            := $(filter-out gen/quality.py,$(wildcard gen/*.py))
GEN_QUALITY_FLOOR ?= 37.7
GEN_LINT_FLOOR    ?= 9.4

# For GitHub Actions. When present, push/pull the Docker layer cache to the
# GitHub Actions cache.
ifneq ($(ACTIONS_RUNTIME_TOKEN),)
DOCKER_CACHE := --cache-from type=gha --cache-to type=gha,mode=max
endif

# Docker SDK image + build volume. Incremental state (objects + ccache)
# persists across runs.
DOCKER_IMAGE  := mx-sdk
DOCKER_VOLUME := mx-build
DOCKER_STAMP  := $(BUILD_ROOT)/.docker-image-stamp

# Prevent root-owned files on Linux.
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
DOCKER_USER := --user $(shell id -u):$(shell id -g)
endif

# Run a container.
DOCKER_RUN := $(DOCKER) run --rm \
	-v $(CURDIR):/workspace \
	-v $(DOCKER_VOLUME):/workspace/build \
	$(DOCKER_USER) \
	$(DOCKER_IMAGE)

# Portable CPU-count detection.
JOBS ?= $(shell nproc 2>/dev/null \
          || sysctl -n hw.ncpu 2>/dev/null \
          || getconf _NPROCESSORS_ONLN 2>/dev/null \
          || echo "$${NUMBER_OF_PROCESSORS:-4}")

# Optional generator flag.
ifneq ($(strip $(GENERATOR)),)
GEN_ARG := -G "$(GENERATOR)"
endif

# build/<mode>/<BUILD_TYPE> for the given mode ($1).
mode_dir = $(BUILD_ROOT)/$(1)/$(BUILD_TYPE)

# Define the Cmake build flags and arguments.
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

# Locate and run a built binary.
define run_bin
	@d='$(1)'; b='$(2)'; found=''; \
	for p in "$$d/$$b" "$$d/$$b.exe" "$$d/$(BUILD_TYPE)/$$b" "$$d/$(BUILD_TYPE)/$$b.exe"; do \
		if [ -x "$$p" ]; then found="$$p"; break; fi; \
	done; \
	if [ -z "$$found" ]; then echo "error: $$b not found under $$d" >&2; exit 1; fi; \
	echo ">> $$found $(3)"; \
	"$$found" $(3)
endef

# Run the three example programs from the given mode dir ($1).
define run_examples
	@mkdir -p data/testOutput
	$(call run_bin,$(1),mxread,)
	$(call run_bin,$(1),mxwrite,./data/testOutput/example.musicxml)
	$(call run_bin,$(1),mxhide,)
endef

.DEFAULT_GOAL := help
.PHONY: help lib dev core test test-all examples-run all clean clean-docker \
        check-docker docker-volume fmt check core-dev check-core-dev \
        test-core-dev coverage-core-dev gen-quality gen-lint \
        xcode-gen xcode-build xcode-test

help:
	@echo 'mx build/test targets:'
	@echo ''
	@echo 'Quality gates (run in docker):'
	@echo '  make fmt            Format all C++ files under src/.'
	@echo '  make check          fmt-check + warning-free build.'
	@echo '  make gen-quality    Score gen/ design quality; fail below the floor.'
	@echo '  make gen-lint       Lint gen/ with pylint; fail below the floor.'
	@echo ''
	@echo 'Build (native):'
	@echo '  make lib            Build just the static library (no tests/examples).'
	@echo '  make dev            Build tests (no slow core tests) + examples.'
	@echo '  make core           Build the full suite incl. slow mx::core tests.'
	@echo ''
	@echo 'Run (native):'
	@echo '  make test           Build dev, run examples + mxtest. ARGS= forwarded.'
	@echo '  make test-all       Build core, run examples + full mxtest. ARGS= fwd.'
	@echo '  make examples-run   Build dev, then run mxread/mxwrite/mxhide.'
	@echo '  make all            Build core, run examples + full mxtest.'
	@echo ''
	@echo 'Housekeeping:'
	@echo '  make clean          Remove the entire $(BUILD_ROOT)/ tree.'
	@echo '  make clean-docker   Remove the sdk image, build volume, and cache.'
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
	-rm -f $(DOCKER_STAMP)
	-$(DOCKER) rmi $(DOCKER_IMAGE) 2>/dev/null || true
	-$(DOCKER) volume rm $(DOCKER_VOLUME) 2>/dev/null || true
	-$(DOCKER) buildx prune -af
	@echo "Removed mx-sdk image, mx-build volume, and Docker build cache."

# --- Quality targets --------------------------------------------------------
#
# fmt/check run inside a pinned Docker toolchain. The Makefile detects
# MX_RUNNING_IN_DOCKER (set by the Dockerfile): inside the container it runs
# the tools directly; outside it builds the mx-sdk image once and runs the
# target inside it via `docker run` with the workspace and build volume
# mounted.

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

# Instrumented core-dev coverage. Builds into the mx-build volume, runs the
# core roundtrip suite to emit .gcda, then runs gcovr. The report lands in
# data/testOutput/coverage, which is on the bind-mounted workspace, so it
# appears on the host directly with no export stage. Compiler is the pinned
# g++-14, so gcov-14 matches the .gcda format. Filtered strictly to
# src/private/mx/core/ -- the codegen target -- excluding the ezxml/utility
# deps and the test harness.
coverage-core-dev:
	@echo "=== build (core-dev, instrumented) ==="
	@# Empty *_COMPILER_LAUNCHER overrides the image's ccache default: keep the
	@# instrumented build off ccache so coverage notes/data stay exact. This is
	@# the small core-dev subset, so the lost caching is cheap.
	$(CMAKE) -S . -B $(call mode_dir,cov-core-dev) \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DCMAKE_C_COMPILER_LAUNCHER= \
		-DCMAKE_CXX_COMPILER_LAUNCHER= \
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

# Static analysis of the gen/ generator (in-container branch). quality.py
# measures and writes the report tree to the workspace mount; the floor check
# below is the gate. See docs/ai/projects/gen.
gen-quality:
	@$(QUALITY_VENV)/bin/python gen/quality.py --floor $(GEN_QUALITY_FLOOR)
	@score=$$(python3 -c "import json; print(json.load(open('$(QUALITY_DIR)/score.json'))['composite'])"); \
	 if awk "BEGIN{exit !($$score < $(GEN_QUALITY_FLOOR))}"; then \
	   echo "ERROR: gen-quality $$score is below floor $(GEN_QUALITY_FLOOR)"; exit 1; \
	 else echo "=== gen-quality OK: $$score >= floor $(GEN_QUALITY_FLOOR) ==="; fi

# pylint as a binary gate (model-A ratchet). --exit-zero so the parsed rating,
# not pylint's message-category exit bitmask, decides pass/fail.
gen-lint:
	@out=$$($(QUALITY_VENV)/bin/pylint --rcfile=gen/.pylintrc --exit-zero $(GEN_PY) 2>&1); \
	 echo "$$out"; \
	 rating=$$(echo "$$out" | sed -n 's/.*rated at \([0-9.]*\)\/10.*/\1/p'); \
	 if [ -z "$$rating" ]; then echo "ERROR: gen-lint could not parse a pylint rating"; exit 1; fi; \
	 if awk "BEGIN{exit !($$rating < $(GEN_LINT_FLOOR))}"; then \
	   echo "ERROR: gen-lint rating $$rating is below floor $(GEN_LINT_FLOOR)"; exit 1; \
	 else echo "=== gen-lint OK: $$rating >= floor $(GEN_LINT_FLOOR) ==="; fi

else

# ===== Outside the container: build the image once, then docker run ========
#
# The image is the pinned toolchain only -- no source. `docker run` bind-mounts
# the workspace and the mx-build volume, so source edits and incremental build
# state (objects + ccache) live outside the image. Inside the container
# MX_RUNNING_IN_DOCKER (set by the image) flips the Makefile to its
# in-container branch above. Every target is the same `docker run` locally and
# in CI; CI just pre-creates the volume bind-backed and caches its ccache dir.

$(DOCKER_STAMP): Dockerfile | check-docker
	@mkdir -p $(BUILD_ROOT)
	$(DOCKER) buildx build $(DOCKER_CACHE) --load -t $(DOCKER_IMAGE) .
	@touch $@

# Ensure the named build volume exists (idempotent). CI pre-creates it
# bind-backed to a cacheable workspace path before invoking make, so this
# inspect succeeds and leaves it alone; locally (or if absent) it creates a
# plain named volume on first use.
docker-volume: | check-docker
	@$(DOCKER) volume inspect $(DOCKER_VOLUME) >/dev/null 2>&1 \
		|| $(DOCKER) volume create $(DOCKER_VOLUME) >/dev/null

fmt: $(DOCKER_STAMP) docker-volume
	$(DOCKER_RUN) make fmt
	@echo "Formatted all C++ files under src/"

check: $(DOCKER_STAMP) docker-volume
	$(DOCKER_RUN) make check

check-core-dev: $(DOCKER_STAMP) docker-volume
	$(DOCKER_RUN) make check-core-dev

# Build instrumented core-dev in the pinned container, run the core roundtrip
# suite, run gcovr; the report tree is written to ./data/testOutput/coverage
# directly via the workspace mount. Same command runs identically in CI.
coverage-core-dev: $(DOCKER_STAMP) docker-volume
	@rm -rf $(COV_DIR)
	$(DOCKER_RUN) make coverage-core-dev
	@echo "Coverage written to $(COV_DIR)/ (open $(COV_DIR)/index.html)"

# Static analysis gates. Pure Python -- no C++ build -- so they only need the
# image. The report tree is written through the workspace mount to
# ./data/testOutput/gen-quality. Same commands run identically in CI.
gen-quality: $(DOCKER_STAMP)
	@rm -rf $(QUALITY_DIR)
	$(DOCKER_RUN) make gen-quality

gen-lint: $(DOCKER_STAMP)
	$(DOCKER_RUN) make gen-lint

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
