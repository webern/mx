# Build and CI Design

## Overview

This document describes the formatting, compiler-warning enforcement, and CI pipeline for mx. It
also defines the quality gates that coding agents must satisfy when modifying `src/`.

### Note on `mx/core/`

The files in `src/private/mx/core/` were originally machine-generated from the MusicXML XSD.
The codegen program no longer exists. These files are treated as normal hand-edited source for the
current gates (formatting and compiler warnings) and must pass them like any other code. They are
not linted (see Future Work). A future codegen rewrite will re-own these files and emit code that
passes the gates from the start.

### Design Principle: One Authoritative Toolchain

Compiler warning sets and clang-format behavior change between tool versions. CI runners
(`ubuntu-latest`, `macos-latest`, `windows-latest`) float their toolchain versions, so a warning or
formatting CI break can occur with no code change at all.

To eliminate that fragility, the quality-gate tooling (clang-format and the warning-enforcing
compiler) runs inside a Docker container with pinned tool versions. This gives deterministic,
reproducible results on any machine - local or CI. Platform-specific CI jobs only build and test
with their native toolchains; they enforce no quality gates.

* * *

## Toolchain (Docker)

The quality-gate tools are pinned inside a Docker image built from the `Dockerfile` at the repo
root. The image is Ubuntu 24.04 with **g++-14** and clang-format-18. The gate compiles with GCC (not
clang) so it uses the same compiler family and standard library (libstdc++) as the required Linux CI
jobs - a `make check` pass locally therefore means the GCC jobs compile too. clang-tidy was
previously the reason to use clang + libc++ here; with clang-tidy removed (see Future Work), GCC is
the better choice because it removes the local-vs-CI portability blind spot. Pinning keeps the
`-Wall -Wextra` warning set deterministic.

### Formatting: clang-format

Style base: **Microsoft**. This is the closest standard clang-format style to the existing codebase:
4-space indent, Allman braces (opening `{` on its own line for namespaces, classes, and functions).
The one visible change from the historical style is the removal of spaces inside parentheses
(`func( arg )` becomes `func(arg)`).

A `.clang-format` file at the repo root encodes this style. All C++ files under `src/` are
formatted, including generated files in `mx/core/`.

### Linting (deferred)

clang-tidy is **not** a current quality gate. It was evaluated and removed because running it across
the whole tree is not viable. See Future Work for the rationale and the intended scoped
reintroduction.

### Compiler Warnings

`CMakeLists.txt` adds `-Wall -Wextra` (GCC/Clang) or `/W4` (MSVC) to the `mx` target. Inside the
Docker container, g++-14 is the compiler. `make check` treats any `warning:` line in the build
output as a failure, and also fails on configure or build errors.

### CMake Version

`CMakeLists.txt` requires CMake >= 3.13. The Makefile uses `-S`/`-B` and `--build --parallel`, both
of which require 3.13+.

* * *

## Docker

### Dockerfile

Located at the repo root. Based on Ubuntu 24.04 with pinned clang-18 packages:

- `gcc-14`, `g++-14` - compiler and libstdc++ (matches the required Linux CI jobs)
- `clang-format-18` - formatting
- `cmake`, `make` - build tools

The Dockerfile sets `ENV MX_RUNNING_IN_DOCKER=1`. The Makefile checks this variable to decide
whether to run tools directly or to launch Docker. A `.dockerignore` allowlist keeps the build
context to the source tree and build configuration only.

### BuildKit Cache

The Dockerfile uses `RUN --mount=type=cache,target=/workspace/build` for the CMake build directory.
This persists the incremental build cache across Docker invocations on the same machine, avoiding
full recompiles on every `make check` run.

### Makefile Docker Integration

The Makefile detects `MX_RUNNING_IN_DOCKER`:

- **Inside the container** (`MX_RUNNING_IN_DOCKER=1`): runs clang-format and the compiler directly.
- **Outside the container**: runs `docker buildx build` to build the image and execute the requested
  target inside it.

For `make fmt`, which needs to write formatted files back to the host, the Makefile uses
`docker buildx build --output type=local,dest=.` to extract the formatted `src/` tree from a
`scratch` export stage (only `src/` is written back).

For `make check`, only the exit code matters - the build runs with `--output type=cacheonly`.

* * *

## Makefile Targets

### Developer Workflow

The standard workflow for any code change under `src/`:

```
make fmt && make check && make test
```

If the change touches files under `src/private/mx/core/`:

```
make fmt && make check && make test-all
```

These three commands are all a developer or AI agent needs to verify their work. `fmt` and `check`
run in Docker (deterministic, no local tool installation required). `test` and `test-all` run
natively with the local compiler.

### Quality Targets (Docker)

| Target              | What it does                                                  |
|---------------------|---------------------------------------------------------------|
| `make fmt`          | Formats all C++ files under `src/` in-place via Docker |
| `make check`        | Quality gate: fmt-check + warning-free build (Docker)         |
| `make clean-docker` | Removes the Docker buildx cache                               |

`check-docker` is an internal prerequisite that verifies Docker is available on `PATH`.

### Build Targets (Native)

| Target      | What it does                                          |
|-------------|-------------------------------------------------------|
| `make lib`  | Build just the static library (no tests, no examples) |
| `make dev`  | Build tests (no slow core tests) + examples           |
| `make core` | Build the full suite including slow `mx::core` tests  |

### Run Targets (Native)

| Target              | What it does                                              |
|---------------------|-----------------------------------------------------------|
| `make test`         | Build dev, run examples + mxtest. `ARGS=` forwarded       |
| `make test-all`     | Build core, run examples + full mxtest. `ARGS=` forwarded |
| `make examples-run` | Build dev, then run mxread/mxwrite/mxhide                 |
| `make all`          | Build core, run examples, run full mxtest                 |

### Xcode Targets (Native)

| Target             | What it does                                  |
|--------------------|-----------------------------------------------|
| `make xcode-gen`   | Runs `cmake -G Xcode -S . -B build/xcode`     |
| `make xcode-build` | Builds the generated project via `xcodebuild` |
| `make xcode-test`  | Runs tests via `xcodebuild test`              |

### Housekeeping

| Target              | What it does                    |
|---------------------|---------------------------------|
| `make clean`        | Remove the entire `build/` tree |
| `make clean-docker` | Remove the Docker buildx cache  |

### Knobs

| Variable     | Default          | Purpose                      |
|--------------|------------------|------------------------------|
| `JOBS`       | auto-detected    | Parallel compile jobs        |
| `BUILD_TYPE` | `Debug`          | CMake build type             |
| `GENERATOR`  | platform default | CMake generator override     |
| `ARGS`       | (none)           | Forwarded to mxtest (Catch2) |
| `DOCKER`     | `docker`         | Docker executable            |

* * *

## CI Design

### File

`.github/workflows/ci.yaml` - the primary workflow.

### Triggers

```yaml
on:
  pull_request:
  push:
    branches: [master]
```

CI runs on every PR update and on every push to `master`.

### Jobs

#### linux-gate (required - quality gate + tests)

Runner: `ubuntu-latest`

| Step         | Command      |
|--------------|--------------|
| Quality gate | `make check` |
| Run tests    | `make test`  |

The Makefile handles Docker internally - CI just runs `make check`. The Docker image is built from
the repo's `Dockerfile` with BuildKit layer caching via GitHub Actions cache (the Makefile
auto-detects `ACTIONS_RUNTIME_TOKEN` and enables the gha cache only when present).

This is the authoritative quality gate. Formatting and compiler warnings are enforced here with
pinned tool versions.

#### linux-core (required - full test suite with GCC)

Runner: `ubuntu-latest`

| Step            | Command         |
|-----------------|-----------------|
| Full test suite | `make test-all` |

Builds and runs the complete test suite including the slow `mx::core` tests using GCC (the system
compiler). The Docker gate job runs `make test` (no core tests); this job runs `make test-all` and
provides full test-suite coverage.

#### macos (required - build + tests)

Runner: `macos-latest`

| Step      | Command     |
|-----------|-------------|
| Run tests | `make test` |

Builds and tests with the system clang. `make test` runs the example programs in addition to
`mxtest`, so the examples are exercised on every platform that runs tests (not just macOS). No
quality gates - those are enforced by linux-gate.

#### windows (required - build + tests)

Runner: `windows-latest`

| Step         | Command              |
|--------------|----------------------|
| Install make | `choco install make` |
| Run tests    | `make test`          |

Builds and tests with MSVC. No quality gates.

#### xcode (advisory)

Runner: `macos-latest`

| Step             | Command            |
|------------------|--------------------|
| Generate project | `make xcode-gen`   |
| Build            | `make xcode-build` |
| Test             | `make xcode-test`  |

Verifies the Xcode generator path separately from the Unix Makefiles build.

### Caching

The linux-gate job caches Docker BuildKit layers via GitHub Actions cache, avoiding a full image
rebuild on every run. The Dockerfile is structured to maximize layer reuse - tool installation
layers change rarely, source code layers change frequently.

Build-and-test jobs (linux-core, macos, windows) cache their `build/` directory:

```
${{ runner.os }}-build-${{ hashFiles('CMakeLists.txt', 'src/**') }}
```

### Branch Protection

The `master` branch requires **linux-gate**, **linux-core**, **macos**, and **windows** to pass
before merge. The **xcode** job is advisory. Merge strategy: regular merge commits (not squash).

* * *

## Quality Gates for Coding Agents

When modifying any file under `src/`, an agent must run:

```
make fmt && make check && make test
```

If the change touches `src/private/mx/core/`, run `make test-all` instead of `make test`.

`make check` enforces:

1. **Formatting** - all files must be formatted per `.clang-format`. `make fmt` fixes formatting.
2. **Compiler warnings** - the build must configure, compile, and emit no `warning:` lines.

These commands require Docker. If Docker is not available, `make check` will report the error. No
other tool installation is needed for quality gates.

* * *

## Future Work: scoped clang-tidy

clang-tidy is not currently a quality gate. It was evaluated and removed because running it across
the whole tree is not viable:

- `src/private/mx/core/` holds ~590 generated element `.cpp` files (1182 total including headers).
  clang-tidy over all of them measured at roughly 8 s/file (~1.3 hours total).
- With a `src/.*` header filter, clang-tidy re-parses shared headers once per translation
  unit and reports millions of duplicated diagnostics.
- The generated `mx/core` code is slated for replacement by a future codegen rewrite, so linting it
  now has little value.

The intended future direction is to reintroduce clang-tidy **scoped to the hand-written public API
and implementation** - starting with `src/include/mx/api/` (`mx/api`) - and explicitly
**excluding generated `mx/core`**. Scoped that way the file count and header-filter scope are small
enough to be a fast, useful gate. This is deferred and is not part of the current pipeline; the
future codegen rewrite is expected to emit `mx/core` that would itself pass such a scoped lint.
