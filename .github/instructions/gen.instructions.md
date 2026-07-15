---
applyTo: "gen/**"
---

`gen/` is a Python code generator (XSD -> IR -> Plates -> Mustache press) with four targets:
C++ (`gen/cpp`, the product), Go, C, JSON Schema. Review against its cardinal rules
(`gen/AGENTS.md`):

- Language-agnosticism: no language knowledge in any `*.py` outside a target's own directory.
  Reject language tables, per-language branches, or target-name checks in generator Python;
  language facts belong in the target's `config.toml` and `templates/`.
- The IR is a pure function of the XSD: no configurable knobs in `gen/ir/` (the `sounds.xml`
  fold is the one documented exception).
- Templates stay dumb: naming/structural decisions belong in Plates or press context, not in
  template text. The Mustache engine has no expressions or filters -- reject attempts to add
  logic to it.
- Fail loud: new error paths must exit non-zero with a `template:line` style message, never
  emit best-effort output.
- Generated output is committed: a generator change without its regenerated output (or vice
  versa) fails the CI drift check (`make gen` then `git diff --exit-code`).
- Template identifiers must be unity-build safe: use `{{ident}}`-suffixed names for
  file-local symbols.
