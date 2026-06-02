# M6B Data Model - Generator Architecture

Static design snapshot for the M6B_DATA_MODEL refactor. Describes the target architecture only,
not how the design was reached (that is in `log.md`).

## Purpose

Insert a clean seam into the ~14k-line generator so generation flows `parse -> configure -> render`.
Each `generate_*` function consumes a fully-resolved per-unit context struct instead of navigating
the global `model` and config dicts inline while it emits. This is a pure refactor: the emitted C++
under `src/private/mx/core/` is byte-identical before and after.

The longer-range payoff (later milestones) is that `parse.py` becomes a target-neutral, reusable
artifact that a future Rust / docs / JSON-schema backend can consume. M6B does not build a second
backend; it only establishes the seam.

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

Parses `docs/musicxml.xsd` into a self-contained data structure. Pure XSD facts: no C++ names, no
type mappings, no config. This is the only stage that touches the XML.

Self-containment invariant: after parse returns, the ElementTree (`model.root` / `model.tree`) is
dropped and never referenced again. Enforced. The one current violation is `generate_enums_h`
(`generate.py:854`), which iterates `model.root` during emission; enum extraction moves into
`parse.py` and is stored on the model.

parse.py assigns a `NodeId` to every node (see IDs below).

### configure.py

A single configuration pass. It is allowed to be C++-aware: lexicon config (type maps, class-name
overrides, member naming, license) and structural config (tree-ness, choice-as-set, group synthesis,
skips, bespoke shaping) are not split apart in this milestone. The lexicon/neutral split is
deferred; future non-C++ targets fork after `parse.py`, not inside `configure.py`.

`configure.py`'s output is the set of per-unit context structs (the "contexts"). It builds them in
two flat phases - no topological sort:

1. Resolve: walk all nodes, populate flat indices keyed by `NodeId` - each node's C++ class name,
   header, resolved member type, the `class_names` set.
2. Build: walk again, emit one self-contained context struct per emittable unit, reading already-
   resolved values from the phase-1 indices.

The two-phase split is what lets a unit's struct embed its dependencies' resolved values without an
ordering pass: phase 1 is complete before phase 2 reads it. Cross-unit references in this codebase
are all name-derivable (a child's class name / header / member type come from the child's name via
flat lookups), so no leaf-first ordering and no render-time context mutation are needed. Both of
those plan ideas are dropped as YAGNI.

### render/

One pure module per unit-kind (element, attrs, group, choice, container, enums), plus one per
bespoke family. Each renderer is `context_struct -> str` built from f-strings. A renderer reads only
its struct: no `model`, no config dicts, no type resolution, no XSD. No template engine; f-strings
stay, made pure.

## IDs

Every node has a `NodeId`, a typed value with a canonical string form. Total coverage: named roots,
local/anonymous children, body particles, and plumbing wrappers all get one.

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

Things that need a minted (Tier-2) ID and become or feed units: 7 anonymous element-local
complexTypes, 21 nested sequence particles, 18 nested choice particles, 57 group-ref occurrences,
2 inline simpleTypes, 4 inline unions. Named-but-locally-scoped (owner-scoped by local name): 419
local element decls, 276 local attribute decls. Body particle of a named owner and anonymous
plumbing (restriction/extension/simpleContent/complexContent wrappers) get IDs too under the total
scheme, though nothing references them today.

## Bespoke families

The seven `BESPOKE_ELEMENTS` (credit, lyric, part-list, harmony, score-wrapper, note, direction)
conform to the architecture (G1): build logic moves to `configure.py`, renderers go pure (read only
their struct). They keep their own struct types and renderers rather than being forced through the
shared generic path. Collapse a family into the shared structs only where it is obviously clean
(e.g. part-list, credit, harmony); leave the irregular ones (note, direction) with a specialized but
still-pure renderer. The non-negotiable is the invariant - purity and the build/render split - not
full unification.

## Module layout

```
gen/parse.py        XSD model, dataclasses, parsing, NodeId assignment
gen/ids.py          NodeId typed value
gen/configure.py    config dicts + two-phase build
gen/contexts.py     per-unit context struct definitions
gen/render/         one pure module per unit-kind + per bespoke family
gen/naming.py       pascal/camel/class-name helpers (C++ lexicon)
gen/cpptypes.py     XSD-to-C++ type maps (C++ lexicon)
gen/generate.py     thin orchestrator (main)
```

Delete the dead, unimported helpers `gen/gen_attrs.py`, `gen/gen_enums.py`,
`gen/gen_enum_members.py`.

## Oracle and migration

Oracle (pure-refactor correctness): `python3 gen/generate.py && git diff --quiet src/private/mx/core`
must show no diff, and `make test-core-dev` must pass; `make test-all` before merge. `gen-quality` is
ignored during the refactor and revisited only if CI fails at the end.

Migration is strangler-style. `generate.py` stays byte-identical the entire time:

1. Extract `parse.py` + total `NodeId`s as a pure internal move (IDs additive, unconsumed); move
   enum extraction in and sever `model.root`. Verify zero diff.
2. Migrate one unit-kind at a time (enums -> attrs -> simple elements -> groups -> choices ->
   containers -> tree-parents -> the 7 bespoke families): build that kind's context struct + pure
   renderer, route only that kind through the new path, leave the rest on the old path, verify zero
   C++ diff after each kind.
3. When all kinds are migrated, delete the old dispatch and the `model`/globals reach-back.
