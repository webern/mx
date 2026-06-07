# gen State

## Milestone

M6C_CONFIG_FILE, in progress.

## What the last session did (2026-06-07, M6C session 1)

Grilled the user on the first M6C changeset, then implemented it:

- Created `gen/cpp/` with `config.toml` (routing), `simple_value_h.j2`, `simple_value_cpp.j2`
- Added `/opt/gen-venv` with Jinja2==3.1.6 to the Dockerfile (separate from quality-venv)
- Added `make generate` target (runs `gen/generate.py` inside Docker)
- Modified `gen/generate.py`: simple-value elements (101 elements) now render via Jinja2 templates.
  The `_render_simple_value()` function builds a context dict from existing Python lookup tables and
  renders the templates. The old fake-CT path through `generate_element_h/cpp` is removed for this
  category.
- Verified zero diff across all 202 simple-value files

Not yet committed or tested through Docker build / CI. The user needs to rebuild the Docker image
(`make generate` will trigger it) and verify the full oracle:
`make generate && make fmt && git diff --quiet src/private/mx/core`.

## What the next session should do

Get instructions from the user. Likely options:
- Continue M6C: template the next element category (text-value, empty, empty-with-attrs, etc.)
- At some point, lookup tables (TYPE_DEFAULT_VALUE, etc.) can move to TOML once all their consumers
  are templated

## Oracle (how to prove zero diff)

`make generate && make fmt && git diff --quiet src/private/mx/core`

Then `make test-core-dev`. Reset generated C++ before committing:
`git checkout -- src/private/mx/core` (the refactor must change only `gen/` files).

## Gotchas

- `make fmt` (~1 min, Docker) is part of the oracle - the generator emits unformatted C++.
- The generator now requires Jinja2. Running `python3 gen/generate.py` bare requires a Python
  environment with `jinja2` and `tomllib` (Python 3.11+). Use `make generate` to run inside Docker.
- CI `linux-gate` runs `make gen-quality` (floor 37.7) and `make gen-lint` (floor 9.4). The new
  `_render_simple_value` function and imports should be scored normally.
- `gen-quality`/`gen-lint` are otherwise ignored during the refactor (user directive) unless CI
  fails.
- Jinja2 environment uses `trim_blocks=True` and `lstrip_blocks=True` to avoid extra blank lines
  from block tags. Do not use `-%}` suffix on block tags in templates - it eats leading indentation.
- `node_id` fields are `compare=False` on purpose; keep it that way.
