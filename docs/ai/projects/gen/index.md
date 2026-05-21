---
created: 2026-05-18
---

# gen

## Goal

Reverse engineer the codegen process that produced `mx/core` from MusicXML XSD. Build a generator
that re-produces the existing C++ code from `docs/musicxml.xsd` with minimal unexplained diff, then
point it at MusicXML 4.0 to generate updated types.

## Index

- `plan.md` - milestones from reverse-engineering through MusicXML 4.0 PR
- `state.md` - current session state and next-session instructions
- `log.md` - append-only session log
- `iteration-notes.md` - detailed per-iteration findings from the experiment phase (iters 1-7)
- `design/` - design docs (current-state snapshots, not historical)
  - `overrides.md` - override taxonomy (RULE/EXC/FIX/SUBSTRATE/ANOMALY) and catalog
  - `scoring.md` - CHANGE_PENALTY mechanism and Phase 1 exit gates
  - `forensics.md` - analysis of the original Ruby codegen and mx/core patterns

## Instructions

### Generator location and how to run

The generator and eval tools live at `gen/` (not a subdirectory of the project docs):

```
python3 gen/generate.py          # regenerates C++ on top of src/private/mx/core/elements/
python3 gen/eval.py              # scores the diff
python3 gen/eval.py --verbose    # shows individual penalized lines
```

After studying the diff, reset with `git checkout -- src/private/mx/core/`.

Quality gates: `make fmt && make check && make test-all`.

### Key files outside the project directory

- `gen/generate.py` - the generator (~2800 lines Python)
- `gen/eval.py` - diff scoring script
- `gen/eval_config.yaml` - scoring category rules
- `docs/musicxml.xsd` - the input XSD schema
- `src/private/mx/core/elements/` - the target output directory (591 .h/.cpp pairs)
- `docs/ai/projects/build-and-ci-design.md` - repo-wide build/CI/formatting docs

### Eval config policy

Do NOT autonomously add or change entries in `gen/eval_config.yaml`. When studying the diff, if you
see a pattern that should be zero or low cost, flag it to the user with the pattern, a sample diff
line, and your reasoning. The user decides whether to add it to the config.

### Commit discipline

Each iteration should end with a commit containing generator changes, eval changes, and updated
project docs (state.md, log.md, and any design doc changes). Reset generated code before committing
(`git checkout -- src/private/mx/core/`).

### Design doc maintenance

Design docs in `design/` describe current design state only. When the design evolves, update the doc
in place. Record design changes and rationale in `log.md`.
