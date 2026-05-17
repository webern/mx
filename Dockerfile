# syntax=docker/dockerfile:1
#
# Pinned quality-gate toolchain for mx.
#
# This image is not a deliverable. It is a deterministic environment for the
# `make fmt`, `make check`, and `make lint` quality gates: Ubuntu 24.04 with
# clang-18 and libc++, so clang-tidy's frontend and the standard-library
# headers come from one toolchain (no clang-vs-libstdc++ false positives) and
# formatting / warning behavior is identical on every machine.
#
# The Makefile drives this file: outside the container `make check` runs
# `docker buildx build` with a target/build-arg; inside the container (where
# MX_RUNNING_IN_DOCKER=1) the same `make check` runs the tools directly.
#
# See Documents/ai/project/build-and-ci-design.md.

FROM ubuntu:24.04 AS base

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
        clang-18 \
        clang-tidy-18 \
        clang-format-18 \
        libc++-18-dev \
        libc++abi-18-dev \
        cmake \
        make \
    && rm -rf /var/lib/apt/lists/*

# Unversioned names so the Makefile and CMake invoke the pinned tools without
# knowing the version suffix.
RUN ln -sf /usr/bin/clang-18        /usr/local/bin/clang \
 && ln -sf /usr/bin/clang++-18      /usr/local/bin/clang++ \
 && ln -sf /usr/bin/clang-tidy-18   /usr/local/bin/clang-tidy \
 && ln -sf /usr/bin/clang-format-18 /usr/local/bin/clang-format

# Build with clang + libc++. CMake reads CC/CXX/CXXFLAGS/LDFLAGS at first
# configure. MX_RUNNING_IN_DOCKER tells the Makefile it is inside the
# container and should run the tools directly.
ENV MX_RUNNING_IN_DOCKER=1 \
    CC=clang \
    CXX=clang++ \
    CXXFLAGS=-stdlib=libc++ \
    LDFLAGS=-stdlib=libc++

WORKDIR /workspace
COPY . .

# --- run stage: execute a make target for its exit code (check / lint) ------
FROM base AS run
ARG MX_TARGET=check
RUN --mount=type=cache,target=/workspace/build make "$MX_TARGET"

# --- fmt stage: format in place, then export only the Sourcecode tree -------
FROM base AS fmt
RUN --mount=type=cache,target=/workspace/build make fmt

FROM scratch AS fmt-out
COPY --from=fmt /workspace/Sourcecode /Sourcecode
