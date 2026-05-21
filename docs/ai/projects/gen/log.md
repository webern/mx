# gen Log

## 2026-05-18 (pre-project)

Work began before the /project skill existed. Seven iterations of the codegen experiment were
completed in a single day. The generator (`gen/generate.py`) was built from scratch, starting with
basic element class generation and progressively adding attributes structs, group wrapper classes,
choice classes, enum handling, and fromXElementImpl/streamContents patterns.

Iteration-by-iteration details and 28 numbered findings are preserved in `iteration-notes.md`.

Summary of progress across iterations 1-7:
- Iter 1: basic element generation, 297/631 .h exact. Identified 14 diff categories.
- Iter 2: setValue/parse fix, attrs naming. 233/592 .h exact (regression from attrs refactor).
- Iter 3: include ordering, member ordering, empty private block fix. 386/592 .h exact.
- Iter 4: forward declare alphabetical sort. 386/592 .h exact (minimal change).
- Iter 5: Pattern B method ordering, group wrapper generation for 11 groups. 402/592 .h exact.
- Iter 6: group inlining, fromXElementImpl/streamContents patterns, hasContents, streamAttributes.
  Eval penalty 39,297. 403/592 .h, 186/590 .cpp exact.
- Iter 7: choice classes for 5 elements, enum parse fix, type mappings, 14 elements skipped.
  Eval penalty 29,998. 427/592 .h, 202/590 .cpp exact.

Three design docs were produced through /grill-me sessions: overrides taxonomy, scoring mechanism,
and codegen forensics (all now in `design/`).

## 2026-05-19 12:21

Project created under `docs/ai/projects/gen/`. Stub files only. Prior work existed in agenda.md,
gen-overrides.md, change-penalty.md, and gen-todo-move-me-later/.

## 2026-05-19 12:42

Migrated all prior work into the /project structure:
- Promoted `gen/experiment/*` to `gen/*` (generator is no longer an "experiment")
- Moved design docs into `docs/ai/projects/gen/design/` (overrides.md, scoring.md, forensics.md)
- Moved experiment notes to `iteration-notes.md` as historical reference
- Updated eval.py path references from `gen/experiment/` to `gen/`
- Wrote index.md with instructions for coding agents
- Wrote state.md with current progress and next-session priorities
- Removed old `gen-todo-move-me-later/` directory
- Updated /project skill to clarify that subdirectories are allowed
