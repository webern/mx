# Build and CI Design

## Overview

This document describes the formatting, linting, compiler-warning enforcement, and CI pipeline for
mx. It also defines the quality gates that coding agents must satisfy when modifying `Sourcecode/`.

### Note on `mx/core/`

The files in `Sourcecode/private/mx/core/` were originally machine-generated from the MusicXML XSD.
The codegen program no longer exists. These files are now treated as normal hand-edited source and
must pass all quality gates like any other code. A future codegen rewrite will re-own these files
and will be written to emit code that passes the linter from the start.

### Design Principle: One Authoritative Toolchain

Running clang-tidy with one compiler's frontend against another compiler's standard library headers
is fragile. clang-tidy IS a clang compiler frontend - it builds a full AST, performs template
instantiation, and runs overload resolution. When it parses code against GCC's libstdc++, the two
compilers can disagree on edge cases in template internals (e.g. whether `std::sort` requires copy
assignment vs. move assignment in a particular code path). These disagreements produce spurious
errors that don't reflect real code bugs.

Similarly, compiler warning sets and clang-format behavior can change between versions. CI runners
(`ubuntu-latest`, `macos-latest`) float their toolchain versions, so a CI break can occur with no
code change.

To eliminate these fragility surfaces, all quality-gate tooling (formatting, linting, compiler
warnings) runs inside a Docker container with pinned tool versions. This gives deterministic,
reproducible results on any machine - local or CI. Platform-specific CI jobs only build and test.

* * *

## Toolchain (Docker)

All quality-gate tools are pinned inside a Docker image built from the `Dockerfile` at the repo
root. The image is based on Ubuntu 24.04 with clang-18 and uses libc++ (clang's own standard
library) so that clang-tidy's frontend and the standard library headers are from the same
toolchain.

### Formatting: clang-format

Style base: **Microsoft**. This is the closest standard clang-format style to the existing codebase:
4-space indent, Allman braces (opening `{` on its own line for namespaces, classes, and functions).
The one visible change from the historical style is the removal of spaces inside parentheses
(`func( arg )` becomes `func(arg)`).

A `.clang-format` file at the repo root encodes this style. All C++ files under `Sourcecode/` are
formatted, including generated files in `mx/core/`.

### Linting: clang-tidy

clang-tidy reads `compile_commands.json` produced by CMake (`-DCMAKE_EXPORT_COMPILE_COMMANDS=ON`).
This gives it the correct include paths, preprocessor defines, and C++ standard.

The `.clang-tidy` file at the repo root enables a conservative starter set of checks:

| Check group              | Purpose                                                         |
|--------------------------|-----------------------------------------------------------------|
| `bugprone-*`             | Flags likely bugs (suspicious constructs, error-prone patterns) |
| `modernize-use-nullptr`  | Replace `NULL`/`0` with `nullptr`                               |
| `modernize-use-override` | Add `override` to overriding virtual functions                  |
| `performance-*`          | Common performance anti-patterns                                |

The check set is intentionally narrow. It can be expanded as the codebase improves.

### Compiler Warnings

`CMakeLists.txt` adds `-Wall -Wextra` (GCC/Clang) or `/W4` (MSVC) to the `mx` target. Inside the
Docker container, clang-18 is the compiler. `make check` treats any `warning:` line in the build
output as a failure.

### CMake Version

`CMakeLists.txt` requires CMake >= 3.13. The Makefile uses `-S`/`-B` and `--build --parallel`, both
of which require 3.13+.

* * *

## Docker

### Dockerfile

Located at the repo root. Based on Ubuntu 24.04 with pinned clang-18 packages:

- `clang-18`, `libc++-18-dev`, `libc++abi-18-dev` - compiler and standard library
- `clang-format-18` - formatting
- `clang-tidy-18` - linting
- `cmake`, `make` - build tools

The Dockerfile sets `ENV MX_RUNNING_IN_DOCKER=1`. The Makefile checks this variable to decide
whether to run tools directly or to launch Docker.

### BuildKit Cache

The Dockerfile uses `RUN --mount=type=cache,target=/workspace/build` for the CMake build directory.
This persists the incremental build cache across Docker invocations on the same machine, avoiding
full recompiles on every `make check` run.

### Makefile Docker Integration

The Makefile detects `MX_RUNNING_IN_DOCKER`:

- **Inside the container** (`MX_RUNNING_IN_DOCKER=1`): runs clang-format, clang-tidy, and the
  compiler directly.
- **Outside the container**: runs `docker buildx build` to build the image and execute the
  requested target inside it.

For `make fmt`, which needs to write formatted files back to the host, the Makefile uses
`docker buildx build --output type=local,dest=.` to extract the formatted `Sourcecode/` tree.

For `make check` and `make lint`, only the exit code matters - no file extraction needed.

* * *

## Makefile Targets

### Developer Workflow

The standard workflow for any code change under `Sourcecode/`:

```
make fmt && make check && make test
```

If the change touches files under `Sourcecode/private/mx/core/`:

```
make fmt && make check && make test-all
```

These three commands are all a developer or AI agent needs to verify their work. `fmt` and `check`
run in Docker (deterministic, no local tool installation required). `test` and `test-all` run
natively with the local compiler.

### Quality Targets (Docker)

| Target             | What it does                                                      |
|--------------------|-------------------------------------------------------------------|
| `make fmt`         | Formats all C++ files under `Sourcecode/` in-place via Docker     |
| `make check`       | Full quality gate: fmt-check + warning-free build + lint (Docker) |
| `make lint`        | Runs clang-tidy only (Docker)                                     |
| `make clean-docker`| Removes the Docker image and buildx cache                         |

`check-docker` is an internal prerequisite that verifies Docker is available on `PATH`.

### Build Targets (Native)

| Target             | What it does                                                      |
|--------------------|-------------------------------------------------------------------|
| `make lib`         | Build just the static library (no tests, no examples)             |
| `make dev`         | Build tests (no slow core tests) + examples                       |
| `make core`        | Build the full suite including slow `mx::core` tests              |

### Run Targets (Native)

| Target              | What it does                                                     |
|---------------------|------------------------------------------------------------------|
| `make test`         | Build dev, then run mxtest. `ARGS=` forwarded                    |
| `make test-all`     | Build core, then run full mxtest. `ARGS=` forwarded              |
| `make examples-run` | Build dev, then run mxread/mxwrite/mxhide                       |
| `make all`          | Build core, run examples, run full mxtest                        |

### Xcode Targets (Native)

| Target             | What it does                                  |
|--------------------|-----------------------------------------------|
| `make xcode-gen`   | Runs `cmake -G Xcode -S . -B build/xcode`    |
| `make xcode-build` | Builds the generated project via `xcodebuild` |
| `make xcode-test`  | Runs tests via `xcodebuild test`              |

### Housekeeping

| Target             | What it does                         |
|--------------------|--------------------------------------|
| `make clean`       | Remove the entire `build/` tree      |
| `make clean-docker`| Remove Docker image and buildx cache |

### Knobs

| Variable   | Default            | Purpose                                            |
|------------|--------------------|----------------------------------------------------|
| `JOBS`     | auto-detected      | Parallel compile jobs                              |
| `BUILD_TYPE`| `Debug`           | CMake build type                                   |
| `GENERATOR`| platform default   | CMake generator override                           |
| `ARGS`     | (none)             | Forwarded to mxtest (Catch2)                       |

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

| Step            | Command        |
|-----------------|----------------|
| Quality gate    | `make check`   |
| Run tests       | `make test`    |

The Makefile handles Docker internally - CI just runs `make check`. The Docker image is built
from the repo's `Dockerfile` with BuildKit layer caching via GitHub Actions cache.

This is the authoritative quality gate. Formatting, linting, and compiler warnings are enforced
here with pinned tool versions.

#### linux-core (required - full test suite with GCC)

Runner: `ubuntu-latest`

| Step            | Command        |
|-----------------|----------------|
| Full test suite | `make test-all`|

Builds and runs the complete test suite including the slow `mx::core` tests using GCC (the
system compiler). This provides GCC compilation coverage that the Docker gate job (which uses
clang) does not.

#### macos (required - build + tests)

Runner: `macos-latest`

| Step            | Command           |
|-----------------|-------------------|
| Run tests       | `make test`       |
| Run examples    | `make examples-run`|

Builds and tests with the system clang. No quality gates - those are enforced by linux-gate.

#### windows (required - build + tests)

Runner: `windows-latest`

| Step            | Command    |
|-----------------|------------|
| Install make    | `choco install make` |
| Run tests       | `make test`|

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
${{ runner.os }}-build-${{ hashFiles('CMakeLists.txt', 'Sourcecode/**') }}
```

### Branch Protection

The `master` branch requires **linux-gate**, **linux-core**, **macos**, and **windows** to pass
before merge. The **xcode** job is advisory. Merge strategy: regular merge commits (not squash).

* * *

## Quality Gates for Coding Agents

When modifying any file under `Sourcecode/`, an agent must run:

```
make fmt && make check && make test
```

If the change touches `Sourcecode/private/mx/core/`, run `make test-all` instead of `make test`.

`make check` enforces:

1. **Formatting** - all files must be formatted per `.clang-format`. `make fmt` fixes formatting.
2. **Compiler warnings** - the build must emit no `warning:` lines.
3. **Linting** - all clang-tidy checks in `.clang-tidy` must pass with zero warnings.

These commands require Docker. If Docker is not available, `make check` will report the error.
No other tool installation is needed for quality gates.
