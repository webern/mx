# mx

A library for serializing and deserializing MusicXML in C++. The ./README.md file is useful.

## Project Index

Key paths in this repository:

- `src/` — Main C++ source tree (public API, core elements, implementation, tests)
- `src/include/mx/api/` — Public API headers: `*Data` structs and `DocumentManager` entry
  point
- `src/private/mx/api/` — Public API implementation backing the `include/mx/api` headers
- `src/private/mx/core/` — Strongly-typed MusicXML element classes (mostly code-generated)
- `src/private/mx/core/elements/` — Generated element classes, one pair per MusicXML element
  (1182 files)
- `src/private/mx/impl/` — Conversion layer mapping the core DOM to the public API
- `src/private/mx/ezxml/` — Embedded lightweight XML reader/writer used by the core
- `src/private/mx/utility/` — Shared helpers (string, parsing, file system utilities)
- `src/private/mxtest/` — Test suite (api, core, file, import, impl, control)
- `src/private/cpul/` — Catch-based unit-test harness and test runner main
- `gen/version-b/` — Rust tool that generates the core element classes from the MusicXML XSD
- `gen/version-b/src/` — Generator source: XSD parsing, model, and C++ emission
- `data/` — MusicXML input files and expected-output suites for round-trip tests
- `docs/` — MusicXML XSD specifications and developer notes
- `DevScripts/` — Ruby and shell scripts for one-off code maintenance
- `CMakeLists.txt` — CMake build configuration
- `Makefile` — Primary build-and-test entry point (wraps CMake; `make help` lists targets)
- `README.md` — Project overview, build instructions, and usage examples
- `Xcode/` — Xcode project and workspace files for macOS/iOS builds

## Historical Context

`src/private/mx/core/` and `src/private/mx/core/elements/` was originally
"hand-generated" by human brute-force using Ruby scripts which can still be found in `./DevScripts`.
This was never a one-shot solution to generating the code from the XSD spec. Rather, it was an
iterative process, solving problems encountered one-at-a-time until the XSD spec was entirely
covered. As such, it is not viable for re-use at this time, but can be used to understand the
historical nature of how the types were first generated.

## The Problem

We are stuck somewhere around MusicXML 3.1 (or maybe 3.0) because we cannot reliably re-generate the
types from a newer version of the specification. MusicxML 4.0 has been out for a long time, and we
want to support it. But we need to write new code-gen tooling to reproduce the emission of the core
types and then expose the new features in `src/include/mx/api/`.

## Quality Gates

When modifying any file under `src/`, run:

```
make fmt && make check && make test
```

before considering the change complete. If the changes include anything under
`src/private/mx/core/`, use `make test-all` instead of `make test`.

`make fmt` and `make check` run inside Docker (requires Docker on the host). No other tool
installation is needed. `make check` enforces formatting (clang-format) and zero compiler warnings
using a pinned toolchain. (clang-tidy is deferred future work, scoped to `mx/api` only - see the
design doc.)

On `make check` failure, run `make fmt` to fix formatting, then address any remaining warnings
manually.

See `docs/ai/project/build-and-ci-design.md` for full details on the CI pipeline and quality
gate design.

## The Project

We are working on reverse engineering a new codegen system to regenerate mx/core for MusicXML 4.0.
See the project directory @./docs/ai/project/AGENTS.md
