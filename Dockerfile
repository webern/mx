# syntax=docker/dockerfile:1
#
# mx-sdk: the pinned developer toolchain for mx. Not a project deliverable.

# deslag is built in its own stage so the image does not carry a Rust
# toolchain, only the binary. The Makefile reads DESLAG_VERSION from the ARG
# below for native installs, so this is the only place the version is pinned.
# Bookworm's glibc is older than Ubuntu 24.04's, so the binary runs there.
FROM rust:1.91.1-slim-bookworm AS deslag
ARG DESLAG_VERSION=0.0.6
RUN cargo install deslag --version ${DESLAG_VERSION} --locked --root /opt/deslag

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

# PATH is appended, not prepended -- emsdk's own dirs shadow real tools by name (node, cmake).
ENV EMSDK=/opt/emsdk \
    EM_CONFIG=/opt/emsdk/.emscripten \
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

COPY --from=deslag /opt/deslag/bin/deslag /usr/local/bin/deslag
ENV DESLAG=/usr/local/bin/deslag

# MX_TOOLCHAIN=native makes the Makefile run the tools directly instead of
# forwarding into Docker again. Build with the pinned GCC through ccache.
ENV MX_TOOLCHAIN=native \
    CC=gcc-14 \
    CXX=g++-14 \
    CMAKE_C_COMPILER_LAUNCHER=ccache \
    CMAKE_CXX_COMPILER_LAUNCHER=ccache

# The compiler caches live in /cache, where the Makefile mounts a volume that
# every worktree shares. On Linux the container runs as the caller's uid:gid
# (DOCKER_USER), not root. Docker gives a new, empty volume the permissions of
# the directory it is mounted over, so /cache is made writable by anyone.
#
# That uid has no passwd entry, so HOME is "/". Left alone, Go would put its
# caches under /go and /.cache, which it cannot write, and Emscripten would
# write its cache under the root-owned /opt/emsdk. So every cache is set here.
# GOPATH stays in the per-worktree build volume: the Go module is vendored, so
# nothing is downloaded into it.
RUN mkdir -p /cache && chmod 1777 /cache
ENV CCACHE_DIR=/cache/ccache \
    CCACHE_MAXSIZE=10G \
    EM_CACHE=/cache/em \
    GOCACHE=/cache/go/build \
    GOPATH=/workspace/build/go \
    GOMODCACHE=/workspace/build/go/pkg/mod

WORKDIR /workspace
