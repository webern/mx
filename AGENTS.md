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
  data/                 <- MusicXML test corpus (~1,347 files, see data/README.md)
  docs/ai/design/       <- design docs (plates.md: the Plates, the template-facing layer)
  src/private/          <- C++ source
    mx/ezxml/           <- vendored pugixml-backed XML layer
    mx/core/            <- generated C++ typed model
    mx/utility/         <- generated C++ utilities
    mxtest/corert/      <- C++ core roundtrip test harness (Catch2, dynamic registration)
    mxtest/import/      <- normalization helpers (sort attrs, strip decimal zeros)
    mxtest/file/        <- PathRoot.h (CMake-generated, gitignored)
    cpul/               <- vendored Catch2 test runner
  gen/                  <- code generator system (see gen/README.md)
    __main__.py         <- CLI: analyze | ir | plates | <config.toml>
    README.md           <- architecture, IR glossary, XSD analysis
    xsd/                <- XSD parser + structural analysis
    ir/                 <- resolved intermediate representation (IR)
    cpp/config.toml     <- C++ target configuration
    test/go/            <- Go corert test target
      config.toml       <- Go target configuration
      go.mod, go.sum    <- Go module (etree dependency, vendored)
      vendor/           <- vendored Go deps
      corert/           <- test package (discover, fixer, normalize, roundtrip, test)
      mx/               <- the GENERATED Go model (committed; do not edit)
    test/c/             <- C corert test target
      config.toml       <- C target configuration
      CMakeLists.txt    <- CMake project using libxml2
      src/              <- C source (main, discover, fixer, normalize, compare, roundtrip)
      mx/               <- the GENERATED C model (committed; do not edit)
```

## Build system

### Docker (mx-sdk)

All Docker-gated targets auto-build the `mx-sdk` image on first use. The workspace is bind-mounted
at `/workspace`. A named Docker volume `mx-build` persists CMake/ccache state across runs.

The `MX_RUNNING_IN_DOCKER` env var switches the Makefile between in-container (direct tool
invocation) and outside-container (docker run wrapper) behavior.

### Makefile targets

Run `make help` for the full, current target list (native C++, generator, Go/C test targets,
housekeeping). Docker-gated targets auto-build and run via mx-sdk.

## The corert (core roundtrip) test

The corert test is the primary correctness gate. It exercises the generated parser by round-tripping
every eligible XML file in `data/` through the typed model and comparing the output to a normalized
form of the input. The corpus is pinned to version `3.0` throughout (input and expected) even though
the generators target newer schemas (C++ 4.0, Go and C 3.1), so comparison runs against a stable
baseline.

### Flow (same in all three languages)

1. **Discover** eligible `.xml`/`.musicxml` files under `data/`, excluding directories `expected`,
   `testOutput`, `generalxml`, `smufl`, and files matching `*.fixup.xml` or having a `.invalid`
   sibling marker.
2. For each file:
   a. Load the XML into a DOM.
   b. Set the root `version` attribute to `"3.0"`.
   c. **Parse** into the typed model via `fromXDoc` (the generated code).
   d. **Serialize** back to XML via `toXDoc`.
   e. **Normalize** the actual output (see Normalization pipeline below).
   f. Load a fresh expected document from disk, apply the same normalization.
   g. Apply **fixups** from `.fixup.xml` sidecars to the expected document.
   h. **Compare** the two DOMs depth-first: element names, text content, attributes (with numeric
      equivalence for ints/floats).
3. Report pass/fail per file.

### Current state

The Go and C suites are GREEN: 776 files pass, 0 fail, and 52 skip (they declare MusicXML 4.0;
those targets generate from the 3.1 schema, and while MusicXML is backward compatible, a newer
document may use types an older model cannot represent -- the harnesses gate on the root's
declared version). The C++ target still has no generated `mx/core`, so corert C++ does not
compile yet; that is the remaining expected gap.

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
4. Strip whitespace-only text nodes from every element (pretty-printing indentation is not
   content; MusicXML has no mixed content, and the rule is applied to both sides, so it stays
   symmetric).
5. Strip trailing zeros from decimal fields (the list lives in `DecimalFields.h`).
6. Sort attributes alphabetically by QUALIFIED name (`xlink:href`, not `href`; must be last).

Comparison details that took debugging to get right (the C++ harness will need the same when its
generated core lands): compare each element's DIRECT text only, never the subtree concatenation
(a numerically-equivalent leaf reformat would otherwise fail at every ancestor); compare
attributes by qualified name with entity-resolved values (a parsed `xlink:href` is (ns, href)
while a serialized one may be the literal name); the Go loader transcodes UTF-16 and ISO-8859-1
to UTF-8 (libxml2 and pugixml auto-detect these; Go's encoding/xml does not). Documents whose
root declares a version newer than the target's generated `SupportedMusicXMLVersion` /
`MX_SUPPORTED_MUSICXML_VERSION` constant are skipped, not failed.

### Numeric equivalence

Text comparisons use numeric equivalence: if both strings parse as integers (or floats), compare
their values instead of their string representations. Float comparison uses epsilon `< 0.00000001`.

## Generator architecture

The generator (`gen/`) is a Python program structured as a pipeline: parse the MusicXML XSD into a
model (`gen/xsd/`), lower that into a resolved intermediate representation (`gen/ir/`), project the
IR onto a target as the Plates (`gen/plates/`), then emit code from the plates via per-language
backends (`gen/emit/`). The IR data model preserves the schema's named structure
(model groups, attribute groups, inheritance edges); `gen/ir/resolve.py` collapses it on demand into
the flattened view an emitter consumes (attribute groups expanded, group refs spliced into content),
so that splicing-and-deduping reasoning lives once rather than once per language. See `gen/README.md`
for the architecture, IR glossary, the resolution layer, and a structural analysis of the schema.

Vocabulary: a **plate** is the per-type metadata object handed to a template -- one per emitted
type, carrying the target's identifier casings, type mappings, emit strategy, and file assignment.
The **Plates** is the full collection projected for one target; it is where config.toml meets the
IR, so templates stay dumb renderers. Specified in `docs/ai/design/plates.md`.

Commands:
- `python3 -m gen analyze [xsd]` - print a structural analysis of the XSD.
- `python3 -m gen ir [--type NAME] [--resolve] [--config C] [xsd]` - lower the XSD to the IR and
  print it as JSON; `--resolve` prints the collapsed (group-spliced, attribute-flattened) view of
  complex types; `--config` applies a target config's companion patches (the sounds.xml fold) first.
- `python3 -m gen plates --config C [--type NAME] [--check]` - project the IR onto the target the
  config describes and print the Plates as JSON; `--check` validates renames and detects identifier
  collisions (a CI gate, like analyze).
- `python3 -m gen <config.toml>` - emit code for the target in the config (not yet implemented).

Each target has a `config.toml` specifying the MusicXML XSD it generates from (`[input] xsd`), the
output directory (`[output] dir`, relative to the config file), an optional `[sounds] xml` companion
file (see below), and, eventually, language-specific settings. Each path is relative to the config
file. The three targets deliberately span the matrix: C++ is 4.0 with sounds, C is 3.1 with sounds,
and Go is 3.1 without sounds (the C/Go pair differ only by the companion fold).

### Companion data

`instrument-sound` is `xs:string` in the XSD; the standard sound identifiers live only in the
separately versioned `sounds.xml` (vendored as `docs/sounds-<version>.xml`). When a target's
`config.toml` sets `[sounds] xml`, `gen/ir/sounds.py` folds them into the IR as a `sound-id` enum
unioned with an open string (element `instrument-sound` retyped from `string` to that union). This is
the only place the IR depends on an input beyond the XSD; it is opt-in per target, so the base IR
stays a pure function of the schema.

**Status.** The parse, IR, analysis, Plates, and emit stages exist (`python3 -m gen plates
--config C [--check]` dumps or gates the projection; `python3 -m gen <config.toml>` emits). The
Go and C backends are complete -- all value and complex shapes plus the document entry points --
and both corert suites are green. Generated output is committed (`gen/test/go/mx/`,
`gen/test/c/mx/`). The C++ backend is not yet implemented.

## Language targets

### C++ (primary, `gen/cpp/`)

MusicXML 4.0 with the sounds companion. The existing codebase. Generated code lands in
`src/private/mx/core/`. The ezxml layer (`src/private/mx/ezxml/`) provides the XML DOM that the
generated code builds on.

### Go (test target, `gen/test/go/`)

MusicXML 3.1 *without* the sounds companion. Uses `github.com/beevik/etree` (vendored) for DOM-style
XML. The generated code lives in `gen/test/go/mx/` (committed). Test runner uses Go's `testing`
package with subtests; documents declaring a newer MusicXML than 3.1 are skipped.

### C (test target, `gen/test/c/`)

MusicXML 3.1 *with* the sounds companion -- same schema as Go, so the two outputs differ only by the
fold. Uses libxml2 (apt package in Docker). The generated code lives in `gen/test/c/mx/`
(committed), built as the `mx-c` static library plus a `values-smoke` test binary. Test runner is a
simple `main()` that prints pass/fail/skip per file and a summary.

## Key files to understand

- `src/private/mxtest/corert/CoreRoundtripImpl.cpp` - the C++ roundtrip implementation (reference
  for Go and C ports)
- `src/private/mxtest/corert/Fixer.cpp` - C++ fixup logic (reference for ports)
- `src/private/mxtest/import/DecimalFields.h` - list of decimal fields and zero-stripping logic
- `gen/test/go/corert/roundtrip.go` - Go roundtrip implementation
- `gen/test/c/src/roundtrip.c` - C roundtrip implementation
