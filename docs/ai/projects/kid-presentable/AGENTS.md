---
created: 2026-07-17
---

# kid-presentable

## Goal

Make `mx::api` a polyglot: usable from languages other than C++ (JavaScript/WASM, Swift, Python,
etc.). This project's first deliverable is a **design recommendation**: a realistic architecture by
which the `mx::api` surface (and the mx::core/impl machinery behind it) can be exposed to other
languages — including whether the API should be hoisted into an interface definition language (IDL)
from which C++, C, and foreign-language bindings are all generated.

Context: rpatters1 asked about emscripten. `mx::api` as written (C++ classes, std::optional,
std::vector, exceptions at the DocumentManager boundary) cannot be fed directly to binding
generators; a C FFI layer would be required. The question is what the right long-term shape is,
not just how to hack up embind.

The project is named after Moe Szyslak's "kid presentable" line.

## Index

- `design.md` — the polyglot design recommendation (the session 1 deliverable; current state of
  the design, kept in sync as it evolves).
- `log.md` — append-only history. `.prompt` — user-owned, never read.
- `session/plan.md`, `session/state.md` — milestones and session handoff (session mode only).

## Instructions

- Read the `mx-api-doctrine` skill before proposing anything that touches the shape of `mx::api`.
- The gen/ system (`gen/AGENTS.md`, `gen/DESIGN.md`) is precedent: this repo already generates
  multi-language code (C++, Go, C toys) from the MusicXML XSD. Any IDL recommendation should be
  weighed against extending that machinery rather than importing a foreign toolchain wholesale.
- Design docs describe current state only; history and rejected alternatives go in `log.md`
  (a short "alternatives considered" section in the design doc is fine).
- This is a design/recommendation project until the user promotes a milestone to implementation.
