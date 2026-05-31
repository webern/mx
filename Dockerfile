# syntax=docker/dockerfile:1
#
# Pinned toolchain image for mx quality gates and SDK workflows.
#
# This image is not a deliverable. It is a deterministic environment for the
# `make fmt`, `make check`, `make check-core-dev`, and `make coverage-core-dev`
# workflows: Ubuntu 24.04 with a pinned g++-14 / clang-format-18 toolchain. It
# compiles with GCC (not clang) so the gate matches the GCC the required Linux
# CI jobs use - the same standard library, so a `make check` pass locally means
# the GCC jobs compile too. Pinning keeps clang-format output and the warning
# set deterministic regardless of the floating CI runner image.
#
# The image is built once (tagged `mx-sdk`) and then driven with `docker run`,
# not `docker build`: the Makefile builds it on demand, rebuilds it when this
# file changes, and `docker run`s it with the workspace bind-mounted and a
# named `mx-build` volume mounted at /workspace/build. Source edits and
# incremental build state (objects + ccache) live outside the image and persist
# between runs. Inside the container MX_RUNNING_IN_DOCKER=1 flips the Makefile
# to run the pinned tools directly.
#
# ccache is the CMake compiler launcher. Its store lives under the mounted
# build volume, so it persists across runs locally and - because it is
# content-addressed, not mtime-based - it can be cached across CI runs even
# though `actions/checkout` resets source mtimes.

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
# knowing the version suffix. The compiler is selected via CC/CXX below.
RUN ln -sf /usr/bin/clang-format-18 /usr/local/bin/clang-format

# Build with the pinned GCC so the gate matches the GCC the required Linux CI
# jobs use (same compiler family + libstdc++). CMake reads CC/CXX and the
# *_COMPILER_LAUNCHER variables from the environment at first configure.
# MX_RUNNING_IN_DOCKER tells the Makefile it is inside the container and should
# run the tools directly. CCACHE_DIR lives under the mounted build volume so it
# persists with the rest of the build state.
ENV MX_RUNNING_IN_DOCKER=1 \
    CC=gcc-14 \
    CXX=g++-14 \
    CMAKE_C_COMPILER_LAUNCHER=ccache \
    CMAKE_CXX_COMPILER_LAUNCHER=ccache \
    CCACHE_DIR=/workspace/build/.ccache \
    CCACHE_MAXSIZE=2G

WORKDIR /workspace
