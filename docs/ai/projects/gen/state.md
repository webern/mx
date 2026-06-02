# gen State

## Milestone

**M6B_DATA_MODEL, design settled.** Architecture grilled and captured in
`design/m6b-data-model.md`. Implementation not yet started. This is a pure refactor of the generator
(no C++ output change).

## What the last session did (2026-06-02, M6B design grill)

No code changed. Settled the full M6B architecture with the user. See `log.md` 2026-06-02 for the
decision trail and `design/m6b-data-model.md` for the static design. Headline decisions:

- Pipeline `parse.py` (pure XSD, total NodeIds) -> `configure.py` (one config pass, C++-aware,
  two-phase flat build, output IS the contexts) -> pure f-string renderers (`struct -> str`).
- Total ID scheme, kind-embedded segments, ordinals for anonymous siblings.
- Dropped topological-order and context-mutation as YAGNI; renderers are pure.
- Bespoke families conform (G1) but keep their own structs/renderers.
- Strangler migration; `generate.py` stays byte-identical throughout.

## What the next session should do (M6B session 1)

Stand up `parse.py` as a pure internal extraction - zero C++ diff.

1. Extract `XsdModel` + dataclasses + parsing from `generate.py` into `gen/parse.py`.
2. Move enum extraction out of `generate_enums_h` (`generate.py:854`) into parse, store on the model,
   and sever `model.root`/`model.tree` after parse (self-containment invariant).
3. Add `gen/ids.py` with the `NodeId` typed value and assign a total ID to every node in parse
   (additive; nothing consumes IDs yet).
4. Delete dead `gen/gen_attrs.py`, `gen/gen_enums.py`, `gen/gen_enum_members.py`.
5. Prove the oracle is clean:
   `python3 gen/generate.py && git diff --quiet src/private/mx/core && make test-core-dev`.

Do NOT migrate any unit-kind yet - that is session 2 onward. Keep this increment small and verifiable.

## Gotchas

- The oracle is byte-identical C++ output. After any generator edit run
  `python3 gen/generate.py && git diff --quiet src/private/mx/core` - a non-empty diff means a
  regression. Reset with `git checkout -- src/private/mx/core/ && git clean -fd src/private/mx/core/`.
- `make test-all` is slow (>10 min); iterate on `make test-core-dev`. Full gate before merge:
  `make fmt && make check && make test-all`.
- Ignore `gen-quality` during the refactor (user directive). Only deal with it if `linux-gate` CI
  fails at the very end.
- `git checkout -- src/private/mx/core/` preserves mtimes; incremental cmake can then link stale
  `.o` files and report wrong counts. Use `make clean` for an authoritative measurement.
- The three `gen/gen_*.py` helpers are not imported by `generate.py` - safe to delete.
