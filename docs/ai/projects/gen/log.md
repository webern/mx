# gen Log

Append chronologically, oldest on top.

## M1: revgen (2026-05-18 — 2026-05-21, 40 iterations) ✅

Reverse-engineered the codegen.Closed with the generator producing every C++ class in `mx/core`.
Tests still failing; commit `d4f25ee6`

## M2: fix-gen (2026-05-21 — 2026-05-22) ✅

Solved failing test issues

## M3: fix-core-dev (2026-05-22, 5 iterations) ✅

Further test fixes using a new `make test-core-dev` target.

## M4a: test fixer (2026-05-22 -- 2026-05-25) ✅

Built a `Fixer` that patches the expected tree before comparison via per-file `<file>.fixup.xml`
sidecars.

## M5: test coverage expansion (2026-05-25 -- 2026-05-30) ✅

Added real-world corpus files and generated 235 synthetic MusicXML files (`data/synthetic/`) to
achieve 100% symbol coverage of MusicXML 3.0/3.1/4.0 spec symbols. Fixed three corert failures:
PlaybackSound "other" variant (PlaybackSoundType wrapper class), xmlns:xlink preservation
(XMLNS_PRESERVING_ATTRS config in generator). Filed GitHub issue #161 for namespace-prefix
limitation.

Final state: `make test-core-dev` 676/676, `make test-all` all pass.

## M6A: gen-quality tooling (2026-06-01) ✅

Designed a python gen program quality scoring methodology with the user via a grill, then built the
tooling.

Implemented: `gen/quality.py` (scores every `gen/*.py` except itself; writes
`data/testOutput/gen-quality/score.json` with 30 offenders/axis as `path:line` refs, plus
`report.md` and stdout); added a pinned analyzer venv to the Dockerfile; added `make gen-quality` /
`make gen-lint` with bash floor gates; wired both into CI `linux-gate` with a job-summary line and a
per-push PR comment.

Floors are a ratchet, set just under the measured baseline: `GEN_QUALITY_FLOOR`,
`GEN_LINT_FLOOR`.

## 2026-06-02 07:49 M6B design grill ✅

Grilled the user to settle the M6B_DATA_MODEL. Initial grill was not very productive. Design doc
discarded. The core concepts from the grill are as follows.

Seam:
- `parse.py` is target-neutral (pure XSD).
- Next layet, a TOML file is read to configure specifics such as
  - what template file(s) map to which XSD objects
  - what additional transformations are needed to the context, etc.

## 2026-06-02 08:15

M6B session 1: stood up parse.py + ids.py as a pure internal extraction. Zero C++ diff.
