# syntax=docker/dockerfile:1
#
# Pinned quality-gate toolchain for mx.
#
# This image is not a deliverable. It is a deterministic environment for the
# `make fmt` and `make check` quality gates: Ubuntu 24.04 with a pinned
# g++-14 / clang-format-18 toolchain. It compiles with GCC (not clang) so the
# gate matches the GCC the required Linux CI jobs use - the same standard
# library, so a `make check` pass locally means the GCC jobs compile too.
# Pinning keeps clang-format output and the warning set deterministic
# regardless of the floating CI runner image.
#
# The Makefile drives this file: outside the container `make check` runs
# `docker buildx build --target ...`; inside the container (where
# MX_RUNNING_IN_DOCKER=1) the same `make check` runs the tools directly.
#
# See docs/ai/project/build-and-ci-design.md.

FROM ubuntu:24.04 AS base

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
        gcc-14 \
        g++-14 \
        clang-format-18 \
        cmake \
        make \
    && rm -rf /var/lib/apt/lists/*

# Unversioned name so the Makefile invokes the pinned formatter without
# knowing the version suffix. The compiler is selected via CC/CXX below.
RUN ln -sf /usr/bin/clang-format-18 /usr/local/bin/clang-format

# Build with the pinned GCC so the gate matches the GCC the required Linux CI
# jobs use (same compiler family + libstdc++). CMake reads CC/CXX at first
# configure. MX_RUNNING_IN_DOCKER tells the Makefile it is inside the
# container and should run the tools directly.
ENV MX_RUNNING_IN_DOCKER=1 \
    CC=gcc-14 \
    CXX=g++-14

WORKDIR /workspace
COPY . .

# --- run stage: run `make check` for its exit code --------------------------
FROM base AS run
RUN --mount=type=cache,target=/workspace/build make check

# --- fmt stage: format in place, then export only the src tree -------
FROM base AS fmt
RUN --mount=type=cache,target=/workspace/build make fmt

FROM scratch AS fmt-out
COPY --from=fmt /workspace/src /src
