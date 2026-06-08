# mx generator (`gen/`)

Reads the MusicXML XSD and emits typed serialization/deserialization libraries. Runs as
`python3 -m gen`. This document covers the parsing and analysis layer that exists today; for the
build system, Docker toolchain, and corert test, see [`../AGENTS.md`](../AGENTS.md).

## Background

### Pipeline

```
XSD file  --parse-->  XSD model  --lower-->  IR  --emit-->  C++ / Go / C
          (gen.xsd)               (gen.ir)            (templates, not yet built)
```

1. Parse (`gen/xsd/`) reads the XSD into a model mirroring it 1:1, still speaking XSD: restriction
   chains, attribute-group references, anonymous inline types.
2. Lower (`gen/ir/`) resolves that into the intermediate representation (IR): a flat, fully-named,
   dependency-ordered model in code-generation terms. A pure function of the XSD, no configurable
   knobs (see Design principles).
3. Emit turns the IR into code via per-language templates. Not yet implemented.

### Layout

```
gen/
  __main__.py        CLI: analyze | ir | <config.toml>
  xsd/
    model.py         dataclasses mirroring the XSD subset MusicXML uses
    parser.py        ElementTree parser, no external dependencies
    analyze.py       structural analysis + reusable index helpers
  ir/
    model.py         the IR dataclasses
    build.py         lowering from the XSD model to the IR
    resolve.py       collapsed views (group + attribute-group resolution) for emitters
    dump.py          IR to JSON
  cpp/, test/go/, test/c/    per-target config and corert test harnesses
```

### Design principles

- Generate by shape, not by element. Every type falls into one of 8 shapes (4 value + 4 complex,
  defined in the Glossary). One template per shape; no per-element special casing.
- The IR is a pure, canonical function of the XSD. All schema-specific reasoning -- resolving
  references, ordering, dead-code removal, naming -- happens once, in the IR, shared by every
  target. Per-language choices (inheritance vs flattening, mixins vs inlined attributes) belong to
  the emitter. The IR takes no configuration.
- Resolve, but preserve names. The IR data model computes every resolved answer (effective
  primitives, cardinalities, dependency order) yet keeps the schema's named structure (aliases,
  inheritance edges, model groups, attribute groups) so each emitter can decide how much to collapse.
- One resolution, shared. The collapsed form most emitters actually want -- attribute groups
  flattened into a single ordered list, model-group refs spliced into the content, a derived type's
  full attribute set including its base chain -- is *not* duplicated into the data (which would risk
  drift). It is computed on demand by the resolution layer (`ir/resolve.py`), so the
  splicing-and-deduping reasoning lives once and every emitter shares it rather than re-deriving it.

## Usage

```
python3 -m gen analyze [xsd]                     # structural analysis report (text)
python3 -m gen ir [--type NAME] [--resolve] [xsd] # lower to IR, print as JSON (whole IR, or one type)
python3 -m gen <config.toml>                     # emit code for a target (not yet implemented)
```

`--resolve` prints the *collapsed* view of complex types (attribute groups flattened, model-group
refs spliced into the content, derived types carrying their full base-chain attribute set) -- the
form an emitter consumes. Without it, `ir` prints the IR verbatim, with the named structure intact.

`xsd` defaults to `docs/musicxml-4.0-ed15c23.xsd`. Examples:

```
python3 -m gen ir --type note                          # one type
python3 -m gen ir --type note --resolve                # one type, collapsed for an emitter
python3 -m gen ir > build/ir/musicxml-4.0.ir.json      # whole IR (build/ is gitignored)
jq '.complex_types[] | select(.name=="note")' build/ir/musicxml-4.0.ir.json
python3 -m gen analyze docs/musicxml-3.1.xsd           # analyze a different version
```

## Glossary

### XSD source terms

W3C XML Schema constructs MusicXML uses, as the parser sees them.

- simpleType -- a type with no child elements and no attributes: just a constrained text value
  (enumeration, number range, pattern, or union). Becomes an IR value type.
- complexType -- a type for an element with attributes and/or child elements. Becomes an IR complex
  type.
- element -- a named node in the document. In MusicXML every element is declared inline as
  `name="x" type="y"`; there are no global element refs.
- attribute -- a `name="value"` pair on an element. Its type is always a simpleType or builtin.
- group (`xs:group`) -- a named, reusable fragment of element content (a sequence/choice) spliced
  into complex types by reference. No identity in the XML document.
- attributeGroup -- a named, reusable bundle of attributes referenced by complex types.
- restriction / extension -- the two ways one type derives from another: narrowing with facets, or
  adding to it.
- simpleContent / complexContent -- a complex type whose body is a text value plus attributes
  (simpleContent), or that derives from another complex type (complexContent).
- facet -- a constraint on a simpleType: `enumeration`, `pattern`, `minInclusive`, `maxInclusive`,
  `minExclusive`, `maxExclusive`, `minLength`, `maxLength`, `length`.
- particle -- a piece of a content model: an element, `sequence`, `choice`, or group ref, each with
  `minOccurs`/`maxOccurs`.
- anonymous (inline) type -- a type defined in place on an element rather than named at top level.
  The IR names and hoists these (see synthesized type).

### IR stats keys

The `stats` block summarizes the lowered model. Every key:

- value_types (143) -- IR value types: a single scalar value, no child elements. Lowered from XSD
  simpleTypes (plus the text body of simpleContent complex types).
- value_kinds -- value types by kind:
  - enum (96) -- a closed set of allowed string tokens, e.g. `step` = {A..G}. IR fields: `base` (the
    primitive the tokens are drawn from, usually `token`/`string`) and `values`. Emits an enum class
    plus string<->enum lookup tables.
  - number (25) -- a numeric value whose resolved primitive is `decimal`/`integer`/
    `positive_integer`/`non_negative_integer`, with optional bounds. IR fields: `base`, and any of
    `min_inclusive`/`max_inclusive`/`min_exclusive`/`max_exclusive`. Includes numeric aliases (a
    named type that just renames a numeric primitive, e.g. `divisions`). Emits a numeric wrapper
    that range-validates on assignment.
  - string (18) -- a text value (primitive `string`/`token`/`nmtoken`/`date`) with optional
    `patterns` and length constraints. Includes plain string aliases. Emits a string wrapper with an
    optional pattern check.
  - union (4) -- a value that may be any one of several member value types or inline literal sets,
    e.g. `number-or-normal` = decimal | "normal". IR field: `members`, each a `UnionMember` holding
    either a `ref` (a Ref to a value type or primitive) or inline `literals`. Emits a small tagged
    variant.
- complex_types (228) -- IR complex types: elements that carry attributes and/or child elements.
  Lowered from XSD complexTypes (including synthesized ones).
- complex_kinds -- complex types by kind:
  - value (82) -- a typed text body plus attributes (from XSD simpleContent), e.g.
    `accidental-text`. IR fields: `value_type` (a Ref to the body's value type), `attributes`,
    `attribute_groups`. Emits a class with a `value` field plus attribute fields.
  - composite (96) -- child elements arranged in sequences/choices, plus attributes. The structural
    workhorse, e.g. `note`. IR field: `content` (a particle tree). Emits a class with one member per
    child element (cardinality required/optional/vector), order preserved.
  - empty (45) -- an element with no child elements. Two sub-cases the IR unifies: presence-only (a
    bare flag, `presence_only: true`) and attributes-only (attributes but no children, e.g.
    `empty-placement`). Emits a bool or an attributes-only class.
  - derived (5) -- extends another complex type and adds attributes (from XSD complexContent), e.g.
    `metronome-tuplet` extends `time-modification`. IR field: `base` (parent type name). Emits
    inheritance, or a flattened copy where the language has none.
- groups (27) -- XSD model groups carried into the IR as named, reusable content fragments.
- attribute_groups (45) -- XSD attribute groups carried into the IR as named, reusable attribute
  bundles.
- synthesized_types (7) -- complex types the IR created by naming and hoisting anonymous XSD types:
  `score-partwise`, `score-timewise`, `partwise-part`, `partwise-measure`, `timewise-part`,
  `timewise-measure`, `directive`. The part/measure pairs are context-qualified because the partwise
  and timewise hierarchies give them genuinely different shapes.
- dropped_dead_types (5) -- named XSD types nothing references, which the IR omits:
  `empty-print-style`, `empty-print-style-align`, `formatted-symbol`, `positive-decimal`,
  `start-stop-change-continue` (see XSD Analysis).

### IR structural terms

Terms used inside the lowered types, not in `stats`.

- Ref `{ name, category }` -- a typed reference to another type. `category` is `complex` (a
  generated element class), `value` (a generated value type), or `primitive` (a builtin, not
  generated).
- primitive -- a builtin base type the generator does not emit. The IR canonicalizes XSD builtins
  (`xs:decimal` -> `decimal`, `xs:token` -> `token`, ...) and the 10 `xml:`/`xlink:` attribute refs
  into a small primitive set, listed in the `builtins` map.
- attribute -- IR fields: `name`, `type` (a Ref to a value type or primitive), `required`, and
  optional `default`/`fixed`.
- particle -- a node in a complex type's `content`: an `element` (a child occurrence), a `sequence`
  (ordered list), a `choice` (exactly one of), or a `group` reference. Each carries `min`/`max` (max
  may be the string `"unbounded"`).
- cardinality -- the normalized occurrence of an element field: `required` (exactly 1), `optional`
  (0 or 1), or `vector` (repeatable). Derived from min/max.
- presence_only -- true for an empty element with no attributes: its only information is whether it
  appears, so it maps to a bool.
- base -- for a `derived` complex type, the parent complex type it extends. The IR stores only the
  added attributes; inherited attributes are reached through `base`, or flattened in one call by
  `resolve.all_attributes`. `ComplexType.content` is defined for derived types but is currently
  always empty: every MusicXML derivation adds attributes only, never content.
- value_type -- for a `value` complex type, the Ref to the value type of its text body.
- deps -- the complex types a type structurally depends on (child element types + base), resolved
  through groups. Drives the ordering below.
- roots -- the document root elements: `score-partwise` and `score-timewise`.
- builtins -- the map from XSD/external builtin names to canonical IR primitives.

### Resolution layer

The IR data model preserves the schema's named structure; `ir/resolve.py` collapses it on demand.
`Resolver.from_ir(ir)` exposes four read-only accessors over a complex type, none of which mutate the
IR:

- `attributes(ct)` -- the type's own attributes with its `attribute_groups` expanded inline, in
  declaration order, deduped by name. (`note`: 7 own + 5 groups -> 21 attributes.)
- `all_attributes(ct)` -- `attributes(ct)` plus the base chain's attributes, base-most first, for a
  target with no inheritance to lean on. (`mordent`: 3 own -> 20 once `empty-trill-sound` is merged.)
- `content(ct)` -- `ct.content` with every model-group ref spliced in: a self-contained tree of
  elements/sequences/choices with no `group` nodes left. Nesting and all min/max bounds are
  preserved.
- `elements(ct)` -- every element occurrence in the resolved content, in document order, flattened
  across sequences/choices/groups (drops the choice/sequence grouping; use `content` when that
  matters).

`python3 -m gen ir --resolve` dumps this view. `build` itself uses the resolver to compute each
complex type's `deps`, so the group-walking logic lives in exactly one place rather than once per
emitter.

### Ordering

Both `value_types` and `complex_types` are emitted **deps-first**: a type's dependencies always
precede it in the list. The list order *is* the topological order -- there is no separate rank field,
because the array index already encodes it and a duplicate integer would only risk drifting out of
sync. Value types never reference complex types, so concatenating `value_types` then `complex_types`
is a valid total order for a single-file emit. An emitter that wants a different order (alphabetical,
per-file, by shape) has `deps` and can compute its own. Within a value type, union members precede
the union; the only value-to-value dependency is a union referencing its members.

## XSD Analysis

Run `python3 -m gen analyze` for the full report. Key findings for MusicXML 4.0:

### Inventory

145 simpleTypes, 224 complexTypes, 27 model groups, 45 attribute groups, 2 document roots. 440
distinct element names across 478 declaration sites. 351 attribute declarations (60 required).

### Two load-bearing invariants

These hold for 3.0, 3.1, 4.0, and 4.1 alike; the codegen design leans on both:

1. The complex-type graph is a DAG. Zero cycles, zero self-references. So generated code can use
   plain by-value members, emit types in topological order, and skip forward declarations and heap
   indirection entirely -- removing the hardest problem in typed-XML codegen.
2. No element-name collisions. Every element name maps to exactly one type (no global element refs).
   An element's type is fully determined by its name, so parse/serialize dispatch is a flat name ->
   type table with no context-sensitive resolution.

Because these are empirical, not guaranteed by XSD, `analyze` is worth keeping as a CI gate: fail
the build if a future schema introduces a cycle or name collision, before the value-type and
flat-dispatch assumptions silently break.

### The part/measure wrinkle

The one place name -> type is not 1:1 is the document-root scaffolding. `part` and `measure` each
appear as two anonymous types -- partwise nests `part > measure`, timewise nests `measure > part` --
so the IR qualifies them as `partwise-part`/`timewise-part`/etc.

### Dead types

Five named types are defined but referenced by nothing, even indirectly:

| Type                         | Why it is dead                                                     |
|------------------------------|--------------------------------------------------------------------|
| `positive-decimal`           | orphan definition, never referenced                                |
| `start-stop-change-continue` | orphan; elements use `start-stop-change` / `start-stop-continue`   |
| `formatted-symbol`           | superseded by `formatted-symbol-id`, which is the one elements use |
| `empty-print-style`          | superseded by the `-align` / `-id` / `-object` variants            |
| `empty-print-style-align`    | superseded by `empty-print-style-align-id` / `-object`             |

Verified by direct text search of the XSD (independent of the parser): each has exactly one
definition and zero references via `type`/`base`/`ref`/`itemType`/`memberTypes`. The IR drops them
and reports the count. Recognizing the families (print-style, formatted-symbol) is expected -- those
are heavily used; only the specific bare type names are vestigial.
