# mx build/check driver.
# ============================================================================
#
# The generated mx::core C++ model lives in src/private/mx/core/generated
# (emitted by `make gen-cpp`, committed). Every gate runs through the pinned
# `mx-sdk` Docker toolchain: outside a container a target builds the image
# once and re-invokes itself inside it; inside (MX_RUNNING_IN_DOCKER, set by
# the image) it drives the tools directly. Setting MX_RUNNING_IN_DOCKER=1 on
# a dev machine runs natively with the host toolchain instead (used by the
# macOS CI job, which has no Docker). Requires CMake >= 3.13.
#
# ============================================================================

CMAKE      ?= cmake
DOCKER     ?= docker
BUILD_TYPE ?= Debug
BUILD_ROOT := build
JOBS       ?= $(shell nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

# Coverage report output. gcov-14 matches the pinned g++-14 toolchain.
COV_DIR := data/testOutput/coverage
GCOV    ?= gcov-14

# gen-quality / gen-lint: static analysis of the gen/ generator. The floors are
# the CI gate -- a build fails below them; ratchet them upward as the generator
# improves. gen-quality scores design (size + complexity); gen-lint enforces
# genuine lint defects. Pinned analyzers live in the mx-sdk venv (see
# Dockerfile). GEN_PY is every .py under gen/ except the measurer itself.
QUALITY_VENV      := /opt/quality-venv
QUALITY_DIR       := data/testOutput/gen-quality
GEN_PY            := $(shell find gen -name '*.py' ! -name quality.py | sort)
GEN_QUALITY_FLOOR ?= 84.5
GEN_LINT_FLOOR    ?= 9.29

# For GitHub Actions. When present, push/pull the Docker layer cache to the
# GitHub Actions cache.
ifneq ($(ACTIONS_RUNTIME_TOKEN),)
DOCKER_CACHE := --cache-from type=gha --cache-to type=gha,mode=max
endif

# Docker SDK image + build volume. Incremental state persists across runs.
DOCKER_IMAGE  := mx-sdk
DOCKER_VOLUME := mx-build
DOCKER_STAMP  := $(BUILD_ROOT)/.docker-image-stamp

# Prevent root-owned files on Linux.
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
DOCKER_USER := --user $(shell id -u):$(shell id -g)
endif

DOCKER_RUN := $(DOCKER) run --rm \
	-v $(CURDIR):/workspace \
	-v $(DOCKER_VOLUME):/workspace/build \
	$(DOCKER_USER) \
	$(DOCKER_IMAGE)

# C++ to format. Skip vendored pugixml and the cpul/Catch test harness.
FIND_CPP := find src \
	-path 'src/private/cpul' -prune -o \
	-name 'pugixml.cpp' -prune -o \
	-name 'pugixml.hpp' -prune -o \
	-name 'pugiconfig.hpp' -prune -o \
	-type f \( -name '*.cpp' -o -name '*.h' -o -name '*.hpp' \) -print

.DEFAULT_GOAL := help
.PHONY: help sdk fmt check core-dev check-core-dev test-core-dev test-cpp-unit \
        validate-cpp probe-cpp coverage-core-dev test-gen gen-check \
        gen-quality gen-lint \
        gen gen-cpp gen-go gen-c gen-schema \
        build-go build-c test-go test-c \
        lib dev test examples-run test-api-roundtrip discover-api-roundtrip coverage-api \
        clean clean-docker check-docker docker-volume

help:
	@echo 'mx targets (see AGENTS.md). All run via the mx-sdk Docker toolchain;'
	@echo 'set MX_RUNNING_IN_DOCKER=1 to run natively on the host instead.'
	@echo ''
	@echo '  mx::api/mx::impl:'
	@echo '  make lib                Build the mx static library (MX_API=ON).'
	@echo '  make dev                Build mx + mxtest + examples + api-roundtrip binary.'
	@echo '  make test               Run the mxtest suite (api/impl/file/control).'
	@echo '  make examples-run       Build and run all three api example programs.'
	@echo '  make test-api-roundtrip Run the corpus api roundtrip in regression mode (CI gate).'
	@echo '  make discover-api-roundtrip  Run discovery mode over the full corpus (manual only).'
	@echo '  make coverage-api            Instrumented api/impl/utility build + gcovr report.'
	@echo ''
	@echo '  C++ core:'
	@echo '  make core-dev           Build mx_core and the corert/unit/validate binaries.'
	@echo "  make test-core-dev      Run the core roundtrip suite. Filter: ARGS='[core-roundtrip] lysuite/*'"
	@echo '  make test-cpp-unit      Run the mx::core unit tests (values, shapes, rejection suite).'
	@echo '  make validate-cpp       Serialize every corpus file and xmllint-validate the output.'
	@echo '  make probe-cpp          Compile-time negative probes (invalid construction must not compile).'
	@echo '  make check-core-dev     fmt-check + warning-free core-dev build.'
	@echo '  make coverage-core-dev  Instrumented build, corert + unit suites, gcovr -> $(COV_DIR)/.'
	@echo ''
	@echo '  Generator:'
	@echo '  make test-gen       Run the generator (parser + IR + plates + press) Python tests.'
	@echo '  make gen-check      plates --check for every target (renames, collisions).'
	@echo '  make gen            Run the generator for every target (cpp/go/c/schema).'
	@echo '  make gen-cpp        Run the generator for the C++ target (src/private/mx/core/generated).'
	@echo '  make gen-go         Run the generator for the Go target.'
	@echo '  make gen-c          Run the generator for the C target.'
	@echo '  make gen-schema     Run the generator for the JSON Schema target.'
	@echo '  make gen-quality    Score gen/ design quality; fail below the floor.'
	@echo '  make gen-lint       Lint gen/ with pylint; fail below the floor.'
	@echo ''
	@echo '  Go test target:'
	@echo '  make build-go       Build Go corert tests.'
	@echo '  make test-go        Run Go corert tests.'
	@echo ''
	@echo '  C test target:'
	@echo '  make build-c        Build C corert test binary.'
	@echo '  make test-c         Run C corert tests.'
	@echo ''
	@echo '  Housekeeping:'
	@echo '  make fmt            Format C++ under src/ via mx-sdk.'
	@echo '  make check          fmt-check via mx-sdk.'
	@echo '  make sdk            Build the mx-sdk Docker toolchain image.'
	@echo '  make clean          Remove the build/ tree and gen build artifacts.'
	@echo '  make clean-docker   Remove the sdk image and build volume.'

# --- Housekeeping -----------------------------------------------------------

clean:
	rm -rf $(BUILD_ROOT)
	rm -rf gen/test/c/build
	rm -rf gen/test/go/build

clean-docker:
	-rm -f $(DOCKER_STAMP)
	-$(DOCKER) rmi $(DOCKER_IMAGE) 2>/dev/null || true
	-$(DOCKER) volume rm $(DOCKER_VOLUME) 2>/dev/null || true
	@echo "Removed mx-sdk image and mx-build volume."

check-docker:
	@command -v $(DOCKER) >/dev/null 2>&1 || \
		{ echo "Docker not found. Install it to use the mx-sdk gates:"; \
		  echo "  https://docs.docker.com/get-docker/"; exit 1; }

# === Docker-gated targets ===================================================

ifdef MX_RUNNING_IN_DOCKER

# ----- Inside the container (or native via MX_RUNNING_IN_DOCKER=1) ----------

lib:
	$(CMAKE) -S . -B $(BUILD_ROOT)/api -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DMX_API=on
	$(CMAKE) --build $(BUILD_ROOT)/api --target mx --parallel $(JOBS)

dev: lib
	$(CMAKE) --build $(BUILD_ROOT)/api --parallel $(JOBS)

test: dev
	$(BUILD_ROOT)/api/mxtest $(ARGS)

examples-run: dev
	$(BUILD_ROOT)/api/mxread
	$(BUILD_ROOT)/api/mxwrite $(BUILD_ROOT)/api/example.musicxml
	$(BUILD_ROOT)/api/mxhide
	@echo 'examples-run: all three examples ran successfully.'

# Corpus api roundtrip -- regression mode (CI gate): every file in the
# pinned baseline must pass; exit non-zero if any fails.
test-api-roundtrip: dev
	$(BUILD_ROOT)/api/mxtest-api-roundtrip regression \
		$(CURDIR)/data \
		$(CURDIR)/src/private/mxtest/api/roundtrip-baseline.txt

# Discovery mode: walk the full corpus and report pass/fail.
# Never a CI gate; use to grow the pinned list (manual commits only).
discover-api-roundtrip: dev
	$(BUILD_ROOT)/api/mxtest-api-roundtrip discovery $(CURDIR)/data

# Instrumented api coverage: build mx+mxtest with --coverage, run all
# suites, produce gcovr report for src/private/mx/{api,impl,utility}/.
coverage-api:
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
	@echo "=== coverage-api written to $(COV_DIR)/api/ ==="

core-dev:
	$(CMAKE) -S . -B $(BUILD_ROOT)/core-dev -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DMX_CORE_DEV=on
	$(CMAKE) --build $(BUILD_ROOT)/core-dev --parallel $(JOBS)

test-core-dev: core-dev
	$(BUILD_ROOT)/core-dev/mxtest-core-dev --allow-running-no-tests $(ARGS)

test-cpp-unit: core-dev
	$(BUILD_ROOT)/core-dev/mxtest-core $(ARGS)

# Serialize every parsed corpus document and xmllint-validate the output
# against the MusicXML 4.0 XSD -- mechanical proof that import leniency
# (value clamping) still emits only schema-valid XML.
validate-cpp: core-dev
	rm -rf $(BUILD_ROOT)/validate-out
	$(BUILD_ROOT)/core-dev/mxtest-validate $(BUILD_ROOT)/validate-out
	cd $(BUILD_ROOT)/validate-out && xmllint --noout --nonet \
		--schema $(CURDIR)/docs/musicxml-4.0-ed15c23.xsd *.xml 2>&1 \
		| grep -v 'validates$$' | grep -v 'Skipping import' || true
	@cd $(BUILD_ROOT)/validate-out && xmllint --noout --nonet \
		--schema $(CURDIR)/docs/musicxml-4.0-ed15c23.xsd *.xml 2>/dev/null \
		&& echo 'validate-cpp: all outputs are schema-valid.'

# Compile-time negative probes: PROBE=0 must compile (the control);
# every numbered probe is an invalid-construction attempt that must NOT
# compile.
PROBE_COUNT := 7
probe-cpp:
	@$(CXX) -std=c++20 -fsyntax-only -I src/private -DPROBE=0 \
		src/private/mxtest/probe/Probes.cpp \
		|| { echo 'probe-cpp: control failed to compile'; exit 1; }
	@for i in $$(seq 1 $(PROBE_COUNT)); do \
		if $(CXX) -std=c++20 -fsyntax-only -I src/private -DPROBE=$$i \
			src/private/mxtest/probe/Probes.cpp 2>/dev/null; then \
			echo "probe-cpp: PROBE=$$i compiled but must not"; exit 1; \
		fi; \
	done
	@echo 'probe-cpp: control compiles; all $(PROBE_COUNT) must-not-compile probes fail as required.'

# fmt-check + warning-free core-dev build (a fresh tree so stale objects
# cannot mask a warning; same shape as the old master gate, new subject).
check-core-dev:
	@echo "=== fmt-check ==="
	@$(FIND_CPP) | xargs -r clang-format --dry-run --Werror
	@echo "=== build (warning-free, core-dev) ==="
	@$(CMAKE) -S . -B $(BUILD_ROOT)/check-core-dev \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DMX_CORE_DEV=on \
		> $(BUILD_ROOT)/check-build.log 2>&1 \
		|| { cat $(BUILD_ROOT)/check-build.log; \
		     echo "ERROR: cmake configure failed (see above)"; exit 1; }
	@$(CMAKE) --build $(BUILD_ROOT)/check-core-dev --parallel $(JOBS) \
		>> $(BUILD_ROOT)/check-build.log 2>&1; status=$$?; \
		cat $(BUILD_ROOT)/check-build.log; \
		if [ $$status -ne 0 ]; then \
			echo "ERROR: build failed (see above)"; exit $$status; \
		fi; \
		if grep -q 'warning:' $(BUILD_ROOT)/check-build.log; then \
			echo "ERROR: build emitted warnings (see above)"; exit 1; \
		fi
	@echo "=== check-core-dev passed ==="

# Instrumented core-dev coverage: build with --coverage (off ccache so the
# notes/data stay exact), run the corert AND unit suites, then gcovr. The
# report lands in $(COV_DIR) on the bind-mounted workspace. Filtered to
# src/private/mx/core/ -- the generated model and its runtime.
coverage-core-dev:
	@echo "=== build (core-dev, instrumented) ==="
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

test-gen:
	python3 -m unittest discover -s gen/tests -t . $(ARGS)

# plates --check for every target: validates renames and detects identifier
# collisions (a CI gate, like test-gen).
gen-check:
	python3 -m gen plates --config gen/cpp/config.toml --check
	python3 -m gen plates --config gen/test/go/config.toml --check
	python3 -m gen plates --config gen/test/c/config.toml --check
	python3 -m gen plates --config gen/schema/config.toml --check

# Static analysis of the gen/ generator. quality.py measures and writes the
# report tree to the workspace mount; the floor check below is the gate.
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

fmt:
	@$(FIND_CPP) | xargs -r clang-format -i
	@echo "Formatted C++ under src/."

check:
	@$(FIND_CPP) | xargs -r clang-format --dry-run --Werror
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

# ----- Outside the container: build image, then docker run ------------------

$(DOCKER_STAMP): Dockerfile | check-docker
	@mkdir -p $(BUILD_ROOT)
	$(DOCKER) buildx build $(DOCKER_CACHE) --load -t $(DOCKER_IMAGE) . \
		|| $(DOCKER) build -t $(DOCKER_IMAGE) .
	@touch $@

sdk: $(DOCKER_STAMP)

docker-volume: | check-docker
	@$(DOCKER) volume inspect $(DOCKER_VOLUME) >/dev/null 2>&1 \
		|| $(DOCKER) volume create $(DOCKER_VOLUME) >/dev/null

lib: $(DOCKER_STAMP) docker-volume
	$(DOCKER_RUN) make lib BUILD_TYPE=$(BUILD_TYPE)

dev: $(DOCKER_STAMP) docker-volume
	$(DOCKER_RUN) make dev BUILD_TYPE=$(BUILD_TYPE)

test: $(DOCKER_STAMP) docker-volume
	$(DOCKER_RUN) make test BUILD_TYPE=$(BUILD_TYPE) ARGS='$(ARGS)'

examples-run: $(DOCKER_STAMP) docker-volume
	$(DOCKER_RUN) make examples-run BUILD_TYPE=$(BUILD_TYPE)

test-api-roundtrip: $(DOCKER_STAMP) docker-volume
	$(DOCKER_RUN) make test-api-roundtrip BUILD_TYPE=$(BUILD_TYPE)

discover-api-roundtrip: $(DOCKER_STAMP) docker-volume
	$(DOCKER_RUN) make discover-api-roundtrip BUILD_TYPE=$(BUILD_TYPE)

coverage-api: $(DOCKER_STAMP) docker-volume
	@rm -rf $(COV_DIR)/api
	$(DOCKER_RUN) make coverage-api BUILD_TYPE=$(BUILD_TYPE) ARGS='$(ARGS)'
	@echo "API coverage written to $(COV_DIR)/api/ (open $(COV_DIR)/api/index.html)"

core-dev: $(DOCKER_STAMP) docker-volume
	$(DOCKER_RUN) make core-dev BUILD_TYPE=$(BUILD_TYPE)

test-core-dev: $(DOCKER_STAMP) docker-volume
	$(DOCKER_RUN) make test-core-dev BUILD_TYPE=$(BUILD_TYPE) ARGS='$(ARGS)'

test-cpp-unit: $(DOCKER_STAMP) docker-volume
	$(DOCKER_RUN) make test-cpp-unit BUILD_TYPE=$(BUILD_TYPE) ARGS='$(ARGS)'

validate-cpp: $(DOCKER_STAMP) docker-volume
	$(DOCKER_RUN) make validate-cpp BUILD_TYPE=$(BUILD_TYPE)

probe-cpp: $(DOCKER_STAMP) docker-volume
	$(DOCKER_RUN) make probe-cpp

check-core-dev: $(DOCKER_STAMP) docker-volume
	$(DOCKER_RUN) make check-core-dev BUILD_TYPE=$(BUILD_TYPE)

coverage-core-dev: $(DOCKER_STAMP) docker-volume
	@rm -rf $(COV_DIR)
	$(DOCKER_RUN) make coverage-core-dev BUILD_TYPE=$(BUILD_TYPE) ARGS='$(ARGS)'
	@echo "Coverage written to $(COV_DIR)/ (open $(COV_DIR)/index.html)"

test-gen: $(DOCKER_STAMP)
	$(DOCKER_RUN) make test-gen ARGS='$(ARGS)'

gen-check: $(DOCKER_STAMP)
	$(DOCKER_RUN) make gen-check

gen-quality: $(DOCKER_STAMP)
	@rm -rf $(QUALITY_DIR)
	$(DOCKER_RUN) make gen-quality

gen-lint: $(DOCKER_STAMP)
	$(DOCKER_RUN) make gen-lint

fmt: $(DOCKER_STAMP) docker-volume
	$(DOCKER_RUN) make fmt

check: $(DOCKER_STAMP) docker-volume
	$(DOCKER_RUN) make check

gen: $(DOCKER_STAMP) docker-volume
	$(DOCKER_RUN) make gen

gen-cpp: $(DOCKER_STAMP) docker-volume
	$(DOCKER_RUN) make gen-cpp

gen-go: $(DOCKER_STAMP) docker-volume
	$(DOCKER_RUN) make gen-go

gen-c: $(DOCKER_STAMP) docker-volume
	$(DOCKER_RUN) make gen-c

gen-schema: $(DOCKER_STAMP) docker-volume
	$(DOCKER_RUN) make gen-schema

build-go: $(DOCKER_STAMP) docker-volume
	$(DOCKER_RUN) make build-go

test-go: $(DOCKER_STAMP) docker-volume
	$(DOCKER_RUN) make test-go ARGS='$(ARGS)'

build-c: $(DOCKER_STAMP) docker-volume
	$(DOCKER_RUN) make build-c BUILD_TYPE=$(BUILD_TYPE)

test-c: $(DOCKER_STAMP) docker-volume
	$(DOCKER_RUN) make test-c BUILD_TYPE=$(BUILD_TYPE)

endif
