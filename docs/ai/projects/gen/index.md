---
created: 2026-05-18
m1_completed: 2026-05-21
m2_completed: 2026-05-22
m3_completed: 2026-05-22
---

# gen

## Goal

Reverse engineer the codegen process that produced `mx/core` from MusicXML XSD. Build a
generator that re-produces the existing C++ code from `docs/musicxml.xsd`, then point it
at MusicXML 4.0 to generate updated types.

## Files

Standard project layout (see the `/project` skill).

## Repo conventions introduced by this project

- `{file}.invalid` marker (sibling file, human-readable body) marks a MusicXML input that
  is intentionally not XSD-valid. Honored by the corert suite, ignored by api import.
  Documented in `data/README.md`. Introduced in M3.
- The `src/private/mxtest/corert/` core-roundtrip harness (built out during M2/M3) is
  part of `make test-all` and is the daily driver via `make test-core-dev`.

## Generator location and how to run

The generator lives at `gen/` (not under this project directory):

```
python3 gen/generate.py          # regenerates C++ into src/private/mx/core/elements/
python3 gen/eval.py              # scores diff against checked-in mx/core (secondary signal)
```

Workflow: `python3 gen/generate.py && make fmt && make test-all`. To reset:
`git checkout -- src/private/mx/core/ && git clean -fd src/private/mx/core/`.

## Fitness function and gates

`make test-all` pass/fail is authoritative. **Use `make test-all`, never `make test`** —
the latter builds `dev` and skips the slow `mxtest/file/` round-trippers. Full gate:
`make fmt && make check && make test-all`. Every iteration that touches
`src/private/mx/core/*` ends with a recorded `make test-all` result.

## Cardinal rules

- Never change test cases. Test harness code is fair game with user authorization.
- Never change `mx/api`.
- Minimize changes to `mx/impl`; prefer fixing the generator.
- Do not autonomously edit `gen/eval_config.yaml`. Flag patterns to the user with sample
  diff + reasoning.
- Reset generated `mx/core/` before commit if a generator change was tried and reverted.

## Bespoke generator handlers

Six bespoke handlers in `BESPOKE_ELEMENTS` (`gen/generate.py`): credit, lyric, part-list,
harmony, score-wrapper-family, note, direction. Acceptable when an element's shape cannot
be expressed by the shared rule-based path, but they must still read the parsed XSD model
so spec changes propagate. Pattern: "custom algorithm, schema-driven data" — not a
hardcoded string dump.

Always prefer extending a reusable rule-based path (`TREE_ELEMENTS`,
`TREE_ELEMENT_CONFIG` flags, shared helpers) over a new bespoke handler. If a pattern
could plausibly recur for another element, the fix belongs in the shared path with a
config-driven flag.

## Key external files

- `gen/generate.py` — the generator (~14k lines of Python)
- `gen/eval.py`, `gen/eval_config.yaml` — diff scoring
- `docs/musicxml.xsd` — input schema (currently MusicXML 3.x; swap to 4.0 in M6)
- `src/private/mx/core/elements/` — target output (~590 .h/.cpp pairs)
- `src/private/mxtest/corert/` — core-roundtrip harness
