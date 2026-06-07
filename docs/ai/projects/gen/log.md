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

## 2026-06-07 13:32

M6C session 1: grill on the first changeset for template/config separation. User chose to start at
the leaf: simple-value elements (101 elements, e.g. Step, Duration). These wrap a single scalar
value with no attributes and no children. Two sub-variants: XMACRO enum types (use ToString/
FromString free functions) vs. everything else (use operator<< and .parse()).

Decisions from the grill:
- Jinja2 for templating (not string.Template or hand-rolled)
- Templates and config in gen/cpp/ (not gen/templates/)
- TOML config is routing-only (lookup tables stay in Python until all consumers are templated)
- No data duplication: Python builds the context dict from existing dicts, passes to Jinja2
- Jinja2 in a new /opt/gen-venv Docker venv (separate from quality-venv)
- New `make generate` target runs the generator inside Docker

## 2026-06-07 13:54

M6C session 1 implementation: created gen/cpp/ with config.toml, simple_value_h.j2,
simple_value_cpp.j2. Added /opt/gen-venv with Jinja2==3.1.6 to Dockerfile. Added `make generate`
target to Makefile. Modified gen/generate.py: simple-value elements now render via Jinja2 templates
instead of the shared generate_element_h/cpp f-string path. Verified zero diff across all 101
simple-value elements (202 files). Non-simple-value elements remain on the f-string path unchanged.

## 2026-06-07 21:33

M6C session 2: two batches of 10 refactoring rounds (20 commits total on wrk branch).

Batch 1 (rounds 1-10): extract config from generate.py into dedicated Python modules.
- type_maps.py (280 lines): XSD-to-C++ type mapping tables and resolution logic
- naming.py (37 lines): C++ keywords, camel/pascal helpers
- overrides.py (64 lines): per-element/attribute behavioral overrides
- element_config.py (207 lines): element dispatch config, choice tables, dynamics marks
- group_config.py (113 lines): group mutable sets, static dicts, group_class_name
- attrs_config.py (41 lines): attribute struct naming config, resolve_attrs_name
- score_config.py (59 lines): score wrapper partwise/timewise flavor knobs
- Also moved default-value tables to config.toml

Batch 2 (rounds 11-20): TOML config and Jinja2 templates.
TOML additions (config.toml grew from 69 to 281 lines):
- [overrides.attr_default]: 25 per-attribute default value overrides
- [overrides]: xmlns_preserving_attrs, has_contents_always_true, child_init_value
- [score_wrapper]: partwise/timewise behavioral knobs (17 fields each)
- [attrs]: core_root, shared, type_alias
- [elements]: overwrite_file_stems, skip, bespoke_family_owned, tree, class_name_override, value_type_override
- [groups]: generate, wrapping_streamcontents, real_from_x, unbounded_import_group_after, nested_optional_as_group, unbounded_as_group, extension_rename

New Jinja2 templates (4 templates, 380 lines total):
- group_h.j2, group_cpp.j2: group header/impl generation
- attrs_h.j2, attrs_cpp.j2: attrs struct header/impl generation

generate.py reduced from 13,441 to 12,343 lines. All syntax verified. Jinja2 infrastructure
preserved throughout. Cannot run full oracle (jinja2 not installed locally) but all Python modules
parse cleanly and cross-imports verified.
