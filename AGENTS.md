# mx

@./README.md

`mx` is a MusicXML C++ library. The product surface is `mx::api`, a simplified and narrowed
interface of MusicXML backed by the strongly-typed `mx::core` model.

Quickstart: `make test-core-dev` (fast corert gate) - `make fmt` (format) - `make help` (all
targets). Everything runs in the `mx-sdk` Docker image unless `MX_RUNNING_IN_DOCKER=1`.

## Repository layout

```
mx/
  AGENTS.md             <- you are here
  .claude/skills/       <- agent skills; every skill name starts with mx-
  Makefile              <- top-level build driver
  Dockerfile            <- mx-sdk image with toolchains and dev tools
  CMakeLists.txt        <- C++ project
  Package.swift         <- Swift Package Manager wrapper (built by the swift-local CI job)
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
    cpp/                <- C++ target (the product): config.toml + templates
    schema/             <- JSON Schema target
    test/go/            <- A toy Go implementation of MusicXML XSD for gen validation
    test/c/             <- A toy C implementation of MusicXML XSD for gen validation
  audit/                <- MusicXML feature-audit tool (see audit/README.md); `make audit`
```

## Feature audit (`audit/`)

`make audit` inventories corpus feature usage (`*.features.xml` sidecars + `data/corpus.xml`,
checked in; the test suites skip them). See `audit/README.md`; the `mx-api-feature-audit` skill
consumes the output.

## Build system

The `Makefile` is the entrypoint for all build processes; it drives `cmake` and runs everything
inside the `mx-sdk` Docker image unless `MX_RUNNING_IN_DOCKER=1` (see the README's Build section).
Lean on it and suggest improvements when it doesn't have what you need. Run `make help` for the
target list. Extend the `mx-sdk` image when new tools are needed.

## The corert (core roundtrip) test

Runs with `make test-core-dev`. Deserializes and reserializes the test corpus with `mx::core`. It
compiles without `mx::impl`/`mx::api`, so you can iterate on `gen/cpp` and the generated core and
defer fixing the `mx::impl` integration until you are ready.

### Flow (same in all three languages)

1. **Discover** eligible `.xml`/`.musicxml` files under `data/` (excluding certain directories and
   marker files; see `data/README.md`). Files with a sibling `.invalid` marker are skipped as
   unparseable.
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

Do not add new anonymous namespaces (`namespace { }`); many existing files still have them and
they are being retired as touched. Anonymous namespaces give internal linkage, which is correct for a normal
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

## mx::api conventions (presence/absence)

`mx::api` is the public product surface: plain-old-data structs that make MusicXML easier to use
than the strongly-typed `mx::core` model. No core type may appear in a public api header.

- New fields that can be absent: use `std::optional<T>` (values and sub-structs alike).
  Precedent: `NoteData::tieLetRing`, `PartData::transposition`, `MeasureData::partSymbol`.
- Exception, enums: an absent-able enum gets an `unspecified` enumerator as its first value and
  default — never `std::optional<SomeEnum>`. This includes the ternary
  `Bool { unspecified, yes, no }` in `ApiCommon.h`; use it, never `std::optional<bool>`.
- Do not mass-migrate existing `bool is...Specified` / `has...` flags or `-1` sentinels; they are
  legacy (issue #249). Migrations are separate, deliberate breaking changes.
- Every new field needs a default value and a matching `MXAPI_EQUALS_MEMBER(field)` line
  (`MXAPI_DOUBLES_EQUALS_MEMBER` for doubles) in the type's equality block — a missed line
  silently drops the field from equality and round-trip checks.

### Design doctrine

Before designing or changing anything in `mx::api`, read the `mx-api-doctrine` skill
(`.claude/skills/mx-api-doctrine/SKILL.md`): the seven design principles, the failure model
(no UB, no exceptions, `Result` quarantined), choice types, and comment rules.

## Quality gates

Run `make fmt` to format. `make check` is the clang-format gate **only** — it builds and tests nothing.
Run `make test-core-dev` for corert (especially `mx/core` work) and `make check-core-dev` (fmt-check
+ warning-free build) before pushing core work. `make test` runs the api/impl suite + examples;
corert, `make test-api-roundtrip`, and the gen/audit gates run separately (see `make help` / CI).
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
