# M6B Data Model - Generator Architecture

Static design snapshot for the M6B_DATA_MODEL refactor. Describes the target architecture; the
Status section below marks what is built versus still planned, and the realized sections describe
the code as it actually stands. How the design was reached is in `log.md`, not here.

## Purpose

Insert a clean seam into the ~14k-line generator so generation flows `parse -> configure -> render`.
Each `generate_*` function consumes a fully-resolved per-unit context struct instead of navigating
the global `model` and config dicts inline while it emits. This is a pure refactor: the emitted C++
under `src/private/mx/core/` is byte-identical before and after.

The longer-range payoff (later milestones) is that `parse.py` becomes a target-neutral, reusable
artifact that a future Rust / docs / JSON-schema backend can consume. M6B does not build a second
backend; it only establishes the seam.

## Status

Built: `parse.py` and `ids.py`. Their sections below describe the code as it exists. Not yet built:
`configure.py`, `contexts.py`, `render/`, `naming.py`, `cpptypes.py` - those sections describe the
target, and `generate.py` still owns config, naming, type maps, and all emission. The strangler
migration (bottom) has step 1 complete; no unit-kind has been routed through a context/renderer yet.

Two known gaps between the target and the current code, called out where relevant below:

- `parse.py` is not yet C++-name-free: it computes the `class_names` set and `type_usage_count` and
  houses `pascal()`. The target moves `class_names` into `configure.py`'s resolve phase and `pascal`
  into `naming.py`.
- Only `model.tree` is severed; `model.root` is still reachable and read by the four bespoke handlers.

## Pipeline

```
parse.py      pure XSD model, total IDs        (target-neutral, reusable)
   |
configure.py  one config pass, C++-aware       (output IS the contexts)
   |            phase 1: resolve flat indices
   |            phase 2: build per-unit structs
   v
render/*.py   pure f-string functions          (struct -> string)
```

### parse.py

Parses `docs/musicxml.xsd` into a self-contained data structure of XSD facts: no type maps and no
C++ emission. Two C++-lexicon leaks remain pending the `configure.py` split - the model still
computes the `class_names` set (pascal of every element and complexType name) and `type_usage_count`,
and `pascal()` itself lives here. The C++-aware structural config the parser consumes (which
anonymous sequences become synthetic group classes, which inherited groups are renamed) is not
hardcoded here; `generate.py` injects it as a `ParseConfig` so `parse.py` stays config-free. The four
synthetic-group sets in `ParseConfig` are passed by reference: the parser records discovered synthetic
groups into them during parse, and the emission code reads the same objects afterward. This is the
only stage that touches the XML.

Self-containment invariant: after parse returns, the ElementTree is not retained on the model.
`model.tree` is severed. `model.root` is still read by the four bespoke family handlers in
`generate.py` (harmony-chord, score-wrapper, music-data, full-note) and cannot be dropped until those
families migrate to parsed data, so for now it survives scoped to bespoke-only. The former
general-path leaks are closed: enum documentation moved into parse (`model.enum_docs`, consumed by
`generate_enums_h`) and the "pattern B" complex-content predicate moved in too
(`model.complex_content_or_group_cts`, consumed by `_ct_has_complex_content`).

parse.py assigns a `NodeId` to every dataclass-backed node (see IDs below).

### configure.py

Target; not yet built - `generate.py` does configuration and emission inline today.

A single configuration pass. It is allowed to be C++-aware: lexicon config (type maps, class-name
overrides, member naming, license) and structural config (tree-ness, choice-as-set, group synthesis,
skips, bespoke shaping) are not split apart in this milestone. The lexicon/neutral split is
deferred; future non-C++ targets fork after `parse.py`, not inside `configure.py`.

`configure.py`'s output is the set of per-unit context structs (the "contexts"). It builds them in
two flat phases - no topological sort:

1. Resolve: walk all nodes, populate flat indices keyed by `NodeId` - each node's C++ class name,
   header, resolved member type, the `class_names` set (currently computed in `parse.py`; moves here).
2. Build: walk again, emit one self-contained context struct per emittable unit, reading already-
   resolved values from the phase-1 indices.

The two-phase split is what lets a unit's struct embed its dependencies' resolved values without an
ordering pass: phase 1 is complete before phase 2 reads it. Cross-unit references in this codebase
are all name-derivable (a child's class name / header / member type come from the child's name via
flat lookups), so no leaf-first ordering and no render-time context mutation are needed. Both of
those plan ideas are dropped as YAGNI.

### render/

Target; not yet built - the `generate_*` emitters in `generate.py` still navigate `model` and the
config globals inline.

One pure module per unit-kind (element, attrs, group, choice, container, enums), plus one per
bespoke family. Each renderer is `context_struct -> str` built from f-strings. A renderer reads only
its struct: no `model`, no config dicts, no type resolution, no XSD. No template engine; f-strings
stay, made pure.

## IDs

`NodeId` (in `ids.py`) is a typed value with a canonical string form. `XsdModel._assign_ids` assigns
one to every dataclass-backed node in the parsed model: the named roots (`el:` elements, `cx:`
complexTypes, `st:` enum simpleTypes, `gr:` groups, `ag:` attributeGroups), their `at:` attributes,
the `el:`/`gr:` child-refs and the `seq#`/`choice#` content-tree particles beneath them, and
anonymous element-local complexTypes (`el:name/cx#0`). Constructs the model stores as plain dicts or
lists (non-enum simpleTypes; the group and attributeGroup containers themselves) and the anonymous XSD
plumbing wrappers (restriction/extension/simpleContent/complexContent) are not node objects and get no
ID. IDs are additive and unconsumed - nothing reads them yet.

- Named root construct -> one segment `kind:name`: `el:note`, `cx:note-type`, `st:above-below`,
  `gr:editorial`, `ag:bend-sound`. Unique within its XSD symbol space; stable across MusicXML
  versions for free.
- Any child node -> parent-ID + `/` + a segment:
  - named child: `kind:name` -> `cx:note-type/at:type`, `cx:note-type/el:pitch`
  - anonymous child: `kind#ordinal` -> `cx:note-type/seq#0`, `cx:note-type/seq#0/choice#1`

Kind is embedded in every segment (a local element `pitch` and a local attribute `pitch` under one
owner must not collide). Same-kind anonymous siblings get an ordinal.

Stability: only Tier-1 named roots are version-stable, and that is sufficient. Every structural-
config dict keys off a named construct (element name or complexType name) and reaches into anonymous
structure by ordinal-local-to-the-owner or a human-assigned slug - never by a global anonymous ID.
So nested/anonymous IDs are positional and run-local by design; their cross-version instability is
harmless.

### Anonymous-construct inventory (from docs/musicxml.xsd)

A census of the XSD's anonymous constructs the total scheme aims to cover (not all are minted yet -
see below): 7 anonymous element-local complexTypes, 21 nested sequence particles, 18 nested choice
particles, 57 group-ref occurrences, 2 inline simpleTypes, 4 inline unions. Named-but-locally-scoped
(owner-scoped by local name): 419 local element decls, 276 local attribute decls.

Current coverage vs this census: the node-backed constructs are minted (anonymous element-local
complexTypes, nested sequence/choice particles, group refs, local element/attribute decls, and the
body particles of a named owner). The inline simpleTypes/unions are carried as type strings rather
than node objects, and the restriction/extension/simpleContent/complexContent plumbing wrappers are
not modeled at all, so neither is minted today.

## Bespoke families

Target; not yet built - all seven families still emit inline, and four `model.root` walks remain
inside them (harmony-chord, score-wrapper, music-data, full-note).

The seven `BESPOKE_ELEMENTS` (credit, lyric, part-list, harmony, score-wrapper, note, direction)
conform to the architecture (G1): build logic moves to `configure.py`, renderers go pure (read only
their struct). They keep their own struct types and renderers rather than being forced through the
shared generic path. Collapse a family into the shared structs only where it is obviously clean
(e.g. part-list, credit, harmony); leave the irregular ones (note, direction) with a specialized but
still-pure renderer. The non-negotiable is the invariant - purity and the build/render split - not
full unification.

## Module layout

```
gen/parse.py        XSD model, dataclasses, parsing, NodeId assignment   [exists]
gen/ids.py          NodeId typed value                                   [exists]
gen/configure.py    config dicts + two-phase build                       [planned]
gen/contexts.py     per-unit context struct definitions                  [planned]
gen/render/         one pure module per unit-kind + per bespoke family   [planned]
gen/naming.py       pascal/camel/class-name helpers (C++ lexicon)        [planned]
gen/cpptypes.py     XSD-to-C++ type maps (C++ lexicon)                    [planned]
gen/generate.py     orchestrator + all not-yet-migrated config/emission  [exists]
```

`generate.py` is still ~13.4k lines and thins out only as kinds migrate. Until `naming.py` exists,
`pascal()` lives in `parse.py` and `camel`/`has_flag_name` in `generate.py`; until `cpptypes.py`
exists, the XSD-to-C++ type maps live in `generate.py`. The dead `gen_attrs.py` / `gen_enums.py` /
`gen_enum_members.py` helpers have been removed.

## Oracle and migration

Oracle (pure-refactor correctness): the committed C++ equals `python3 gen/generate.py && make fmt`
(raw generator output is unformatted, so the `make fmt` step is required). Thus
`python3 gen/generate.py && make fmt && git diff --quiet src/private/mx/core` must show no diff, and
`make test-core-dev` must pass; `make test-all` before merge. The tightest check during a refactor is
that raw generator output is byte-identical before/after the change (`diff -rq` a pre-change snapshot
of `src/private/mx/core`). `gen-quality` is ignored during the refactor and revisited only if CI
fails at the end.

Migration is strangler-style. `generate.py` stays byte-identical the entire time:

1. [done] Extract `parse.py` + total `NodeId`s as a pure internal move (IDs additive, unconsumed);
   move enum extraction (and the complex-content predicate) in and sever `model.tree`. `model.root`
   survives scoped to the four bespoke handlers pending their migration. Verify zero diff.
2. Migrate one unit-kind at a time (enums -> attrs -> simple elements -> groups -> choices ->
   containers -> tree-parents -> the 7 bespoke families): build that kind's context struct + pure
   renderer, route only that kind through the new path, leave the rest on the old path, verify zero
   C++ diff after each kind.
3. When all kinds are migrated, delete the old dispatch and the `model`/globals reach-back.
