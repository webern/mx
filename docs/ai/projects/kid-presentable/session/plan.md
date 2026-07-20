# kid-presentable Plan

Milestones (draft; only M1 is in flight — later milestones depend on which design the user accepts):

## M1: Design recommendation (in progress)

Research the `mx::api` surface, the gen/ generator system, and the realistic binding strategies
(C shim + per-language binding generators, IDL-first codegen, WIT/component-model, serialization
boundary, full api regeneration from a spec). Produce `design.md` with a concrete recommendation,
including what the IDL (if any) looks like, what is generated vs. hand-written, and a migration
path that does not break existing C++ users.

## M2: Proof-of-concept C ABI slice (not started)

Hand-write (or generate) a thin C ABI for a small vertical slice — e.g. load a MusicXML file,
read parts/measures/notes, write it back — and prove it from one foreign language (likely
JavaScript via emscripten, since that was the triggering request).

## M3: Generator-backed API description (not started)

If the accepted design is IDL/manifest-driven: teach gen/ (or a sibling tool) to read the api
description and emit the C header + C++ shim, replacing the hand-written M2 slice.

## M4: First real language target (not started)

Ship one complete foreign-language binding (JS/WASM or Swift) built on the M2/M3 layer, with CI.
