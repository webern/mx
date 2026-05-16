# mx

A library for serializing and deserializing MusicXML in C++. The ./README.md file is useful.

## Project Index

Key paths in this repository:

- `Sourcecode/` — Main C++ source tree (public API, core elements, implementation, tests)
- `Sourcecode/include/mx/api/` — Public API headers: `*Data` structs and `DocumentManager` entry point
- `Sourcecode/private/mx/api/` — Public API implementation backing the `include/mx/api` headers
- `Sourcecode/private/mx/core/` — Strongly-typed MusicXML element classes (mostly code-generated)
- `Sourcecode/private/mx/core/elements/` — Generated element classes, one pair per MusicXML element (1182 files)
- `Sourcecode/private/mx/impl/` — Conversion layer mapping the core DOM to the public API
- `Sourcecode/private/mx/ezxml/` — Embedded lightweight XML reader/writer used by the core
- `Sourcecode/private/mx/utility/` — Shared helpers (string, parsing, file system utilities)
- `Sourcecode/private/mxtest/` — Test suite (api, core, file, import, impl, control)
- `Sourcecode/private/cpul/` — Catch-based unit-test harness and test runner main
- `CodeGen/` — Rust tool that generates the core element classes from the MusicXML XSD
- `CodeGen/src/` — Generator source: XSD parsing, model, and C++ emission
- `Resources/` — MusicXML input files and expected-output suites for round-trip tests
- `Documents/` — MusicXML XSD specifications and developer notes
- `DevScripts/` — Ruby and shell scripts for one-off code maintenance
- `CMakeLists.txt` — CMake build configuration
- `Makefile` — Primary build-and-test entry point (wraps CMake; `make help` lists targets)
- `README.md` — Project overview, build instructions, and usage examples
- `Xcode/` — Xcode project and workspace files for macOS/iOS builds

## Historical Context

`Sourcecode/private/mx/core/` and  `Sourcecode/private/mx/core/elements/` was originally "hand-generated" by human
brute-force using Ruby scripts which can still be found in `./DevScripts`. This was never a one-shot solution to
generating the code from the XSD spec. Rather, it was an iterative process, solving problems encountered one-at-a-time
until the XSD spec was entirely covered. As such, it is not viable for re-use at this time, but can be used to
understand the historical nature of how the types were first generated.

## The Problem

We are stuck somewhere around MusicXML 3.1 (or maybe 3.0) because we cannot reliably re-generate the types from a newer
version of the specification. MusicxML 4.0 has been out for a long time, and we want to support it. But we need to write
new code-gen tooling to reproduce the emission of the core types and then expose the new features in
`Sourcecode/include/mx/api/`.

## The Project

We are working on reverse engineering a new codegen system to regenerate mx/core for MusicXML 4.0. See the project
directory @./Documents/ai/project/AGENTS.md
