# kid-presentable Log

## 2026-07-17 23:46

Project created via /mx-project new kid-presentable. Goal: design a realistic polyglot
architecture for mx::api (triggered by rpatters1's emscripten question). Session 1 begins with
research (mx-api-doctrine, api surface, gen/ system) toward a design recommendation in design.md.

## 2026-07-18 00:05

Research done: read mx-api-doctrine, DocumentManager.h, ScoreData.h, Result.h, ApiCommon.h,
NoteData.h, TimeChoice.h, gen/DESIGN.md, audit/README.md, Package.swift. Searched GitHub issues
for the rpatters1 emscripten thread; not an issue (likely a discussion), so design.md cites it
without a number. Three findings drove the design: (1) mx::api has document-granularity value
semantics (getData copies the whole ScoreData), so a serialize-the-whole-score boundary loses
nothing; (2) mx::impl is the unportable, corpus-hardened value of the library, so polyglot means
binding to the compiled engine, never regenerating semantics per language; (3) gen/ is already a
language-agnostic IDL machine (IR -> Plates -> press), so the IDL question becomes "give gen a
second input" rather than adopting a foreign toolchain.

Wrote design.md (M1 deliverable). Recommendation: micro C ABI (~8 frozen functions) exchanging
whole scores as versioned Score JSON; a house api-model IDL fed to the gen press emitting the
JSON Schema, the C++ codec, a C++ drift gate, and idiomatic per-language mirrors (TS first);
C++ headers stay hand-written, locked to the model by the gate, optionally flipped to generated
later. Rejected: embind/SWIG direct binding, per-field handle C API, protobuf/flatbuffers,
WIT-as-backbone (revisit when mature), per-language regeneration from XSD, Rust rewrite.

## 2026-07-20 20:28

Goal conversation with the user reshaped the project; design.md demoted to "first spike,
rejected" (kept for reference). The broadened goal: (1) polyglot mx::api — real, supported
libraries in multiple languages with C++ first-class indefinitely (the spike's framing of
"expose the C++ engine" presumed too much about how that happens); (2) a new music notation
language, an alternative to MusicXML, sitting above mx::api as a client of it. Key corrections
from the user: mx::api is not the seed of the new language — it is deliberately bound to
MusicXML and the language must be free of that; MusicXML transfers reliably lose fidelity and
the spec is too loose to prevent it; importers/exporters for real applications (LilyPond,
MuseScore) are needed both as features and as the research method for discovering the
language's model. Session plan/state files were deleted and roadmap/sequence framing retired:
this is an open-ended ideation phase, potentially months long.
