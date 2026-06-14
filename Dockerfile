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
