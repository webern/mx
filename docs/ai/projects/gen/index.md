---
created: 2026-05-18
m1_completed: 2026-05-21
m2_completed: 2026-05-22
m3_completed: 2026-05-22
---

# gen

## Goal

Reverse engineer the codegen process that produced `mx/core` from MusicXML XSD. Build a generator
that re-produces the existing C++ code from `docs/musicxml.xsd`, then point it at MusicXML 4.0 to
generate updated types.

## Files

- `plan.md` - milestones and exit criteria
- `state.md` - current session state and next-session instructions
- `log.md` - append-only session log

## Repo conventions introduced by this project

- `{file}.invalid` marker: a sibling file next to any MusicXML input that is
  intentionally not valid against the XSD. Body is a human-readable
  explanation. The core roundtrip suite skips such inputs; api import does
  not. Convention documented in `data/README.md`.

## Generator location and how to run

The generator lives at `gen/` (not under this project directory):

```
python3 gen/generate.py          # regenerates C++ into src/private/mx/core/elements/
python3 gen/eval.py              # scores diff against checked-in mx/core (secondary signal)
```

Workflow: `python3 gen/generate.py && make fmt && make test-all`, then reset:
`git checkout -- src/private/mx/core/ && git clean -fd src/private/mx/core/`.

Quality gates: `make fmt && make check && make test-all`.

## Fitness function

`make test-all` pass/fail. **Always use `make test-all`, never `make test`** — the latter builds
`dev` and skips the slow `mxtest/file/` round-trippers, under-reporting failures. `make test-all`
takes >10 minutes. Every iteration ends with a recorded `make test-all` result.

## Cardinal rules

- Never change tests.
- Never change `mx/api`.
- Minimize changes to `mx/impl`; prefer fixing the generator.
- Do not autonomously edit `gen/eval_config.yaml`. Flag patterns to the user with sample diff + reasoning.
- Reset generated `mx/core/` before commit.

## Bespoke generator functions

Six bespoke handlers exist (credit, lyric, part-list, harmony, score-wrapper-family, note,
direction), registered in `BESPOKE_ELEMENTS` in `gen/generate.py`. They are acceptable when an
element's shape cannot be expressed by the shared rule-based path, but they must still read the
parsed XSD model so spec changes propagate. Pattern: "custom algorithm, schema-driven data" — not
a hardcoded string dump.

Always prefer extending a reusable rule-based path (`TREE_ELEMENTS`, `TREE_ELEMENT_CONFIG` flags,
shared helpers) over a new bespoke handler. If a pattern could plausibly recur for another
element, the fix belongs in the shared path with a config-driven flag.

## Key external files

- `gen/generate.py` — the generator (~2800 lines of Python)
- `gen/eval.py`, `gen/eval_config.yaml` — diff scoring
- `docs/musicxml.xsd` — input schema (currently MusicXML 3.x; swapped to 4.0 in M5)
- `src/private/mx/core/elements/` — target output (591 .h/.cpp pairs)
