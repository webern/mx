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
    && rm -rf /var/lib/apt/lists/*

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
