# gen State

## Milestone

**M6A complete.** Generator quality gates (`make gen-quality` + `make gen-lint`) are built, wired
into CI, and gating against a ratcheted floor. Next up: M6B_DATA_MODEL.

## What this session did (2026-06-01, M6A)

Designed the scoring methodology with the user (grill), then implemented the tooling:

- `gen/quality.py` rewritten to the agreed rubric: composite 0-100 from structure 50% (LOC-weighted
  function + file size), cyclomatic 25% (radon), cognitive 25% (`cognitive_complexity` lib). One
  smooth `target/max(target,value)` transform per axis, LOC-weighted, so partial refactor wins
  register. Scores every `gen/*.py` except itself. Writes `data/testOutput/gen-quality/score.json`
  (composite, components, 30 offenders/axis with `path:line` refs) + `report.md` + stdout.
- Dropped from the old draft: maintainability-index and Halstead (redundant / step-shaped). pylint
  left the score and became `make gen-lint` (binary pass/fail). Duplication/coupling/cohesion/DIT
  evaluated and excluded (unmeasurable in this f-string-heavy code; jscpd and pylint both read ~0%).
- `gen/.pylintrc` disables the complexity checks gen-quality already scores.
- Dockerfile: pinned analyzer venv at `/opt/quality-venv` (radon 6.0.1, pylint 4.0.5, astroid 4.0.4,
  cognitive_complexity 1.3.0) for deterministic scoring.
- Makefile: `make gen-quality` / `make gen-lint`, docker in/out branches, bash floor gates.
- CI: both gates added to `linux-gate`; report to job summary + a per-push PR comment (coverage
  pattern).
- Deleted dead `gen/eval.py` + `gen/eval_config.yaml` and the old `gen/quality-baseline.json`.

Baseline measured in-container (deterministic, matches local): gen-quality **37.7**, gen-lint
**9.49**. Floors set just under: `GEN_QUALITY_FLOOR=37.7`, `GEN_LINT_FLOOR=9.4`. CI passes "just
barely," per the ratchet plan.

## What the next session should do

- Start M6B_DATA_MODEL (see plan.md): generalize the generator around a common context type.
- Use `make gen-quality` as the refactor compass: `score.json` offenders are the worklist; raise the
  floors as the composite climbs. Worst offenders today: `generate_tree_parent_cpp` (CC 208,
  cognitive 428, 1085 lines), `generate_element_cpp`, `generate_note_cpp`, `main`.

## Gotchas

- The score is deterministic only with the pinned analyzer versions (Dockerfile). Bumping them shifts
  the score - re-measure and reset `GEN_QUALITY_FLOOR` if you do.
- gen-quality/gen-lint run in Docker like fmt/check. For ad-hoc host runs, point a venv with the same
  pins at `python gen/quality.py`.
- The generator's behavior was off-limits this session by user direction (tooling-only PR). The dead
  `OVERWRITE_FILE_STEMS` set in `generate.py` was left in place for that reason.
- `make test-all` is slow (>10 min); iterate on `make test-core-dev` for mx/core changes.
