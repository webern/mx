# Build and CI Design

## Overview

This document describes the formatting, linting, compiler-warning enforcement, and CI pipeline for
mx. It also defines the quality gates that coding agents must satisfy when modifying `Sourcecode/`.

The full implementation is delivered as the commit series at the end of this document. Each commit
leaves the repository in a buildable, coherent state.

### Note on `mx/core/`

The files in `Sourcecode/private/mx/core/` were originally machine-generated from the MusicXML XSD.
The codegen program no longer exists. These files are now treated as normal hand-edited source and
must pass all quality gates like any other code. A future codegen rewrite will re-own these files
and will be written to emit code that passes the linter from the start.

* * *

## Toolchain

### Formatting: clang-format

Style base: **Microsoft**. This is the closest standard clang-format style to the existing codebase:
4-space indent, Allman braces (opening `{` on its own line for namespaces, classes, and functions).
The one visible change from the historical style is the removal of spaces inside parentheses
(`func( arg )` becomes `func(arg)`).

A `.clang-format` file at the repo root encodes this style. All C++ files under `Sourcecode/` are
formatted, including generated files in `mx/core/`.

**Installation**

| Platform | Command                             |
|----------|-------------------------------------|
| macOS    | `brew install clang-format`         |
| Linux    | `sudo apt-get install clang-format` |
| Windows  | Install LLVM from llvm.org          |

### Linting: clang-tidy

clang-tidy reads `compile_commands.json` produced by CMake (`-DCMAKE_EXPORT_COMPILE_COMMANDS=ON`).
This gives it the correct include paths, preprocessor defines, and C++ standard — catching real
issues rather than false positives from missing context.

The `.clang-tidy` file at the repo root enables a conservative starter set of checks:

| Check group              | Purpose                                                         |
|--------------------------|-----------------------------------------------------------------|
| `bugprone-*`             | Flags likely bugs (suspicious constructs, error-prone patterns) |
| `modernize-use-nullptr`  | Replace `NULL`/`0` with `nullptr`                               |
| `modernize-use-override` | Add `override` to overriding virtual functions                  |
| `performance-*`          | Common performance anti-patterns                                |

The check set is intentionally narrow. It can be expanded as the codebase improves.

**Installation**

| Platform | Command                                   |
|----------|-------------------------------------------|
| macOS    | `brew install llvm` (includes clang-tidy) |
| Linux    | `sudo apt-get install clang-tidy`         |
| Windows  | Install LLVM from llvm.org                |

### Compiler Warnings

`CMakeLists.txt` adds `-Wall -Wextra` (GCC/Clang) or `/W4` (MSVC) to the `mx` target. `-Werror` is
intentionally omitted so that warnings can be addressed in a dedicated commit (4b) rather than
breaking the build immediately upon enabling them. Instead, `make check` treats any `warning:` line
in the build output as a failure.

### CMake Version

`CMakeLists.txt` requires CMake >= 3.13. The Makefile uses `-S`/`-B` and `--build --parallel`, both
of which require 3.13+. The `cmake_minimum_required` in `CMakeLists.txt` is bumped to match in
commit 4.

* * *

## Makefile Targets

### check-tools

A prerequisite target that verifies required tools are on `PATH`. It is a dependency of `fmt`,
`lint`, and `check`. If a tool is missing it prints a platform-appropriate install recommendation
and exits non-zero.

```makefile
check-tools:
    @command -v clang-format >/dev/null 2>&1 || \
        { echo "clang-format not found."; \
          echo "  macOS: brew install clang-format"; \
          echo "  Linux: sudo apt-get install clang-format"; \
          exit 1; }
    @command -v clang-tidy >/dev/null 2>&1 || \
        { echo "clang-tidy not found."; \
          echo "  macOS: brew install llvm"; \
          echo "  Linux: sudo apt-get install clang-tidy"; \
          exit 1; }
```

### make fmt

Formats all C++ source files under `Sourcecode/` in-place using clang-format.

```
make fmt
```

Depends on `check-tools`. This target modifies files; running it twice is a no-op.

### make lint

Runs clang-tidy against all C++ source files under `Sourcecode/`. Depends on `make dev` to ensure
`compile_commands.json` is current before linting.

```
make lint
```

Depends on `check-tools` and `dev`.

### make check

The full quality gate. Runs each sub-check in order and fails on the first violation:

1. **fmt** - runs `clang-format --dry-run --Werror` on all files under `Sourcecode/`. Any
   unformatted file is a failure.
2. **build** - runs `make dev`, capturing output. Any `warning:` line is a failure.
3. **lint** - runs clang-tidy using the `compile_commands.json` produced by step 2. Any warning is a
   failure.

```
make check
```

CI runs `make check` on Linux and macOS. On Windows, `make check` runs steps 1 and 2 (fmt-check and
warning-grep) but skips clang-tidy because `compile_commands.json` is not produced by the Visual
Studio generator. MSVC's `/W4` provides adequate static analysis coverage on Windows.

Developers can run `make check` locally to confirm their changes will pass before pushing.

### Xcode Targets

The Xcode project is generated by CMake and is not checked into the repository. `build/xcode/` is
added to `.gitignore`. Three Makefile targets cover the Xcode workflow, allowing developers to
replicate the CI Xcode job locally:

| Target             | What it does                                  |
|--------------------|-----------------------------------------------|
| `make xcode-gen`   | Runs `cmake -G Xcode -S . -B build/xcode`     |
| `make xcode-build` | Builds the generated project via `xcodebuild` |
| `make xcode-test`  | Runs tests via `xcodebuild test`              |

* * *

## CI Design

### File

`.github/workflows/ci.yaml` - the primary workflow. The old `ccpp.yml` is renamed to
`ccpp.yml.archived` to preserve history without running it.

### Triggers

```yaml
on:
  pull_request:
  push:
    branches: [master]
```

CI runs on every PR update and on every push to `master`. This ensures `master` is always verified
green, not just PRs.

### Jobs

All jobs invoke Makefile targets. This is deliberate: a developer can reproduce any CI job locally
by running the same `make` command.

#### linux (required - quality gate + core tests)

Runner: `ubuntu-latest`

| Step            | Command                                   |
|-----------------|-------------------------------------------|
| Install tools   | `apt-get install clang-format clang-tidy` |
| Quality gate    | `make check`                              |
| Full test suite | `make test-core`                          |

This is the deepest job. It enforces all quality gates (fmt, lint, compiler warnings) and runs the
complete test suite including the slow `mx::core` tests.

#### macos (required - quality gate + tests)

Runner: `macos-latest`

| Step            | Command                          |
|-----------------|----------------------------------|
| Install tools   | `brew install clang-format llvm` |
| Quality gate    | `make check`                     |
| Run tests       | `make test`                      |
| Run examples    | `make examples-run`              |

Runs `make check` (fmt, build with warning-grep, lint) followed by the test suite and examples.
Verifies macOS-specific build and runtime correctness.

#### windows (advisory)

Runner: `windows-latest`

| Step            | Command                                        |
|-----------------|------------------------------------------------|
| Install make    | `choco install make`                           |
| Install tools   | Install LLVM (for clang-format)                |
| Quality gate    | `make check` (fmt-check + warning-grep, no lint) |
| Run tests       | `make test`                                    |

Uses GNU make installed via Chocolatey. clang-tidy is skipped because the Visual Studio generator
does not produce `compile_commands.json`. MSVC `/W4` warnings are caught by the warning-grep step.

#### xcode (advisory)

Runner: `macos-latest`

| Step             | Command            |
|------------------|--------------------|
| Generate project | `make xcode-gen`   |
| Build            | `make xcode-build` |
| Test             | `make xcode-test`  |

Verifies the Xcode generator path separately from the Unix Makefiles build.

### Caching

All jobs cache their `build/` directory to avoid full rebuilds on every run. The cache key is:

```
${{ runner.os }}-build-${{ hashFiles('CMakeLists.txt', 'Sourcecode/**') }}-${{ hashFiles('.github/cache-bust') }}
```

If `CMakeLists.txt` or any source file changes, the cache is invalidated. A `.github/cache-bust`
file allows manual invalidation when needed.

### Branch Protection

The `master` branch requires the **linux** and **macos** jobs to pass before merge. The **windows**
and **xcode** jobs are advisory - failures are visible but do not block merge. Merge strategy:
regular merge commits (not squash).

* * *

## Quality Gates for Coding Agents

When modifying any file under `Sourcecode/`, an agent must run:

```
make check
```

before considering the change complete. `make check` enforces:

1. **Formatting** - all files must be formatted per `.clang-format`. Run `make fmt` to fix
   formatting, then re-run `make check`.
2. **Linting** - all clang-tidy checks in `.clang-tidy` must pass with zero warnings.
3. **Compiler warnings** - the build must emit no `warning:` lines.

If `check-tools` reports a missing tool, install it before running `make check`. The CI Linux and
macOS jobs are both authoritative gates. If the two platforms disagree on a warning (e.g. GCC emits
one that Clang does not, or vice versa), both must be fixed.

* * *

## Commit Series

The following commits implement this design in order. Each commit leaves the repository in a
buildable, coherent state.

| #  | Commit message                                    | What it does                                                                                                                                                                                         |
|----|---------------------------------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| 1  | `add clang-format config and make fmt target`     | Adds `.clang-format` (Microsoft style), `check-tools` prerequisite, and `make fmt` target to the Makefile. No files are reformatted yet.                                                             |
| 2  | `reformat Sourcecode/ with clang-format`          | Applies `make fmt` to all of `Sourcecode/`. Pure formatting diff — no logic changes.                                                                                                                 |
| 3  | `add clang-tidy config and make lint target`      | Adds `.clang-tidy`, enables `CMAKE_EXPORT_COMPILE_COMMANDS=ON` in CMake, adds `make lint` target.                                                                                                    |
| 4  | `add compiler warnings and make check target`     | Adds `-Wall -Wextra` / `/W4` to `CMakeLists.txt`, bumps `cmake_minimum_required` to 3.13, adds `make check` target combining fmt-check, build-warning grep, and lint. On Windows, `make check` skips lint. |
| 4b | `fix compiler and clang-tidy warnings`            | Fixes all warnings surfaced by commit 4 across `Sourcecode/`, including the historically-generated files in `mx/core/` (which are normal source today - see Overview).                                |
| 5  | `modernize CI: new ci.yaml, archive old workflow` | Renames `ccpp.yml` to `ccpp.yml.archived`. Writes `ci.yaml` with linux/macos as required gates, windows/xcode as advisory. Adds Xcode targets, build caching, and `choco install make` for Windows.  |
| 6  | `add quality gates to AGENTS.md`                  | Documents `make check` as the required pre-commit gate for agents modifying `Sourcecode/`.                                                                                                           |
