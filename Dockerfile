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
        xz-utils \
    && rm -rf /var/lib/apt/lists/*

# Unversioned name, for running the formatter by hand in the container.
RUN ln -sf /usr/bin/clang-format-18 /usr/local/bin/clang-format

# emsdk: pinned Emscripten toolchain for building mx to WebAssembly (#386).
ARG EMSDK_VERSION=6.0.6
RUN git clone --depth 1 --branch ${EMSDK_VERSION} https://github.com/emscripten-core/emsdk.git /opt/emsdk \
    && /opt/emsdk/emsdk install ${EMSDK_VERSION} \
    && /opt/emsdk/emsdk activate ${EMSDK_VERSION} \
    && ln -s "$(find /opt/emsdk/node -maxdepth 2 -type d -name bin)" /opt/emsdk/node/current-bin \
    && rm -rf /opt/emsdk/downloads

# EM_CACHE: like CCACHE_DIR, avoids writing under root-owned /opt/emsdk at runtime.
# PATH is appended, not prepended -- emsdk's own dirs shadow real tools by name (node, cmake).
ENV EMSDK=/opt/emsdk \
    EM_CONFIG=/opt/emsdk/.emscripten \
    EM_CACHE=/workspace/build/.emcache \
    PATH="${PATH}:/opt/emsdk/node/current-bin:/opt/emsdk/upstream/emscripten"

# Python quality tooling for `make gen-quality` / `make gen-lint`. Isolated in a
# venv (Ubuntu 24.04 marks the system Python externally-managed) and pinned in
# gen/requirements-quality.txt: the gen-quality floor compares an exact score,
# so the analyzers must be reproducible across CI runs. This comes after emsdk
# so a change to the requirements does not reinstall emsdk.
COPY gen/requirements-quality.txt /opt/requirements-quality.txt
RUN python3 -m venv /opt/quality-venv \
    && /opt/quality-venv/bin/pip install --no-cache-dir -r /opt/requirements-quality.txt
ENV QUALITY_VENV=/opt/quality-venv

# MX_TOOLCHAIN=native makes the Makefile run the tools directly instead of
# forwarding into Docker again. Build with the pinned GCC; ccache state lives
# under the mounted build volume.
ENV MX_TOOLCHAIN=native \
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
