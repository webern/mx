# gen/ agent instructions

The `gen/` directory is a Python code generator (`python3 -m gen`) that reads a MusicXML XSD
and emits typed serialization/deserialization libraries. Four targets: C++ (product, `gen/cpp/`),
Go (`gen/test/go/`), C (`gen/test/c/`), JSON Schema (`gen/schema/`). Generated output is committed
to the tree. For design rationale, see `gen/DESIGN.md`.

## Commands

```
python3 -m gen analyze [xsd]                               # structural analysis report
python3 -m gen ir [--type NAME] [--resolve] [--config C]   # lower XSD to IR, print as JSON
python3 -m gen plates --config C [--type NAME] [--check]   # project IR onto target, print
python3 -m gen render --config C --type NAME               # render one type to stdout
python3 -m gen <config.toml>                               # full emit: render the target
```

- `--resolve` prints the collapsed IR view (attribute groups flattened, group refs spliced).
- `--config C` applies that target's companion patches before lowering.
- `--check` on `plates` validates renames and detects identifier collisions; exits non-zero on failure. The same checks run during full generation (`build_plates`), so `make gen` + the CI drift check enforce them -- `--check` is a fast standalone way to run just them.
- Full emit shortcuts: `make gen-cpp` (C++ only), `make gen` (all targets).

## Quality gates

Run all of these after any generator change:

```
make gen-test     # unit tests + test_agnosticism.py + regen + git diff --exit-code
make gen-quality  # design-quality scorer (floor enforced; see quality.py)
make gen-lint     # pylint (config: .pylintrc)
```

After changing a target's `config.toml` or `templates/`, also run that target's corert suite
(`make core-roundtrip-test`, `make test-go`, `make test-c`).

If generated output changes, commit the regenerated files in the same commit as the change
that produced them (the CI drift check -- `make gen` then `git diff --exit-code` -- fails on drift).

## Cardinal rules

**1. The generator is language-agnostic.** Adding or modifying a target must not require editing
any `*.py` file outside that target's own directory. Language knowledge lives in the target's
`config.toml` and `templates/` -- never in `*.py`. Enforced by `gen/tests/test_agnosticism.py`.
If a template needs new dispatch, add a discriminant flag or context field in
`gen/press/context.py`; language tables or per-language branches in Python are forbidden. Language-
specific data (namespace, package, fn_prefix) belongs under `[vars]` in `config.toml` and passes
through as `{{target.vars.namespace}}`.

**2. The IR is a pure function of the XSD.** No configurable knobs may enter `gen/ir/`. The
`sounds.xml` fold is the one documented exception (opt-in input selection; runs before IR is handed
to Plates).

**3. Plates carry decisions; templates stay dumb.** If a template needs to branch on a naming or
structural fact, that fact belongs in `gen/plates/` or `gen/press/context.py`, not in template
text. The Mustache engine has no logic: no expressions, no filters, no assignments.

**4. Fail loud.** Missing template keys, validation errors, rename mismatches, identifier
collisions -- all exit non-zero with a clear `template:line` message.

## Workflows

**Modifying IR or Plates** -- change `gen/ir/` or `gen/plates/`. Run `make gen-test` (its regen +
drift check runs the rename/collision validation as part of generation).

**Modifying a target** -- edit only that target's `config.toml` and `templates/`. Regenerate,
run the target's corert, commit the updated output.

**Adding a target** -- create a new directory with `config.toml` and `templates/`. Run
`make gen-test` to confirm `test_agnosticism.py` passes.

**Renaming a schema type or element** -- use `[rename.type]`, `[rename.element]`, or
`[rename.attribute]` in the target's `config.toml`. Keys are validated against the IR; a typo or
stale key is a build error.

**Debugging a template** -- `python3 -m gen render --config C --type note` renders one plate to
stdout. The Mustache engine must continue to pass the spec test suite (`gen/tests/test_press.py`).

## Layout

```
gen/
  __main__.py        CLI entry point
  config.py          typed config.toml loader
  names.py           tokenizer, convention registry, sanitizer
  naming.base.toml   renames shared across all targets (segno/coda collision fix)
  quality.py         design-quality scorer
  .pylintrc          pylint config
  xsd/               XSD parser (model.py, parser.py, analyze.py) -- no external deps
  ir/                IR model, lowering (build.py), Resolver (resolve.py), sounds fold
  plates/            projection: model.py, build.py, check.py
  press/             engine.py, context.py, render.py, writer.py
  tests/             test suite including test_agnosticism.py and test_press.py
  cpp/               C++ target: config.toml + templates/
  schema/            JSON Schema target: config.toml + templates/ + out/
  test/go/           Go corert target + harness
  test/c/            C corert target + harness
```

## Key files by task

| Task                         | Files                                               |
|------------------------------|-----------------------------------------------------|
| IR shape or resolution       | `gen/ir/model.py`, `gen/ir/build.py`, `gen/ir/resolve.py` |
| Plates projection            | `gen/plates/model.py`, `gen/plates/build.py`        |
| Collision detection          | `gen/plates/check.py`                               |
| Naming / tokenizer           | `gen/names.py`                                      |
| Mustache engine              | `gen/press/engine.py`                               |
| Context building             | `gen/press/context.py`                              |
| Manifest / write             | `gen/press/render.py`, `gen/press/writer.py`        |
| C++ target                   | `gen/cpp/config.toml`, `gen/cpp/templates/`         |
| Agnosticism enforcement      | `gen/tests/test_agnosticism.py`                     |
| Config schema                | `gen/config.py`                                     |
