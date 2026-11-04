# mx build/check driver.
# ============================================================================
#
# The generated mx::core C++ model lives in src/private/mx/core/generated
# (emitted by `make gen-cpp`, committed). MX_TOOLCHAIN picks where the
# targets run:
#
#   docker (default)  Build the pinned `mx-sdk` image once and run the same
#                     make target inside it.
#   native            Run the tools directly on this machine. The mx-sdk
#                     image sets this, and so does the macOS CI job, which has
#                     no Docker. You need the host tools a target uses.
#
# MX_RUNNING_IN_DOCKER is the old name for native mode and still works.
# Requires CMake >= 3.13.
#
# ============================================================================

ifneq ($(MX_RUNNING_IN_DOCKER),)
MX_TOOLCHAIN := native
endif
MX_TOOLCHAIN ?= docker
ifneq ($(MX_TOOLCHAIN),docker)
ifneq ($(MX_TOOLCHAIN),native)
$(error MX_TOOLCHAIN must be docker or native, not '$(MX_TOOLCHAIN)')
endif
endif

CMAKE      ?= cmake
DOCKER     ?= docker
BUILD_TYPE ?= Debug
BUILD_ROOT := build
JOBS       ?= $(shell nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

# Tool names. Each prefers the version the mx-sdk image pins and falls back to
# the plain name, so native mode works on a machine that has only the plain
# one. gcov must match the compiler: gcov-14 goes with g++-14.
CLANG_FORMAT ?= $(if $(shell command -v clang-format-18 2>/dev/null),clang-format-18,clang-format)
GCOV         ?= $(if $(shell command -v gcov-14 2>/dev/null),gcov-14,gcov)

# Coverage report output.
COV_DIR := data/testOutput/coverage

# gen-quality / gen-lint: static analysis of the gen/ generator. The floors are
# the CI gate -- a build fails below them; ratchet them upward as the generator
# improves. gen-quality scores design (size + complexity); gen-lint enforces
# genuine lint defects. The analyzers are pinned in QUALITY_REQS and run from
# the Python venv at QUALITY_VENV. The mx-sdk image has its own venv and sets
# QUALITY_VENV to it. Natively, `make quality-venv` builds one under build/.
# GEN_PY is every .py under gen/ except the measurer itself.
QUALITY_VENV      ?= $(CURDIR)/$(BUILD_ROOT)/quality-venv
QUALITY_REQS      := gen/requirements-quality.txt
QUALITY_DIR       := data/testOutput/gen-quality
GEN_PY            := $(shell find gen -name '*.py' ! -name quality.py | sort)
GEN_QUALITY_FLOOR ?= 84.5
GEN_LINT_FLOOR    ?= 9.29

# For GitHub Actions. When present, push/pull the Docker layer cache to the
# GitHub Actions cache.
ifneq ($(ACTIONS_RUNTIME_TOKEN),)
DOCKER_CACHE := --cache-from type=gha --cache-to type=gha,mode=max
endif

# Docker SDK image and volumes. Each worktree gets its own build volume, named
# with a hash of its path, so builds in two worktrees cannot clash. The
# compiler caches (ccache, Emscripten, Go) live in one cache volume that every
# worktree shares. Every worktree is mounted at /workspace, so the compile
# commands match and a cache entry from one worktree is a hit in another.
# Deleting a worktree does not lose the cache.
DOCKER_IMAGE        := mx-sdk
DOCKER_VOLUME       ?= mx-build-$(shell printf '%s' '$(CURDIR)' | md5sum 2>/dev/null | cut -c1-8 || printf '%s' '$(CURDIR)' | md5 -q | cut -c1-8)
DOCKER_CACHE_VOLUME ?= mx-cache
DOCKER_STAMP        := $(BUILD_ROOT)/.docker-image-stamp

# Prevent root-owned files on Linux.
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
DOCKER_USER := --user $(shell id -u):$(shell id -g)
endif

DOCKER_RUN := $(DOCKER) run --rm \
	-v $(CURDIR):/workspace \
	-v $(DOCKER_VOLUME):/workspace/build \
	-v $(DOCKER_CACHE_VOLUME):/cache \
	-e JOBS=$(JOBS) \
	$(DOCKER_USER) \
	$(DOCKER_IMAGE)

# Targets that need the toolchain. With MX_TOOLCHAIN=docker each one is
# forwarded into the mx-sdk image (see the end of this file).
DOCKER_TARGETS := \
	test-all symbol-check backend-pugixml-test \
	api-lib api-build api-test api-examples api-roundtrip \
	api-roundtrip-discover api-roundtrip-dump api-roundtrip-classify api-coverage \
	wasm-lib wasm-build wasm-test \
	core-build core-roundtrip-test core-unit core-coverage \
	gen-test audit-test gen-quality gen-lint \
	fmt fmt-check gen gen-cpp gen-go gen-c gen-schema \
	audit audit-force \
	build-go build-c test-go test-c \
	cache-stats

# C++ to format. Skip the vendored pugixml (src/private/mxpugi, written by
# scripts/vendor_pugixml.py) and the cpul/Catch test harness.
FIND_CPP := find src \
	-path 'src/private/cpul' -prune -o \
	-name 'mxpugi.cpp' -prune -o \
	-name 'mxpugi.hpp' -prune -o \
	-name 'mxpugiconfig.hpp' -prune -o \
	-type f \( -name '*.cpp' -o -name '*.h' -o -name '*.hpp' \) -print

.DEFAULT_GOAL := help
.PHONY: help sdk vendor-pugixml quality-venv clean clean-docker check-docker docker-volume \
        clean-docker-all $(DOCKER_TARGETS)

help:
	@echo 'mx targets (see AGENTS.md). They run in the mx-sdk Docker image by default'
	@echo '(MX_TOOLCHAIN=docker). Set MX_TOOLCHAIN=native to run them with the host tools.'
	@echo ''
	@echo '  Everything C++ (the default deep gate, run in CI on Linux):'
	@echo '  make test-all       core-roundtrip-test + core-unit + api-test + api-roundtrip.'
	@echo '  make symbol-check   Fail if a built archive exports a global pugi:: symbol (CI gate).'
	@echo '  make backend-pugixml-test  Build mx against a stock pugixml (MX_XML_BACKEND=pugixml) and run the core suites (CI gate).'
	@echo ''
	@echo '  Product surface (mx::api / mx::impl):'
	@echo '  make api-lib             Build the mx static library (MX_API=ON).'
	@echo '  make api-build           Build mx + mxtest + examples + api-roundtrip binary.'
	@echo '  make api-test            Run examples + mxtest suite (api/impl/file/control).'
	@echo '  make api-examples        Build and run all three api example programs.'
	@echo '  make api-roundtrip       Run the corpus api roundtrip in regression mode (CI gate).'
	@echo '  make api-roundtrip-discover  Run discovery mode over the full corpus (manual only).'
	@echo '  make api-roundtrip-dump      Dump normalized expected/actual XML for failures.'
	@echo '  make api-roundtrip-classify  Classify dumped failures by root cause (Python).'
	@echo '  make api-coverage        Instrumented api/impl/utility build + gcovr report.'
	@echo ''
	@echo '  WebAssembly (mx::api via Emscripten, issue #386):'
	@echo '  make wasm-lib            Build the mx static library for wasm (MX_API=ON, emcmake).'
	@echo '  make wasm-build          wasm-lib plus the mxread/mxwrite/mxhide examples.'
	@echo '  make wasm-test           Run mxread/mxwrite/mxhide under Node against the wasm build.'
	@echo ''
	@echo '  Core substrate (mx::core):'
	@echo '  make core-build          Build mx_core and the corert + unit test binaries.'
	@echo "  make core-roundtrip-test Run the core roundtrip suite. Filter: ARGS='[core-roundtrip] lysuite/*'"
	@echo '  make core-unit           Run the mx::core unit tests (values, shapes, rejection suite).'
	@echo '  make core-coverage       Instrumented build, corert + unit suites, gcovr -> $(COV_DIR)/.'
	@echo ''
	@echo '  Generator (gen/):'
	@echo '  make gen-test       Run the generator (parser + IR + plates + press) Python tests.'
	@echo '  make gen            Run the generator for every target (cpp/go/c/schema).'
	@echo '  make gen-cpp        Run the generator for the C++ target (src/private/mx/core/generated).'
	@echo '  make gen-go         Run the generator for the Go target.'
	@echo '  make gen-c          Run the generator for the C target.'
	@echo '  make gen-schema     Run the generator for the JSON Schema target.'
	@echo '  make gen-quality    Score gen/ design quality; fail below the floor.'
	@echo '  make gen-lint       Lint gen/ with pylint; fail below the floor.'
	@echo '  make quality-venv   Native only: build the Python venv gen-quality/gen-lint use, at $(QUALITY_VENV).'
	@echo ''
	@echo '  Feature audit (audit/):'
	@echo '  make audit          Write missing *.features.xml sidecars + rebuild data/corpus.xml.'
	@echo '  make audit-force    Rewrite every *.features.xml sidecar (use after a format change).'
	@echo '  make audit-test     Run the audit tool Python tests (incl. failure classifier).'
	@echo ''
	@echo '  Generated Go target:'
	@echo '  make build-go       Build Go corert tests.'
	@echo '  make test-go        Run Go corert tests.'
	@echo ''
	@echo '  Generated C target:'
	@echo '  make build-c        Build C corert test binary.'
	@echo '  make test-c         Run C corert tests.'
	@echo ''
	@echo '  Housekeeping:'
	@echo '  make fmt            Format C++ under src/ with clang-format.'
	@echo '  make fmt-check      clang-format gate (checks formatting; builds nothing).'
	@echo '  make sdk            Build the mx-sdk Docker toolchain image.'
	@echo '  make vendor-pugixml VERSION=x.y  Refresh src/private/mxpugi from a pugixml release (runs on the host).'
	@echo '  make clean          Remove the build/ tree and gen build artifacts.'
	@echo '  make cache-stats    Show ccache statistics (the shared cache volume in Docker mode).'
	@echo '  make clean-docker   Remove this worktree'"'"'s build volume. Keeps the image and the shared cache.'
	@echo '  make clean-docker-all  Remove the mx-sdk image, the shared cache volume, and the'
	@echo '                      build volume of EVERY worktree, not just this one. The other'
	@echo '                      worktrees still work; their next build starts cold.'

# --- Housekeeping -----------------------------------------------------------

clean:
	rm -rf $(BUILD_ROOT)
	rm -rf gen/test/c/build
	rm -rf gen/test/go/build

# Re-vendor pugixml under the mxpugi namespace. Runs natively: it only needs
# python3 and network access, and it writes source files, not build output.
vendor-pugixml:
	@test -n "$(VERSION)" || { echo "usage: make vendor-pugixml VERSION=1.16"; exit 1; }
	python3 -I scripts/vendor_pugixml.py --version $(VERSION) --out src/private/mxpugi

# Removes only what belongs to this worktree. The stamp goes too, so the next
# run checks the image again.
clean-docker:
	rm -f $(DOCKER_STAMP)
	@if $(DOCKER) volume inspect $(DOCKER_VOLUME) >/dev/null 2>&1; then \
	  $(DOCKER) volume rm $(DOCKER_VOLUME) >/dev/null && echo "Removed volume $(DOCKER_VOLUME)."; \
	else echo "No volume $(DOCKER_VOLUME) to remove."; fi

# Removes everything mx keeps in Docker, for all worktrees. Build volumes are
# matched by name: mx-build plus an 8-character path hash, or plain mx-build
# as CI names it. That also catches volumes left behind by deleted worktrees.
# A volume or image that is still in use is not removed; Docker prints why and
# the rest are removed anyway.
MX_BUILD_VOLUME_PATTERN := ^mx-build(-[0-9a-f]{8})?$$

clean-docker-all:
	rm -f $(DOCKER_STAMP)
	-@for v in $$($(DOCKER) volume ls -q --filter name=mx-build | grep -E '$(MX_BUILD_VOLUME_PATTERN)') \
	          $$($(DOCKER) volume ls -q --filter name=$(DOCKER_CACHE_VOLUME) | grep -Fx '$(DOCKER_CACHE_VOLUME)'); do \
	  $(DOCKER) volume rm "$$v" >/dev/null && echo "Removed volume $$v."; \
	done; true
	-@if $(DOCKER) image inspect $(DOCKER_IMAGE) >/dev/null 2>&1; then \
	  $(DOCKER) rmi $(DOCKER_IMAGE) >/dev/null && echo "Removed image $(DOCKER_IMAGE)."; \
	fi; true

check-docker:
	@command -v $(DOCKER) >/dev/null 2>&1 || \
		{ echo "Docker not found ($(DOCKER)). Install it to use the mx-sdk image:"; \
		  echo "  https://docs.docker.com/get-docker/"; \
		  echo "Or run with the host tools instead:"; \
		  echo "  MX_TOOLCHAIN=native make $(or $(MAKECMDGOALS),<target>)"; \
		  echo "'make doctor' lists which host tools each target needs."; \
		  exit 1; }

# === Toolchain targets ======================================================

ifeq ($(MX_TOOLCHAIN),native)

# ----- MX_TOOLCHAIN=native: run the tools here (also inside the container) --

# Use ccache when it is installed. CMake reads these variables when it first
# configures a build tree. The mx-sdk image and the macOS CI job set them
# already, and a setting from the environment wins. CCACHE_BASEDIR lets
# worktrees at different paths share ccache entries.
ifeq ($(origin CMAKE_CXX_COMPILER_LAUNCHER),undefined)
ifneq ($(shell command -v ccache 2>/dev/null),)
export CMAKE_C_COMPILER_LAUNCHER := ccache
export CMAKE_CXX_COMPILER_LAUNCHER := ccache
endif
endif
CCACHE_BASEDIR ?= $(CURDIR)
export CCACHE_BASEDIR

# All C++ suites: the core substrate (corert + unit) and the api product
# surface (mxtest + examples + corpus round-trip). The deep gate; CI runs it
# on Linux. gen/go/c and the audit tool are separate (see their targets).
test-all: core-roundtrip-test core-unit api-test api-roundtrip

# mx compiles pugixml under its own namespace (src/private/mxpugi) so that a
# consumer who links a stock pugixml does not get duplicate symbols from mx.
# This gate reads every archive the api and core-dev builds produced and
# fails on any symbol in the stock `pugi::` namespace. The mxpugi:: count is
# printed, and must be non-zero, so an empty or stale archive cannot pass.
# `nm -g | c++filt` is the same on GNU and Apple toolchains. The check only
# makes sense for the default MX_XML_BACKEND=vendored build, which is what
# build/api and build/core-dev hold; a pugixml-mode build references pugi::
# on purpose (see backend-pugixml-test).
SYMBOL_CHECK_ARCHIVES := \
	$(BUILD_ROOT)/api/libmx.a \
	$(BUILD_ROOT)/api/libmx_core.a \
	$(BUILD_ROOT)/api/libmxpugi.a \
	$(BUILD_ROOT)/core-dev/libmx_core.a \
	$(BUILD_ROOT)/core-dev/libmxpugi.a

symbol-check:
	@found=0; total=0; status=0; \
	for archive in $(SYMBOL_CHECK_ARCHIVES); do \
	  [ -f "$$archive" ] || continue; \
	  found=$$((found + 1)); \
	  symbols=$$(nm -g "$$archive" | c++filt); \
	  ours=$$(printf '%s\n' "$$symbols" | grep -c 'mxpugi::' || true); \
	  total=$$((total + ours)); \
	  leaked=$$(printf '%s\n' "$$symbols" | grep -E '(^|[^A-Za-z0-9_])pugi::' || true); \
	  if [ -n "$$leaked" ]; then \
	    echo "symbol-check: $$archive exports symbols in the stock pugi:: namespace:"; \
	    printf '%s\n' "$$leaked" | head -n 20; \
	    status=1; \
	  else \
	    echo "symbol-check: $$archive: no pugi:: symbols, $$ours mxpugi:: symbols"; \
	  fi; \
	done; \
	if [ "$$found" -eq 0 ]; then \
	  echo "symbol-check: no archives found under $(BUILD_ROOT)/api or $(BUILD_ROOT)/core-dev; build first"; exit 1; \
	fi; \
	if [ "$$total" -eq 0 ]; then \
	  echo "symbol-check: the check did not see an mxpugi:: symbol in $$found archives; it is not looking at real output"; exit 1; \
	fi; \
	[ "$$status" -eq 0 ] || exit 1; \
	echo "symbol-check passed: $$found archives, $$total mxpugi:: symbols, 0 pugi:: symbols."

# mx against a stock pugixml (MX_XML_BACKEND=pugixml). The consumer project
# in cmake/backend-pugixml fetches pugixml 1.16 and adds mx as a
# subdirectory; mx's CMakeLists puts the binaries at the top of that build
# tree. After the core suites, nm proves the mode took: libmx_core.a must
# reference pugi:: and must not contain an mxpugi:: symbol. The test binaries
# still link mxpugi, which is why the archive, not the binaries, is checked.
BACKEND_PUGIXML_DIR := $(BUILD_ROOT)/backend-pugixml

backend-pugixml-test:
	$(CMAKE) -S cmake/backend-pugixml -B $(BACKEND_PUGIXML_DIR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	$(CMAKE) --build $(BACKEND_PUGIXML_DIR) --parallel $(JOBS)
	$(BACKEND_PUGIXML_DIR)/mxtest-core-dev --allow-running-no-tests $(ARGS)
	$(BACKEND_PUGIXML_DIR)/mxtest-core
	@archive=$(BACKEND_PUGIXML_DIR)/libmx_core.a; \
	symbols=$$(nm -g "$$archive" | c++filt); \
	ours=$$(printf '%s\n' "$$symbols" | grep -c 'mxpugi::' || true); \
	theirs=$$(printf '%s\n' "$$symbols" | grep -cE '(^|[^A-Za-z0-9_])pugi::' || true); \
	if [ "$$ours" -ne 0 ]; then \
	  echo "backend-pugixml-test: $$archive still has $$ours mxpugi:: symbols; the pugixml backend did not take"; exit 1; \
	fi; \
	if [ "$$theirs" -eq 0 ]; then \
	  echo "backend-pugixml-test: $$archive does not reference a pugi:: symbol; the pugixml backend did not take"; exit 1; \
	fi; \
	echo "backend-pugixml-test passed: $$archive references $$theirs pugi:: symbols and 0 mxpugi:: symbols."

api-lib:
	$(CMAKE) -S . -B $(BUILD_ROOT)/api -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DMX_API=on
	$(CMAKE) --build $(BUILD_ROOT)/api --target mx --parallel $(JOBS)

api-build: api-lib
	$(CMAKE) --build $(BUILD_ROOT)/api --parallel $(JOBS)

api-test: api-examples
	$(BUILD_ROOT)/api/mxtest $(ARGS)

api-examples: api-build
	$(BUILD_ROOT)/api/mxread
	$(BUILD_ROOT)/api/mxwrite $(BUILD_ROOT)/api/example.musicxml
	$(BUILD_ROOT)/api/mxhide
	@echo 'api-examples: all three examples ran successfully.'

# Corpus api roundtrip -- regression mode (CI gate): every file in the
# pinned baseline must pass; exit non-zero if any fails.
api-roundtrip: api-build
	$(BUILD_ROOT)/api/mxtest-api-roundtrip regression \
		$(CURDIR)/data \
		$(CURDIR)/src/private/mxtest/api/roundtrip-baseline.txt

# Discovery mode: walk the full corpus and report pass/fail.
# Never a CI gate; use to grow the pinned list (manual commits only).
api-roundtrip-discover: api-build
	$(BUILD_ROOT)/api/mxtest-api-roundtrip discovery $(CURDIR)/data

# Dump normalized expected/actual XML for every failing api round-trip.
# Output goes to build/api/roundtrip-dump/ (build dir, already gitignored).
# Feeds the classifier: make api-roundtrip-dump && make api-roundtrip-classify
api-roundtrip-dump: api-build
	rm -rf $(BUILD_ROOT)/api/roundtrip-dump
	mkdir -p $(BUILD_ROOT)/api/roundtrip-dump
	$(BUILD_ROOT)/api/mxtest-api-roundtrip discovery $(CURDIR)/data \
		--dump $(CURDIR)/$(BUILD_ROOT)/api/roundtrip-dump

# Classify api round-trip failures by root cause.
# Reads the dump produced by api-roundtrip-dump; writes build/api/classified.json.
# Fast (pure Python); kept separate from the slow dump step so classification
# logic can be re-run without re-dumping. Pass DUMP_DIR=path to override.
DUMP_DIR ?= $(BUILD_ROOT)/api/roundtrip-dump
api-roundtrip-classify:
	python3 -m audit classify $(DUMP_DIR) \
		--data $(CURDIR)/data \
		--out $(BUILD_ROOT)/api/classified.json

# Instrumented api coverage: build mx+mxtest with --coverage, run all
# suites, produce gcovr report for src/private/mx/{api,impl,utility}/.
api-coverage:
	@echo "=== build (api, instrumented) ==="
	$(CMAKE) -S . -B $(BUILD_ROOT)/cov-api \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DCMAKE_C_COMPILER_LAUNCHER= \
		-DCMAKE_CXX_COMPILER_LAUNCHER= \
		-DMX_API=on \
		-DMX_COVERAGE=on
	$(CMAKE) --build $(BUILD_ROOT)/cov-api --parallel $(JOBS)
	@echo "=== run mxtest + corpus roundtrip ==="
	$(BUILD_ROOT)/cov-api/mxtest
	$(BUILD_ROOT)/cov-api/mxtest-api-roundtrip regression \
		$(CURDIR)/data \
		$(CURDIR)/src/private/mxtest/api/roundtrip-baseline.txt
	@echo "=== gcovr report (src/private/mx/{api,impl,utility}/) ==="
	@mkdir -p $(COV_DIR)/api
	gcovr --root . \
		--gcov-executable '$(GCOV)' \
		--filter 'src/private/mx/api/' \
		--filter 'src/private/mx/impl/' \
		--filter 'src/private/mx/utility/' \
		--print-summary \
		--txt $(COV_DIR)/api/coverage.txt \
		--xml $(COV_DIR)/api/coverage.xml \
		--html-self-contained --html $(COV_DIR)/api/index.html \
		$(BUILD_ROOT)/cov-api | tee $(COV_DIR)/api/summary.txt
	@echo "=== api-coverage written to $(COV_DIR)/api/ ==="

# wasm: mx::api built for Emscripten. ccache wraps emcc via the ambient
# CMAKE_*_COMPILER_LAUNCHER env (Dockerfile), same as every other job.
wasm-lib:
	emcmake $(CMAKE) -S . -B $(BUILD_ROOT)/wasm \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DMX_API=on
	emmake $(CMAKE) --build $(BUILD_ROOT)/wasm --target mx --parallel $(JOBS)

wasm-build: wasm-lib
	emmake $(CMAKE) --build $(BUILD_ROOT)/wasm --target mxread mxwrite mxhide --parallel $(JOBS)

# Runs mxread/mxwrite/mxhide under Node, exercising mx::api, not just a compile.
# mxwrite's output path is bare -- Emscripten's MEMFS starts empty, no build/wasm/ dir.
wasm-test: wasm-build
	node $(BUILD_ROOT)/wasm/mxread.js
	node $(BUILD_ROOT)/wasm/mxwrite.js example.musicxml
	node $(BUILD_ROOT)/wasm/mxhide.js
	@echo 'wasm-test: mxread/mxwrite/mxhide ran successfully under Node (wasm).'

core-build:
	$(CMAKE) -S . -B $(BUILD_ROOT)/core-dev -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DMX_CORE_DEV=on
	$(CMAKE) --build $(BUILD_ROOT)/core-dev --parallel $(JOBS)

core-roundtrip-test: core-build
	$(BUILD_ROOT)/core-dev/mxtest-core-dev --allow-running-no-tests $(ARGS)

core-unit: core-build
	$(BUILD_ROOT)/core-dev/mxtest-core $(ARGS)

# Instrumented core coverage: build with --coverage (off ccache so the
# notes/data stay exact), run the corert AND unit suites, then gcovr. The
# report lands in $(COV_DIR) on the bind-mounted workspace. Filtered to
# src/private/mx/core/ -- the generated model and its runtime.
core-coverage:
	@echo "=== build (core, instrumented) ==="
	$(CMAKE) -S . -B $(BUILD_ROOT)/cov-core-dev \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DCMAKE_C_COMPILER_LAUNCHER= \
		-DCMAKE_CXX_COMPILER_LAUNCHER= \
		-DMX_CORE_DEV=on \
		-DMX_COVERAGE=on
	$(CMAKE) --build $(BUILD_ROOT)/cov-core-dev --parallel $(JOBS)
	@echo "=== run corert + unit suites ==="
	$(BUILD_ROOT)/cov-core-dev/mxtest-core-dev --allow-running-no-tests $(ARGS)
	$(BUILD_ROOT)/cov-core-dev/mxtest-core
	@echo "=== gcovr report (src/private/mx/core/) ==="
	@mkdir -p $(COV_DIR)
	gcovr --root . \
		--gcov-executable '$(GCOV)' \
		--filter 'src/private/mx/core/' \
		--print-summary \
		--txt $(COV_DIR)/coverage.txt \
		--xml $(COV_DIR)/coverage.xml \
		--html-self-contained --html $(COV_DIR)/index.html \
		$(BUILD_ROOT)/cov-core-dev | tee $(COV_DIR)/summary.txt
	@echo "=== coverage written to $(COV_DIR)/ ==="

gen-test:
	python3 -m unittest discover -s gen/tests -t . $(ARGS)

# Audit tool Python tests (feature-audit + the round-trip failure classifier).
audit-test:
	python3 -m unittest discover -s audit/tests -t . $(ARGS)

# The venv for gen-quality and gen-lint. The stamp records which requirements
# file it was built from, so it is rebuilt only when that file changes.
QUALITY_VENV_STAMP := $(QUALITY_VENV)/.mx-requirements-stamp

quality-venv: $(QUALITY_VENV_STAMP)

$(QUALITY_VENV_STAMP): $(QUALITY_REQS)
	python3 -m venv $(QUALITY_VENV)
	$(QUALITY_VENV)/bin/pip install --quiet --disable-pip-version-check -r $(QUALITY_REQS)
	@touch $@

# gen-quality and gen-lint do not build the venv themselves. That would mean a
# network install in the middle of a check. They stop with a hint instead.
CHECK_QUALITY_VENV = @test -x $(QUALITY_VENV)/bin/python || \
	{ echo "No Python venv at $(QUALITY_VENV). Run: make quality-venv"; exit 1; }

cache-stats:
	@if command -v ccache >/dev/null 2>&1; then ccache -s; \
	else echo "ccache is not installed."; fi

# Static analysis of the gen/ generator. quality.py measures and writes the
# report tree to the workspace mount; the floor check below is the gate.
gen-quality:
	$(CHECK_QUALITY_VENV)
	@$(QUALITY_VENV)/bin/python gen/quality.py --floor $(GEN_QUALITY_FLOOR)
	@score=$$(python3 -c "import json; print(json.load(open('$(QUALITY_DIR)/score.json'))['composite'])"); \
	 if awk "BEGIN{exit !($$score < $(GEN_QUALITY_FLOOR))}"; then \
	   echo "ERROR: gen-quality $$score is below floor $(GEN_QUALITY_FLOOR)"; exit 1; \
	 else echo "=== gen-quality OK: $$score >= floor $(GEN_QUALITY_FLOOR) ==="; fi

# pylint as a binary gate (model-A ratchet). --exit-zero so the parsed rating,
# not pylint's message-category exit bitmask, decides pass/fail.
gen-lint:
	$(CHECK_QUALITY_VENV)
	@out=$$($(QUALITY_VENV)/bin/pylint --rcfile=gen/.pylintrc --exit-zero $(GEN_PY) 2>&1); \
	 echo "$$out"; \
	 rating=$$(echo "$$out" | sed -n 's/.*rated at \([0-9.]*\)\/10.*/\1/p'); \
	 if [ -z "$$rating" ]; then echo "ERROR: gen-lint could not parse a pylint rating"; exit 1; fi; \
	 if awk "BEGIN{exit !($$rating < $(GEN_LINT_FLOOR))}"; then \
	   echo "ERROR: gen-lint rating $$rating is below floor $(GEN_LINT_FLOOR)"; exit 1; \
	 else echo "=== gen-lint OK: $$rating >= floor $(GEN_LINT_FLOOR) ==="; fi

fmt:
	@$(FIND_CPP) | xargs -r $(CLANG_FORMAT) -i
	@echo "Formatted C++ under src/."

fmt-check:
	@$(FIND_CPP) | xargs -r $(CLANG_FORMAT) --dry-run --Werror
	@echo "fmt-check passed."

gen: gen-cpp gen-go gen-c gen-schema

gen-cpp:
	python3 -m gen gen/cpp/config.toml

gen-go:
	python3 -m gen gen/test/go/config.toml

gen-c:
	python3 -m gen gen/test/c/config.toml

gen-schema:
	python3 -m gen gen/schema/config.toml

# Feature audit: inventory which MusicXML features the corpus uses, for comparing
# against mx::api support. See audit/README.md and the mx-api-feature-audit skill.
audit:
	python3 -m audit all

audit-force:
	python3 -m audit all --force

build-go:
	cd gen/test/go && MX_REPO_ROOT=$(CURDIR) go test -c -o build/corert-test ./corert/

test-go:
	cd gen/test/go && MX_REPO_ROOT=$(CURDIR) go test -count=1 -v ./corert/ $(ARGS)

build-c:
	$(CMAKE) -S gen/test/c -B gen/test/c/build \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DMX_REPO_ROOT=$(CURDIR)
	$(CMAKE) --build gen/test/c/build --parallel $(JOBS)

test-c: build-c
	gen/test/c/build/corert-c

else

# ----- MX_TOOLCHAIN=docker: build the image, then docker run ----------------

# clean-docker-all in another worktree can delete the image while this
# worktree's stamp stays behind. Treat a missing image as a stale stamp.
ifneq ($(wildcard $(DOCKER_STAMP)),)
ifneq ($(shell command -v $(DOCKER) 2>/dev/null),)
ifeq ($(shell $(DOCKER) image inspect $(DOCKER_IMAGE) >/dev/null 2>&1 && echo found),)
$(shell rm -f $(DOCKER_STAMP))
endif
endif
endif

$(DOCKER_STAMP): Dockerfile $(QUALITY_REQS) | check-docker
	@mkdir -p $(BUILD_ROOT)
	$(DOCKER) buildx build $(DOCKER_CACHE) --load -t $(DOCKER_IMAGE) . \
		|| $(DOCKER) build -t $(DOCKER_IMAGE) .
	@touch $@

sdk: $(DOCKER_STAMP)

# The image has its own venv (see Dockerfile).
quality-venv:
	@echo 'quality-venv is for MX_TOOLCHAIN=native. The mx-sdk image has its own venv.'

docker-volume: | check-docker
	@for v in $(DOCKER_VOLUME) $(DOCKER_CACHE_VOLUME); do \
	  $(DOCKER) volume inspect $$v >/dev/null 2>&1 || $(DOCKER) volume create $$v >/dev/null; \
	done

# Forward each toolchain target into the container, which runs the same make
# target there. A target that needs extra host-side steps, before or after the
# container run, sets them in DOCKER_BEFORE or DOCKER_AFTER below.
$(DOCKER_TARGETS): $(DOCKER_STAMP) docker-volume
	$(DOCKER_BEFORE)
	$(DOCKER_RUN) make $@ BUILD_TYPE=$(BUILD_TYPE) ARGS='$(ARGS)'
	$(DOCKER_AFTER)

# The build volume is not visible from the host, so copy results out of it.
api-roundtrip-dump: DOCKER_AFTER = rm -rf $(BUILD_ROOT)/api/roundtrip-dump && \
	mkdir -p $(BUILD_ROOT)/api/roundtrip-dump && \
	$(DOCKER) run --rm -v $(DOCKER_VOLUME):/vol:ro -v $(CURDIR)/$(BUILD_ROOT)/api/roundtrip-dump:/out \
		alpine sh -c 'cp -a /vol/api/roundtrip-dump/. /out/'

api-roundtrip-classify: DOCKER_AFTER = mkdir -p $(BUILD_ROOT)/api && \
	$(DOCKER) run --rm -v $(DOCKER_VOLUME):/vol:ro -v $(CURDIR)/$(BUILD_ROOT)/api:/out \
		alpine cp /vol/api/classified.json /out/classified.json

# Reports land on the workspace mount; clear old ones first.
api-coverage: DOCKER_BEFORE = @rm -rf $(COV_DIR)/api
api-coverage: DOCKER_AFTER = @echo "API coverage written to $(COV_DIR)/api/ (open $(COV_DIR)/api/index.html)"

core-coverage: DOCKER_BEFORE = @rm -rf $(COV_DIR)
core-coverage: DOCKER_AFTER = @echo "Coverage written to $(COV_DIR)/ (open $(COV_DIR)/index.html)"

gen-quality: DOCKER_BEFORE = @rm -rf $(QUALITY_DIR)

endif
