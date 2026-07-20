# kid-presentable State

## Where we are

Milestone M1 (design recommendation) is drafted and awaiting the user's verdict. Session 1
(2026-07-17) created the project and wrote `design.md`.

## What session 1 did

Researched the api surface (doctrine, DocumentManager, ScoreData, Result, choice types), the
gen/ architecture (DESIGN.md), audit/, and Package.swift. Wrote `design.md` recommending:

- a micro C ABI (~8 functions, frozen) that exchanges whole scores as versioned "Score JSON";
- a house api-model IDL as a second gen/ input, pressed into: Score JSON Schema, the C++ codec,
  a C++ drift gate (headers stay hand-written, model stays authoritative), and idiomatic
  per-language mirrors (TypeScript first, Swift via the existing SPM package next);
- explicit rejections: embind/SWIG direct binding, per-field handle C API,
  protobuf/flatbuffers, WIT as backbone (future-compatible though), per-language regeneration,
  Rust rewrite.

Key insight to preserve: mx::api already has document-granularity value semantics (getData
copies the whole tree), so the serialization boundary loses nothing relative to the C++ api.

## Next session

Blocked on the user's review of `design.md`. Depending on the verdict:

- Accepted → start M2: hand-write `mxc.h` + a vertical-slice Score JSON codec (score header,
  parts, measures, notes with pitch/duration/tick), an emscripten build target in the Makefile
  (the mx-sdk image may need emsdk added to the Dockerfile), and a ten-line Node demo. Keep the
  slice out of src/include/mx/api (it is shim territory, e.g. src/private/mxc or similar);
  nothing in the public C++ api changes.
- Revised → update design.md (current-state only; move superseded reasoning to log.md).

## Gotchas

- Read the mx-api-doctrine skill before touching anything api-shaped; no core types in public
  headers; no exceptions across boundaries — the mxc shim must catch everything like
  DocumentManager does.
- gen/'s cardinal rule applies to any future api-model targets: a new language target must
  touch no generator Python.
- The corpus is the test bed: any codec work should plan to ride the existing round-trip
  harness (xml → Score JSON → xml, same DOM comparison).
