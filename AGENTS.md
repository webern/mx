# mx

@./README.md

## Project Index

Keep this section as a Markdown table. When updating entries, maintain the table format.

| Path                            | Description                                                                 |
|---------------------------------|-----------------------------------------------------------------------------|
| `src/`                          | Main C++ source tree (public API, core elements, implementation, tests)     |
| `src/include/mx/api/`           | Public API headers: `*Data` structs and `DocumentManager` entry point       |
| `src/private/mx/api/`           | Implementation backing the public API headers                               |
| `src/private/mx/core/`          | Strongly-typed MusicXML element classes (mostly code-generated)             |
| `src/private/mx/core/elements/` | Generated element classes, one pair per MusicXML element (1182 files)       |
| `src/private/mx/impl/`          | Conversion layer mapping the core DOM to the public API                     |
| `src/private/mx/ezxml/`         | Embedded lightweight XML reader/writer used by the core                     |
| `src/private/mx/utility/`       | Shared helpers (string, parsing, file system utilities)                     |
| `src/private/mxtest/`           | Test suite (api, core, file, import, impl, control)                         |
| `src/private/cpul/`             | Catch-based unit-test harness and test runner main                          |
| `gen/version-a/`                | Historical Ruby/shell scripts from the original brute-force code gen        |
| `gen/version-b/`                | Active Rust code generator for reproducing MusicXML 4.0 element classes     |
| `data/`                         | MusicXML test files and expected-output suites for round-trip tests         |
| `docs/musicxml.xsd`             | MusicXML specification (reference)                                          |
| `docs/ai/project/`              | AI-assisted project planning and codegen design documents                   |
| `Makefile`                      | Primary build-and-test entry point (wraps CMake; `make help` lists targets) |
| `CMakeLists.txt`                | CMake build configuration                                                   |

## Code Generation - Historical Context

`src/private/mx/core/` and `src/private/mx/core/elements/` was originally "hand-generated" by human
brute-force using Ruby scripts which can still be found in `./gen/version-a`. This was never a
one-shot solution to generating the code from the XSD spec. Rather, it was an iterative process,
solving problems encountered one-at-a-time until the XSD spec was entirely covered. As such, it is
not viable for re-use at this time, but can be used to understand the historical nature of how the
types were first generated.

At some point, I tried to create a Rust based codegenerator in order to be able to regenerate
`mx/core`. However, this devolved and failed. It is kept in `gen/version-b` for historical
curiosity, but it never worked.

### The Problem

We are stuck somewhere around MusicXML 3.1 (or maybe 3.0) because we cannot reliably re-generate the
types from a newer version of the specification. MusicxML 4.0 has been out for a long time, and we
want to support it. But we need to write new code-gen tooling to reproduce the emission of the core
types and then expose the new features in `src/include/mx/api/`.

Furthermore, many decisions in the original gen process were taken by hand. Using a hand-rolled
type, for example, instead of what would have strictly implemented the spec. There are human choices
throughout `mx/core` that will need to be preserved with future code generating efforts.

Code generation was never, and should not in the future, be designed to generate any valid XSD
specification. Rather, the goal of code generation is bespoke, to produce what is needed for the
`mx` library from the MusicXML specification.

## Quality Gates

When modifying any file under `src/`, run:

```
make fmt && make check && make test
```

If the changes include anything under `src/private/mx/core/`, use `make test-all` instead of
`make test`.

`make fmt` and `make check` run inside Docker (requires Docker on the host).

On `make check` failure, run `make fmt` to fix formatting, then address any remaining warnings
manually.

See `docs/ai/project/build-and-ci-design.md` for full details on the CI pipeline and quality gate
design.

## The Project

We are working on reverse engineering a new codegen system to regenerate mx/core for MusicXML 4.0.
See the project directory @./docs/ai/project/AGENTS.md
