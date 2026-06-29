# mx

@./README.md

`mx` is a MusicXML C++ library. The product surface is `mx::api`, a simplified and narrowed
interface of MusicXML backed by the strongly-typed `mx::core` model.

## Repository layout

```
mx/
  AGENTS.md             <- you are here
  Makefile              <- top-level build driver
  Dockerfile            <- mx-sdk image with toolchains and dev tools
  CMakeLists.txt        <- C++ project
  data/                 <- MusicXML test corpus (large, see data/README.md)
  docs/ai/design/       <- design docs
  docs/musicxml-4.0-ed15c23.xsd <- the current musicxml.xsd schema
  src/                  <- C++ sourcecode root
  src/include/mx/api/   <- the public mx::api headers; mx's public interface
  src/private/          <- C++ source
    pugixml/            <- vendored pugixml 1.15 (XML parser and raw DOM)
    mx/core/            <- mx::core's hand-written runtime (outside of mx/core/generated)
    mx/core/generated/  <- MusicXML XSD C++ typed model (generated, see gen/cpp)
    mx/api/             <- mx::api implementation
    mx/impl/            <- the api to core translation layer
    mx/utility/         <- helpers
    mx/examples/        <- api example programs
    mxtest/core/        <- mx::core unit tests (small)
    mxtest/corert/      <- C++ core roundtrip test (deserializes and serializes the test corpus)
    mxtest/import/      <- normalization support (Normalize.cpp/h + DecimalFields.h) shared by corert and api tests
    mxtest/probe/       <- compile-time negative probes: constructs that MUST NOT compile (`make probe-cpp`)
    mxtest/validate/    <- xmllint validation driver: parses+serializes corpus, outputs for `make validate-cpp`
    mxtest/api/         <- tests for the mx::api layer
    mxtest/impl/        <- tests for the mx::impl layer
    mxtest/file/        <- file-walking test infrastructure for the mx::api tests (MxFile, MxFileTest, etc.)
    mxtest/control/     <- compile-control flags (CompileControl.h: enables/disables test suite compilation)
    cpul/               <- vendored Catch2 test runner
  gen/                  <- code generator system (see gen/README.md)
    test/go/            <- A toy Go implementation of MusicXML XSD for gen validation
    test/c/             <- A toy C implementation of MusicXML XSD for gen validation
  audit/                <- MusicXML feature-audit tool (see audit/README.md); `make audit`
```

## Feature audit (`audit/`)

`python3 -m audit` (run via `make audit`) inventories which MusicXML features the corpus uses, so we
can compare against what `mx::api` exposes. It writes a `*.features.xml` sidecar next to each corpus
file and a `data/corpus.xml` aggregate (all checked in; the round-trip suites skip them). The
`api-feature-audit` skill uses these to find enum bugs and feature gaps in `mx::api`. See
`audit/README.md` and `data/README.md`.

## Build system

### Docker (mx-sdk)

Docker is used for tool reproducibility. The `Makefile` has sections that are invoked on the host
and others that are invoked inside the container. The `MX_RUNNING_IN_DOCKER` env var drives this
distinction.

In general, we should strive to make build processes reproducable on developer machines and in CI by
leveraging the `mx-sdk` image and extending it with new tools as they are needed.

### Makefile targets

The `Makefile` serves as the entrypoint of build processes. It calls `cmake` which produces deeper,
generated makefiles in the build directory. You should lean heavily on our top-level `Makefile` and
suggest improvements when it doesn't have what you need.

Run `make help` for the target list.

## The corert (core roundtrip) test

Runs with `make test-core-dev`.

This test suite deserializes and reserializes the test corupus with `mx::core`. It can be compiled
without compiling the `mx::impl` and `mx::api` layes. This provides a mechanism for innovating on
the generated code and templates found in `gen/cpp` without fixing the `mx::impl` layer on every
change to `mx::core`. i.e. you can defer integrating `mx::core` changes with `mx::impl` (and by
extension `mx::api`) until you are ready.

### Flow (same in all three languages)

1. **Discover** eligible `.xml`/`.musicxml` files under `data/`, (excluding certain directories, and
   marker files. See `data/README.md)
   - unparseable files have a sibling file ending with `.invalid` and are skipped.
2. For each file: Load the XML into a DOM, make certain expected alterations, parse it with
   `mx::core`, serialize it back to XML, normalize the output, and compare the two DOMs.
3. Report pass/fail per file.

## Generator (`gen/`)

`gen/` is a Python code generator (`python3 -m gen`) that reads the MusicXML XSD and emits typed
serialization/deserialization libraries. See:
- `gen/AGENTS.md`: your entrypoint
- `gen/DESIGN.md`: how it was designed
- `gen/README.md`: human summary

What you need to know right now is that `gen/cpp` is where our MusicXML types are coming from. Run
`make gen-cpp` to regenerate the C++ types.

## C++ coding rules

Do not use anonymous namespaces (`namespace { }`) anywhere in the codebase. All symbols must be in
a named namespace. Anonymous namespaces give internal linkage, which is correct for a normal
one-TU-per-file build but causes redefinition errors in unity builds (where multiple `.cpp` files
are compiled as a single translation unit). Use a named helper function or per-type name instead:

- For file-local helper functions: name them after the type or file, e.g. `tokenIsNameChar`,
  `clampedTenths`.
- For file-local constants (arrays, string_views): use a per-type prefix, e.g. `kYesNoWire`,
  `kSmuflAccidentalGlyphNamePrefix`.
- In code generator templates (`gen/cpp/templates/`): use `{{ident}}` to make names unique, e.g.
  `k{{ident}}Wire`, `isNameChar{{ident}}`.

Unity builds can be tested with CMake's built-in support — no changes to `CMakeLists.txt` are
needed by contributors. To verify, configure with:

```
cmake -S . -B build/unity -DCMAKE_UNITY_BUILD=ON -DCMAKE_UNITY_BUILD_BATCH_SIZE=0
cmake --build build/unity --target mx
```

`BATCH_SIZE=0` puts all files in a target into one translation unit, which is the strictest test.

## Quality gates

Run `make fmt` to format. `make check` is the clang-format gate **only** — it builds and tests nothing.
Run `make test-core-dev` for corert (especially `mx/core` work); `make test` for everything (slow).
Adding/removing a `data/` file: bump the pinned count in `CoreRoundtripTest.cpp`, run `make audit` (regenerates `corpus.xml` + `*.features.xml`), confirm round-trip via `make test-core-dev`.
`ApiLoadSmokeTest` proves a file imports without crashing, not that the data is correct; the read→write→read gate (`make test-api-roundtrip` / `roundtrip-baseline.txt`) is the correctness check — pin a fixture there to defend a feature.

Look at what will run in CI `.github/workflows/ci.yaml` and anticipate issues there when coding
locally.

## Key files to understand

| File | What it is |
|------|------------|
| `src/include/mx/api/DocumentManager.h` | The public API entry point: createFromFile, createFromScore, getData, writeToFile |
| `src/include/mx/api/ScoreData.h` | The primary api data model (ScoreData, PartData, MeasureData, ...) |
| `src/private/mx/api/DocumentManager.cpp` | API implementation: error channel, parse/serialize orchestration |
| `src/private/mx/impl/ScoreReader.cpp` | Translates mx::core -> mx::api ScoreData |
| `src/private/mx/impl/ScoreWriter.cpp` | Translates mx::api ScoreData -> mx::core |
| `src/private/mx/impl/NoteReader.cpp` | Core -> api note translation (one of the largest impl files) |
| `src/private/mx/impl/NoteWriter.cpp` | Api -> core note translation |
| `src/private/mx/core/generated/Document.h` | The core document model (parse, serialize, Document class) |
| `src/private/mxtest/corert/CoreRoundtripImpl.cpp` | The corert test runner (discover, parse, serialize, compare) |
| `src/private/mxtest/corert/Compare.cpp` | DOM normalization and comparison engine used by corert and api tests |

## Git Authorship

The git commit Author and Committer fields are for the user's information, not yours. No Co-Authored-By either.

Correct:

```text
Author: The User <the.user@gmail.com>
Date:   Sat Jun 20 10:31:06 2026 +0000

    Blah
```

Incorrect:

```text
Author: Claude <noreply@anthropic.com>
Date:   Sat Jun 20 10:31:06 2026 +0000

    Blah

    Co-Authored-By: Claude Opus 4.8 (1M context) <noreply@anthropic.com>
    Claude-Session: https://claude.ai/code/session_01S42a2LXrZb5GUk9cY7Hdru
```
