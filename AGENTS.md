# mx

A code generator that reads the MusicXML 4.0 XSD specification and emits typed document
serialization/deserialization libraries in multiple languages. C++ is the primary target; Go and C
are secondary targets that keep the generator architecture honest about extensibility.

Ignore git history prior to `b01288`. Reading anything before that commit will only confuse you and
degrade your performance.

## Repository layout

```
mx/
  AGENTS.md             <- you are here
  Makefile              <- top-level build driver (native + Docker-gated targets)
  Dockerfile            <- mx-sdk toolchain image (Ubuntu 24.04, GCC 14, Go, libxml2, Python 3)
  CMakeLists.txt        <- C++ project: ezxml library + corert test harness
  data/                 <- MusicXML test corpus (~830 files, see data/README.md)
  src/private/          <- C++ source
    mx/ezxml/           <- vendored pugixml-backed XML layer (builds today)
    mx/core/            <- generated C++ typed model (empty until gen emits code)
    mx/utility/         <- generated C++ utilities (empty)
    mxtest/corert/      <- C++ core roundtrip test harness (Catch2, dynamic registration)
    mxtest/import/      <- normalization helpers (sort attrs, strip decimal zeros)
    mxtest/file/        <- PathRoot.h (CMake-generated, gitignored)
    cpul/               <- vendored Catch2 test runner
  gen/                  <- code generator system
    __main__.py         <- generator entry point (stub, not yet implemented)
    cpp/config.toml     <- C++ target configuration
    test/go/            <- Go corert test target
      config.toml       <- Go target configuration
      go.mod, go.sum    <- Go module (etree dependency, vendored)
      vendor/           <- vendored Go deps
      corert/           <- test package (discover, fixer, normalize, roundtrip, test)
      stub/             <- placeholder parser stubs (always return error)
    test/c/             <- C corert test target
      config.toml       <- C target configuration
      CMakeLists.txt    <- CMake project using libxml2
      src/              <- C source (main, discover, fixer, normalize, compare, roundtrip, stub)
```

## Build system

### Docker (mx-sdk)

All Docker-gated targets auto-build the `mx-sdk` image on first use. The workspace is bind-mounted
at `/workspace`. A named Docker volume `mx-build` persists CMake/ccache state across runs.

The `MX_RUNNING_IN_DOCKER` env var switches the Makefile between in-container (direct tool
invocation) and outside-container (docker run wrapper) behavior.

### Makefile targets

**Native C++ (no Docker required):**
- `make ezxml` - build the ezxml XML layer
- `make core-dev` - build corert binary (fails until gen emits `mx/core`)
- `make test-core-dev` - run C++ corert suite

**Generator (via mx-sdk):**
- `make gen` - run generator for all targets (cpp/go/c)
- `make gen-cpp`, `make gen-go`, `make gen-c` - single-target gen

**Go test target (via mx-sdk):**
- `make build-go` - compile Go corert test binary
- `make test-go` - run Go corert tests

**C test target (via mx-sdk):**
- `make build-c` - compile C corert test binary
- `make test-c` - build + run C corert tests

**Housekeeping:**
- `make fmt` / `make check` - C++ formatting (via mx-sdk)
- `make sdk` - force-build the Docker image
- `make clean` - remove all build artifacts
- `make clean-docker` - remove Docker image and volume

## The corert (core roundtrip) test

The corert test is the primary correctness gate. It exercises the generated parser by round-tripping
every eligible XML file in `data/` through the typed model and comparing the output to a normalized
form of the input.

### Flow (same in all three languages)

1. **Discover** eligible `.xml`/`.musicxml` files under `data/`, excluding directories `expected`,
   `testOutput`, `generalxml`, `smufl`, and files matching `*.fixup.xml` or having a `.invalid`
   sibling marker.
2. For each file:
   a. Load the XML into a DOM.
   b. Set the root `version` attribute to `"3.0"`.
   c. **Parse** into the typed model via `fromXDoc` (this is the generated code -- currently a stub
      that always fails).
   d. **Serialize** back to XML via `toXDoc`.
   e. **Normalize** the actual output: set XML declaration, set DOCTYPE, set version, strip trailing
      zeros from decimal fields, sort attributes alphabetically.
   f. Load a fresh expected document from disk, apply the same normalization.
   g. Apply **fixups** from `.fixup.xml` sidecars to the expected document.
   h. **Compare** the two DOMs depth-first: element names, text content, attributes (with numeric
      equivalence for ints/floats).
3. Report pass/fail per file.

### Current state

All three test targets (C++, Go, C) discover ~829 files and produce 100% failures because the
generated parser stubs always return "not implemented". This is the expected state until the
generator emits code.

### Data directory conventions

- `data/README.md` - documents marker file conventions.
- `*.xml.invalid` - sibling marker meaning the file is not valid MusicXML; skip it.
- `*.fixup.xml` - sidecar describing value substitutions for the expected document. Used when mx
  clamps out-of-bounds values on import (e.g. MIDI channel 0 -> 1). Format:
  ```xml
  <fixups>
    <replace>
      <type>element</type>          <!-- "element" or "attribute" -->
      <name>midi-channel</name>
      <value>0</value>
      <replacement-value>1</replacement-value>
    </replace>
  </fixups>
  ```
- `data/testOutput/corert/` - debug output directory for failure diffs (gitignored via build/).

### Normalization pipeline

Applied to both expected and actual documents before comparison:

1. Set XML declaration: `<?xml version="1.0" encoding="UTF-8" standalone="no"?>`.
2. Set DOCTYPE based on root element name (`score-timewise` vs `score-partwise`).
3. Set root `version` attribute to `"3.0"`.
4. Strip trailing zeros from decimal fields: `top-system-distance`, `dynamics`, `left-margin`,
   `right-margin`, `staff-distance`, `system-distance`, `default-y`, `default-x`, `tenths`, `width`.
5. Sort attributes alphabetically (must be last).

### Numeric equivalence

Text comparisons use numeric equivalence: if both strings parse as integers (or floats), compare
their values instead of their string representations. Float comparison uses epsilon `< 0.00000001`.

## Generator architecture

The generator (`gen/`) is a Python program invoked as `python3 -m gen <config.toml>`. It reads the
MusicXML XSD spec and emits code for a specific language target based on the config file.

Each target has:
- `config.toml` - specifies the output directory (relative to the config file) and will eventually
  hold language-specific settings.
- Template files (not yet created) - Jinja2 or similar templates for code generation.

**Not yet implemented.** The `gen/__main__.py` stub exits with "not implemented".

## Language targets

### C++ (primary, `gen/cpp/`)

The existing codebase. Generated code lands in `src/private/mx/core/`. The ezxml layer
(`src/private/mx/ezxml/`) provides the XML DOM that the generated code builds on.

### Go (test target, `gen/test/go/`)

Uses `github.com/beevik/etree` (vendored) for DOM-style XML. The generated code will land in
`gen/test/go/mx/`. Test runner uses Go's `testing` package with subtests.

### C (test target, `gen/test/c/`)

Uses libxml2 (apt package in Docker). The generated code will land in `gen/test/c/mx/`. Test runner
is a simple `main()` that prints pass/fail per file and a summary.

## Key files to understand

- `src/private/mxtest/corert/CoreRoundtripImpl.cpp` - the C++ roundtrip implementation (reference
  for Go and C ports)
- `src/private/mxtest/corert/Fixer.cpp` - C++ fixup logic (reference for ports)
- `src/private/mxtest/import/DecimalFields.h` - list of decimal fields and zero-stripping logic
- `gen/test/go/corert/roundtrip.go` - Go roundtrip implementation
- `gen/test/c/src/roundtrip.c` - C roundtrip implementation
