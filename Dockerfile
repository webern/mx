# syntax=docker/dockerfile:1
#
# mx-sdk: the pinned developer toolchain for mx. Not a project deliverable.

FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
        gcc-14 \
        g++-14 \
        clang-format-18 \
        cmake \
        make \
        ccache \
        gcovr \
        python3 \
        python3-venv \
        golang-go \
        libxml2-dev \
        libxml2-utils \
        pkg-config \
        git \
        ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Python quality tooling for `make gen-quality` / `make gen-lint`. Isolated in a
# venv (Ubuntu 24.04 marks the system Python externally-managed) and version
# pinned: the gen-quality floor compares an exact score, so the analyzers must
# be reproducible across CI runs.
RUN python3 -m venv /opt/quality-venv \
    && /opt/quality-venv/bin/pip install --no-cache-dir \
        radon==6.0.1 \
        astroid==4.0.4 \
        pylint==4.0.5 \
        cognitive_complexity==1.3.0

# Unversioned name so the Makefile invokes the formatter without the suffix.
RUN ln -sf /usr/bin/clang-format-18 /usr/local/bin/clang-format

# emsdk: the Emscripten toolchain, used to build mx to WebAssembly (issue #386;
# see CMakeLists.txt's EMSCRIPTEN block). Pinned to a specific release, not
# "latest", for the same reproducibility reason every other tool in this image
# is version-pinned. emsdk writes its config (.emscripten) inside $EMSDK by
# default (has since 1.39.x) rather than $HOME, so it needs no help from us
# there -- but its system-library cache defaults to living under $EMSDK too,
# which the container's caller-uid:gid user (no passwd entry, root-owned
# /opt/emsdk) can't write to at runtime; EM_CACHE below redirects that.
# node's bundled bin dir has a version number baked into its path; symlink a
# stable name so PATH below doesn't have to guess it.
ARG EMSDK_VERSION=6.0.6
RUN git clone --depth 1 --branch ${EMSDK_VERSION} https://github.com/emscripten-core/emsdk.git /opt/emsdk \
    && /opt/emsdk/emsdk install ${EMSDK_VERSION} \
    && /opt/emsdk/emsdk activate ${EMSDK_VERSION} \
    && ln -s "$(find /opt/emsdk/node -maxdepth 2 -type d -name bin)" /opt/emsdk/node/current-bin

# EM_CACHE (the compiled libc/libc++/compiler-rt system-library cache) is
# redirected to the build volume for the same reason CCACHE_DIR is below: it
# would otherwise try to write under the root-owned /opt/emsdk at runtime.
ENV EMSDK=/opt/emsdk \
    EM_CONFIG=/opt/emsdk/.emscripten \
    EM_CACHE=/workspace/build/.emcache \
    PATH="/opt/emsdk:/opt/emsdk/upstream/emscripten:/opt/emsdk/node/current-bin:${PATH}"

# MX_RUNNING_IN_DOCKER flips the Makefile to its in-container branch. Build with
# the pinned GCC; ccache state lives under the mounted build volume.
ENV MX_RUNNING_IN_DOCKER=1 \
    CC=gcc-14 \
    CXX=g++-14 \
    CMAKE_C_COMPILER_LAUNCHER=ccache \
    CMAKE_CXX_COMPILER_LAUNCHER=ccache \
    CCACHE_DIR=/workspace/build/.ccache \
    CCACHE_MAXSIZE=2G

# The container runs as the caller's uid:gid (DOCKER_USER), which has no passwd
# entry, so HOME defaults to "/". Go would then place GOPATH at /go and GOCACHE
# at /.cache/go-build -- both unwritable -- and `make test-go` dies with
# "could not create module cache: mkdir /go: permission denied". Pin the Go
# caches under the writable build volume, exactly as CCACHE_DIR above. The Go
# module is vendored, so no network fetch is needed.
ENV GOPATH=/workspace/build/go \
    GOCACHE=/workspace/build/go/cache \
    GOMODCACHE=/workspace/build/go/pkg/mod

WORKDIR /workspace
