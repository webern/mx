# mx generator (`gen/`)

A Python pipeline that reads a MusicXML XSD and emits typed
serialization/deserialization libraries. The C++ `mx::core` model
(`src/private/mx/core/generated/`) is its primary output. Secondary Go, C, and JSON Schema
targets validate language-agnosticism.

## How it works

```
XSD  ->  XSD model  ->  IR  ->  Plates  ->  press  ->  C++ / Go / C / JSON Schema
         (gen/xsd)      (gen/ir)  (gen/plates)  (gen/press)
```

Parse the XSD. Lower it to a language-agnostic IR. Project the IR onto one target as the
**Plates** (per-type metadata: identifiers in each casing, type mappings, emit strategies,
file assignment). Render the target's Mustache templates through the **press** per its
`[render]` manifest.

A **target** is a directory: `config.toml` plus `templates/`. All language knowledge lives
there. Adding a new language touches no generator Python.

## Running it

Commands, gates, and workflows: see `gen/AGENTS.md`. Full runs: `make gen-cpp` (C++ target),
`make gen` (all targets). Generated output is committed; the CI drift check regenerates
(`make gen`) and asserts `git diff --exit-code`.

## For agents

Read `gen/AGENTS.md` before working on the generator. It has the hot path: commands, gates,
cardinal rules, and what must never break.

Read `gen/DESIGN.md` for the full design: goals, pipeline stage responsibilities, IR model and
glossary, Plates architecture (name model, override system, collision detection), the press
and Mustache spec, generator agnosticism rationale, and alternatives considered.
