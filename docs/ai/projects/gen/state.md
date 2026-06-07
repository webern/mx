# gen State

## Milestone

M6C_CONFIG_FILE, in progress.

## What the last session did (2026-06-07, M6C session 2)

Two batches of 10 refactoring rounds (20 commits on `wrk` branch, not pushed):

**Batch 1**: Extracted config from the generate.py monolith into 7 Python modules:
- `gen/type_maps.py`, `gen/naming.py`, `gen/overrides.py`, `gen/element_config.py`,
  `gen/group_config.py`, `gen/attrs_config.py`, `gen/score_config.py`

**Batch 2**: Moved data to TOML config and created Jinja2 templates:
- `gen/cpp/config.toml` grew from 69 to 281 lines (all override tables, element dispatch,
  group config, attrs naming, score wrapper config)
- 4 new Jinja2 templates: `group_h.j2`, `group_cpp.j2`, `attrs_h.j2`, `attrs_cpp.j2`
- `generate.py` down to 12,343 lines (from 13,441)

All Python syntax verified. Jinja2 infrastructure intact. Zero-diff oracle not yet run (needs Docker
rebuild for jinja2 env).

## What the next session should do

1. Run the oracle: `make generate && make fmt && git diff --quiet src/private/mx/core` to confirm
   zero diff. If any diff, debug and fix.
2. Run `make gen-quality` and `make gen-lint` to check floor compliance.
3. Continue M6C: more f-string functions to templates (choice_class_h/cpp, element_h/cpp are big
   targets). More lookup tables to TOML as their consumers get templated.
4. Eventually: squash or organize the 20 commits for a clean PR.

## Oracle (how to prove zero diff)

`make generate && make fmt && git diff --quiet src/private/mx/core`

Then `make test-core-dev`. Reset generated C++ before committing:
`git checkout -- src/private/mx/core` (the refactor must change only `gen/` files).

## Gotchas

- `make fmt` (~1 min, Docker) is part of the oracle - the generator emits unformatted C++.
- The generator now requires Jinja2. Running `python3 gen/generate.py` bare requires a Python
  environment with `jinja2` and `tomllib` (Python 3.11+). Use `make generate` to run inside Docker.
- CI `linux-gate` runs `make gen-quality` (floor 37.7) and `make gen-lint` (floor 9.4).
- `gen-quality`/`gen-lint` are otherwise ignored during the refactor (user directive) unless CI
  fails.
- Jinja2 environment uses `trim_blocks=True` and `lstrip_blocks=True`. Do not use `-%}` suffix on
  block tags in templates.
- The new modules (`overrides.py`, `attrs_config.py`, `score_config.py`, `element_config.py`,
  `group_config.py`) each independently load config.toml. This is fine at import time but means
  the TOML is parsed multiple times. Not a perf concern for a code generator.
- `SYNTHETIC_OPTIONAL_GROUPS`, `SYNTHETIC_UNBOUNDED_GROUPS`, `SUPPRESS_GROUP_SUFFIX` remain as
  mutable Python sets in `group_config.py` (they're passed by reference to ParseConfig and
  mutated during parsing). They cannot move to TOML.
- `BESPOKE_ELEMENTS` dict maps to function objects - cannot be extracted to config.
- The `_emit_ctor_init` line-wrapping logic and `_emit_group_real_from_x_impl` are pre-rendered
  in Python and passed as strings to templates (too complex for Jinja2 logic).
