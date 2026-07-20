---
created: 2026-07-17
---

# kid-presentable

## Goal

An open-ended ideation phase (months, not weeks) with two connected ambitions:

1. **A polyglot mx::api.** Real, supported mx::api libraries in multiple languages — C++ (the
   existing library, first-class indefinitely), Rust, Python, Java, JavaScript/TypeScript.
   mx::api is the MusicXML interop layer: how programs get scores into and out of MusicXML.

2. **A new music notation language** — an alternative to MusicXML — sitting above/outside
   mx::api as a *client* of it (using it for MusicXML import/export). It must not inherit
   mx::api's shape: mx::api is deliberately bound to MusicXML, and the language must be free of
   that. Importers/exporters for real applications (LilyPond, MuseScore, …) are both necessary
   features and the research method: the language's model must be learned from how real
   applications represent notation and where fidelity dies in translation, because MusicXML
   transfers reliably lose fidelity and the spec is too loose to prevent it.

Origin: rpatters1's emscripten question (ambition 1). The project is named after Moe Szyslak's
"kid presentable" line.

## Index

- `design.md` — first spike (micro C ABI + Score JSON + house IDL, C++-engine-centric).
  **Rejected**; kept for reference.
- `log.md` — append-only history. `.prompt` — user-owned, never read.

## Instructions

- Read the `mx-api-doctrine` skill before proposing anything that touches the shape of `mx::api`.
- The gen/ system (`gen/AGENTS.md`, `gen/DESIGN.md`) is precedent: this repo already generates
  multi-language code (C++, Go, C toys) from the MusicXML XSD. Any IDL recommendation should be
  weighed against extending that machinery rather than importing a foreign toolchain wholesale.
- Design docs describe current state only; history and rejected alternatives go in `log.md`
  (a short "alternatives considered" section in the design doc is fine).
- This is an open-ended ideation phase, potentially months long. Do not impose roadmaps,
  sequences, or delivery framing on it; follow the user's lead.
