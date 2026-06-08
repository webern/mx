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
        python3 \
        python3-venv \
        golang-go \
        libxml2-dev \
        pkg-config \
    && rm -rf /var/lib/apt/lists/*

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

WORKDIR /workspace
