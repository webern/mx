# syntax=docker/dockerfile:1
#
# Developer tools for mx. Not a project deliverable.

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

# Unversioned name so the Makefile invokes the pinned formatter without
# knowing the version suffix.
RUN ln -sf /usr/bin/clang-format-18 /usr/local/bin/clang-format

# Build with the pinned GCC. MX_RUNNING_IN_DOCKER tells the Makefile it
# is inside the container. CCACHE_DIR lives under the mounted build
# volume.
ENV MX_RUNNING_IN_DOCKER=1 \
    CC=gcc-14 \
    CXX=g++-14 \
    CMAKE_C_COMPILER_LAUNCHER=ccache \
    CMAKE_CXX_COMPILER_LAUNCHER=ccache \
    CCACHE_DIR=/workspace/build/.ccache \
    CCACHE_MAXSIZE=2G

WORKDIR /workspace
