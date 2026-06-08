# mx build/check driver -- clean-slate spike.
# ============================================================================
#
# The legacy library and generator were removed on the `newgen` branch; the
# generator and most of the library are being rebuilt from scratch (see
# AGENTS.md). What survives is the structural skeleton: the embedded ezxml XML
# layer and the core roundtrip (corert) test harness. corert does not compile
# until the new generator emits src/private/mx/core -- expected for now.
#
# Native targets (ezxml/core-dev/test-core-dev) drive CMake directly. The fmt
# and check gates run inside the pinned `mx-sdk` Docker toolchain, built once
# and bind-mounting the workspace. Requires CMake >= 3.13.
# ============================================================================

CMAKE      ?= cmake
DOCKER     ?= docker
BUILD_TYPE ?= Debug
BUILD_ROOT := build
JOBS       ?= $(shell nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

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
.PHONY: help sdk fmt check ezxml core-dev test-core-dev \
        clean clean-docker check-docker docker-volume

help:
	@echo 'mx targets (clean-slate spike -- see AGENTS.md):'
	@echo '  make ezxml          Build the embedded ezxml XML layer.'
	@echo '  make core-dev       Build the corert binary (fails until mx/core is regenerated).'
	@echo "  make test-core-dev  Run the core roundtrip suite. Filter: ARGS='[core-roundtrip] lysuite/*'"
	@echo '  make fmt            Format C++ under src/ via mx-sdk.'
	@echo '  make check          fmt-check via mx-sdk.'
	@echo '  make sdk            Build the mx-sdk Docker toolchain image.'
	@echo '  make clean          Remove the build/ tree.'
	@echo '  make clean-docker   Remove the sdk image and build volume.'

# --- Native builds ----------------------------------------------------------

ezxml:
	$(CMAKE) -S . -B $(BUILD_ROOT)/dev -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	$(CMAKE) --build $(BUILD_ROOT)/dev --parallel $(JOBS) --target mx-ezxml

core-dev:
	$(CMAKE) -S . -B $(BUILD_ROOT)/core-dev -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DMX_CORE_DEV=on
	$(CMAKE) --build $(BUILD_ROOT)/core-dev --parallel $(JOBS)

# --allow-running-no-tests keeps the run green once corert builds but before any
# data/ case is wired up. (Until mx/core exists, core-dev above fails first.)
test-core-dev: core-dev
	$(BUILD_ROOT)/core-dev/mxtest-core-dev --allow-running-no-tests $(ARGS)

# --- Housekeeping -----------------------------------------------------------

clean:
	rm -rf $(BUILD_ROOT)

clean-docker:
	-rm -f $(DOCKER_STAMP)
	-$(DOCKER) rmi $(DOCKER_IMAGE) 2>/dev/null || true
	-$(DOCKER) volume rm $(DOCKER_VOLUME) 2>/dev/null || true
	@echo "Removed mx-sdk image and mx-build volume."

check-docker:
	@command -v $(DOCKER) >/dev/null 2>&1 || \
		{ echo "Docker not found. Install it to use the mx-sdk gates:"; \
		  echo "  https://docs.docker.com/get-docker/"; exit 1; }

ifdef MX_RUNNING_IN_DOCKER

# ===== Inside the container: run the pinned tools directly ==================

fmt:
	@$(FIND_CPP) | xargs -r clang-format -i
	@echo "Formatted C++ under src/."

check:
	@$(FIND_CPP) | xargs -r clang-format --dry-run --Werror
	@echo "fmt-check passed."

else

# ===== Outside the container: build the image once, then docker run ========

$(DOCKER_STAMP): Dockerfile | check-docker
	@mkdir -p $(BUILD_ROOT)
	$(DOCKER) build -t $(DOCKER_IMAGE) .
	@touch $@

sdk: $(DOCKER_STAMP)

docker-volume: | check-docker
	@$(DOCKER) volume inspect $(DOCKER_VOLUME) >/dev/null 2>&1 \
		|| $(DOCKER) volume create $(DOCKER_VOLUME) >/dev/null

fmt: $(DOCKER_STAMP) docker-volume
	$(DOCKER_RUN) make fmt

check: $(DOCKER_STAMP) docker-volume
	$(DOCKER_RUN) make check

endif
