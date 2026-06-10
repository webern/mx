# The Galley: the template-facing, target-projected layer

Status: design only. No code in this change. This document specifies the layer that sits between the
IR (`gen/ir`) and the per-language templates in the generator pipeline:

```
XSD file -> XSD model -> IR -> [ Galley ] -> templates -> C++ / Go / C / JSON Schema
            (gen.xsd)   (gen.ir)              (dumb renderers)
```

The IR is a pure, language-agnostic, config-free function of the schema inputs. The Galley is its
opposite number: the per-target projection of that neutral model into a presentation-ready form a
template can print without thinking. It is where config.toml meets the IR. Everything a target needs
to decide -- what an identifier is called in each casing, what a `decimal` maps to, whether a
derived type uses inheritance or a flattened copy, which file a type lands in -- is decided here,
once, so the templates stay dumb: walk the structure, print text, no naming logic and no per-element
special casing.

## 1. Name and rationale

**Chosen name: the Galley** (Python package `gen/galley/`, CLI `python3 -m gen galley --config C`).

In letterpress printing a *galley* is the shallow tray into which a compositor sets the actual sorts
(the metal type) after composing a manuscript: the words are now in concrete type, in final order
and layout, proofed (the "galley proof") before being locked into the press and inked. The metaphor
maps exactly onto this layer:

- The IR is the abstract manuscript: neutral content, no typeface, no layout.
- The Galley is the composed tray: the same content rendered into a *specific* target's concrete
  identifiers (the casing is the typeface), in that target's order and file layout, ready for the
  press.
- The templates are the press: they ink and print what the galley already arranged. They add no
  composition decisions of their own.
- `python3 -m gen galley --config C` is the galley proof: a dumpable, diffable preview of the
  composed tray before any code is printed -- the same role `ir --resolve` plays for the IR.

The name is evocative, thematically apt for a project about music engraving (itself a printing
discipline), and -- importantly -- collides with nothing already in this codebase: not `model`, not
`IR`, not `facet` (which already means an XSD constraint here), not `resolve`. It reads cleanly as a
noun, a module, and a command alongside `ir`.

### Alternatives considered and rejected

- **ViewModel** -- conceptually the most precise fit (MVVM's "presentation-ready projection of the
  model that the view consumes" is exactly this). Rejected: the brief explicitly bars overloading
  "model", and the term drags in web-framework baggage.
- **Projection / Project** -- accurate (the IR is "projected" onto a target), but `project` is badly
  overloaded in this repo (there is a `/project` skill and a `docs/ai/projects/` tree), and `gen
  project` reads as a noun command while colliding with the verb. The CLI ergonomics alone
  disqualify it.
- **Binding** -- the brief's own framing ("target-binding stage") endorses it, and in compiler terms
  binding-to-a-target is exactly this. Rejected for the audience: to a systems engineer "binding"
  reads first as FFI / language bindings (bindgen, "Rust bindings to libfoo"). Since this project
  literally emits C++/C/Go libraries, naming the layer "Binding" actively invites the wrong reading.
  The term survives as the name of the *per-target field group* inside the Galley (see section 4),
  where the FFI reading cannot intrude.
- **Facet** -- already a load-bearing XSD term in this codebase (enumeration/pattern/minInclusive).
  Reusing it would be a genuine collision.
- **Dialect / Idiom** -- evocative of per-language flavor, but both connote *names only* and
  undersell the layer's representation, layout, and structural work.

## 2. Responsibilities and non-responsibilities

The Galley owns the per-target projection and nothing else.

It **is responsible for**:

- Name expansion: every fundamental name gets all standard casings, automatically, plus its
  immutable wire form preserved verbatim (section 5).
- Renames and per-convention overrides, with validation against the IR (section 6).
- Post-projection collision detection as a CI gate (section 7).
- Representation strategy: mapping each of the 8 shapes to an emit strategy, cardinality to the
  target's optional/collection types, and IR primitives to target types via a config-overridable map
  (section 8).
- Resolving `default`/`fixed` literals that name an enum variant to that variant's target
  identifier, while keeping the wire literal (section 8).
- Exposing both the resolved content tree and a flat member list (section 8).
- File/layout partitioning and the per-file include/import graph (optional; section 8).
- Namespaces/packages/prefixes, reserved-word policy, identifier-validity enforcement, doc-comment
  style, deterministic ordering (section 8).

It **is not responsible for** (stays in the IR):

- Schema resolution: collapsing restriction chains, normalizing cardinalities, hoisting anonymous
  types, dropping dead types, dependency ordering, and the group / attribute-group structure. The
  Galley consumes the IR and its `Resolver`; it never re-derives a schema fact.
- The wire names themselves (the Galley preserves them, the IR produces them).
- The sounds.xml fold (an IR-level, config-gated input selection, already done before the Galley
  runs).

It is **not responsible for** (stays in the templates):

- The literal text: language grammar, punctuation, whitespace, file headers, the actual rendering of
  a strategy tag into source lines. Templates contain no naming logic and no per-element
  conditionals; they read Galley fields and print.

## 3. One layer or two: decided by the JSON Schema contrast

The forcing question (section 9 works it fully): a template that emits a JSON Schema version of the
MusicXML spec wants wire names (not casings), the resolved choice/sequence structure, enum wire
literals, union members, number facets, string patterns, the open-enum, and docs as `description`.
It wants **none** of the file partitioning, includes, reserved-word mangling, comment styling, or
casing machinery a code target needs.

That split is real, and it has a sharp consequence: almost everything the JSON Schema target wants
is *already in the IR plus the Resolver*. The IR's names already are the wire names;
`Resolver.content` already splices groups into a choice/sequence tree; union members, number bounds,
patterns, enum values, the open-enum, and `doc` strings are all present. So the neutral half of this
layer is not new information -- it is the IR, re-presented.

This drives the decision:

**The Galley is one rich, materialized, template-facing object, internally partitioned into two
field groups on each node:**

- a **neutral core** -- wire-faithful, target-independent facts (wire name, shape, resolved
  structure, value lists, facets, docs), mirrored from the IR + Resolver; and
- a **target binding** -- the per-target overlay (the casing bundle, resolved target types, emit
  strategy tags, file assignment, reserved-word resolution, doc style).

Code targets read both groups. A neutral target like JSON Schema reads only the neutral core, leaves
the binding's optional pieces (partitioning, includes) unconfigured, and never touches the casings.

**Why one object and not two passes.** Two separate artifacts -- a neutral enrichment layer plus a
detached per-target overlay -- would force every template to cross-reference the two by name and
re-walk the structure to stitch them, re-introducing exactly the per-emitter splicing the IR worked
to centralize. It would also split a wire name from its own casings across two objects. One object
with a disciplined neutral/bound field split gives the ergonomics of one (templates walk a single
tree) and the generality proof of two (the JSON Schema target demonstrably needs only the neutral
fields). The cost -- computing five casings per name and a file assignment even for a target that
ignores them -- is trivial (a few thousand names) and partitioning is opt-out, so neutral targets
pay nothing meaningful.

## 4. Data shape: materialized, dumpable, built on the Resolver

The IR's `Resolver` is computed-on-demand because it is pure over the IR and is needed *mid-build*
(to compute `deps`). The Galley has neither property: it depends on a config (a specific target),
and nothing consumes it mid-build. It is therefore **materialized** -- a plain dataclass tree built
once per target -- for three reasons:

1. Collision detection (section 7) and rename validation (section 6) are global passes over all
   projected identifiers; they are naturally build-then-check steps, which fit a materialized
   result.
2. Inspectability and gating: a materialized tree dumps to JSON via the existing `gen/ir/dump.py`
   machinery, giving `gen galley --config C` as a diffable artifact and a `--check` CI gate,
   matching the project's analyze-as-gate ethos.
3. Templates want random-access to fully-resolved nodes, not recomputation.

It is *built on* the Resolver: the Galley consumes `Resolver.attributes`, `all_attributes`,
`content`, and `elements` rather than re-deriving any splicing.

Design sketch of the types (shapes and accessors, not implementation):

```
# --- the neutral/bound name bundle (R1, R3) ---
Name:
    wire: str                 # immutable on-the-wire string (R3); never a code identifier
    words: tuple[str, ...]    # the tokenized word vector (section 5)
    cased: dict[str, str]     # convention-name -> identifier, e.g. {"pascal": "Note", ...}
    # convenience accessors pascal/camel/snake/kebab/screaming read from `cased`.
    # `cased` is filled by iterating a CONVENTION REGISTRY, so adding a convention
    # later is registering one function -- zero changes elsewhere (R1).

# --- value types (mirror the IR's 4 value shapes) ---
GalleyEnum:    name: Name; base: str; variants: list[Variant]; doc: str|None
Variant:       wire: str; name: Name; ident: str   # ident = sanitized name.cased[variant-conv]
GalleyNumber:  name: Name; base: str; bounds: NumberBounds; target_type: str; doc: str|None
GalleyString:  name: Name; base: str; patterns; length; target_type: str; doc: str|None
GalleyUnion:   name: Name; members: list[UnionMember]; doc: str|None  # member -> Ref or literal set

# --- complex types (mirror the IR's 4 complex shapes) ---
Member:        name: Name; kind: str           # "element" | "attribute" | "value"
               type_ref: GalleyRef; cardinality: str          # required|optional|vector
               repr: MemberRepr                # concrete optional/collection wrapper (section 8)
               default: str|None; fixed: str|None
               default_variant: str|None       # variant ident when default/fixed names a variant
               doc: str|None
GalleyType:    name: Name; shape: str          # value|composite|empty|derived (or value-type shape)
               strategy: str                   # emit-strategy tag the template switches on
               members: list[Member]           # flat, deduped, ordered (code targets)
               content: ContentNode|None       # resolved sequence/choice tree (schema targets)
               base: GalleyRef|None             # derived: the inheritance edge
               all_members: list[Member]|None  # derived: flattened (base chain merged)
               presence_only: bool
               file: FileId|None               # None when partition == single
               doc: str|None

# --- the whole projected target ---
TargetInfo:    language: str; namespace: str; prefix: str
               conventions: list[str]; doc_style: DocStyle; reserved: set[str]; partition: str
Galley:        target: TargetInfo
               value_types: list[GalleyEnum|GalleyNumber|GalleyString|GalleyUnion]  # deps-ordered
               complex_types: list[GalleyType]                                      # deps-ordered
               roots: list[GalleyRef]
               files: list[FileSpec]|None       # per-file include graph; None when not partitioned
               type_map: dict[str, str]         # primitive -> target type, after config overrides
```

Build entry point and CLI (mirrors `ir`):

```
build_galley(ir: Ir, config: Config) -> Galley     # uses Resolver + a NameFactory + collision check
python3 -m gen galley --config C [--type N] [--check]
```

`--check` runs rename validation and collision detection and exits non-zero on any failure, so it
can gate CI exactly as `analyze` does for the DAG/no-collision invariants. Output serializes through
the existing `to_jsonable` in `gen/ir/dump.py`.

## 5. The name-convention model

### 5.1 Tokenizer

A fundamental name is split into an ordered **word vector** of lowercase words, then recased. The
wire form is preserved untouched alongside (R3); tokenization feeds *only* the cased identifiers,
never serialization.

Rules, applied in order:

1. **Separators.** Split on and consume any of: hyphen `-`, dot `.`, underscore `_`, colon `:`, and
   ASCII whitespace. (Hyphen covers ordinary kebab names; dot covers `brass.alphorn`; whitespace
   covers space-separated enum values like `up down` and `bass drum`; colon covers external refs
   like `xml:lang`, `xlink:type`.)
2. **Case-transition splits** (for any already-mixed-case input, rare in MusicXML but the tokenizer
   must be total): split at a lower-to-upper boundary (`fooBar` -> `foo`, `bar`) and at an acronym
   boundary, where an uppercase run is followed by an uppercase+lowercase (`MIDIChannel` -> `midi`,
   `channel`): the last capital of the run begins the next word.
3. **Digits do not split.** A letter-digit or digit-letter boundary is *not* a word boundary, so
   `default-x` -> `[default, x]` (split on the hyphen only), `midi-128` -> `[midi, 128]`, and the
   enum value `1024th` -> `[1024th]` (one word). Digits ride with their adjacent letters.
4. **Lowercase.** Each resulting word is lowercased to its canonical form. Casing is reapplied per
   convention.
5. **Degenerate input.** If the rules yield an empty vector -- the empty-string enum value `""` from
   `positive-integer-or-empty` and a few `*-value` enums -- substitute the configured fallback word
   vector, default `["empty"]`. The wire form stays `""`; only the identifier gets a name.

### 5.2 Recasing

Each convention is a function from the word vector (plus the acronym set) to a string. The five
standard conventions:

- **PascalCase**: capitalize every word, concatenate.
- **camelCase**: the first word fully lowercased, every later word capitalized, concatenate.
- **snake_case**: words joined with `_`.
- **kebab-case**: words joined with `-`.
- **SCREAMING_SNAKE_CASE**: each word uppercased, joined with `_`.

Where "capitalize a word" means: if the word is in the **acronym set**, uppercase it whole (`midi`
-> `MIDI`, `id` -> `ID`); else if its first character is a letter, uppercase that letter and
lowercase the rest; else (a digit-led word like `1024th`) leave it lowercased. The acronym set is
config-extensible (`[naming] acronyms = [...]`); the default is `{midi, id, xml, css, smufl, uri,
url}`. Acronyms affect only PascalCase and the non-leading words of camelCase; snake/kebab/screaming
are case-uniform and ignore the set. (The camelCase *leading* word is always fully lowercased, so a
leading acronym yields `midiChannel`, not `MIDIChannel`.)

Because conventions live in a registry keyed by name, adding (say) `Train-Case` or `dot.case` later
is registering one function; `Name.cased` simply grows a key and templates opt in (R1).

### 5.3 Worked conversion table

| wire             | words            | PascalCase        | camelCase         | snake_case          | kebab-case          | SCREAMING_SNAKE_CASE |
|------------------|------------------|-------------------|-------------------|---------------------|---------------------|----------------------|
| `note`           | [note]           | `Note`            | `note`            | `note`              | `note`              | `NOTE`               |
| `default-x`      | [default, x]     | `DefaultX`        | `defaultX`        | `default_x`         | `default-x`         | `DEFAULT_X`          |
| `clef-octave-change` | [clef, octave, change] | `ClefOctaveChange` | `clefOctaveChange` | `clef_octave_change` | `clef-octave-change` | `CLEF_OCTAVE_CHANGE` |
| `midi-channel`   | [midi, channel]  | `MIDIChannel`     | `midiChannel`     | `midi_channel`      | `midi-channel`      | `MIDI_CHANNEL`       |
| `optional-unique-id` | [optional, unique, id] | `OptionalUniqueID` | `optionalUniqueID` | `optional_unique_id` | `optional-unique-id` | `OPTIONAL_UNIQUE_ID` |
| `brass.alphorn`  | [brass, alphorn] | `BrassAlphorn`    | `brassAlphorn`    | `brass_alphorn`     | `brass-alphorn`     | `BRASS_ALPHORN`      |
| `up down`        | [up, down]       | `UpDown`          | `upDown`          | `up_down`           | `up-down`           | `UP_DOWN`            |
| `1024th`         | [1024th]         | `1024th`          | `1024th`          | `1024th`            | `1024th`            | `1024TH`             |
| `` (empty)       | [empty]          | `Empty`           | `empty`           | `empty`             | `empty`             | `EMPTY`              |

Notes on the hard rows:

- `default-x` shows digit-free splitting on the hyphen and a single-letter trailing word; the wire
  form `default-x` is preserved for the attribute on the wire.
- `midi-channel` and `optional-unique-id` show the acronym set producing `MIDI`/`ID` in PascalCase
  while snake/kebab/screaming stay mechanical.
- `brass.alphorn` shows the dot as a separator while the wire keeps the dot for serialization (R3).
- `up down` shows a space-separated enum value tokenizing cleanly while the wire keeps the space.
- `1024th` shows a digit-led word: the casings are well-defined, but the result is not a legal
  identifier in most code targets. That is fixed in the *binding's* identifier-validity step
  (section 8.6), not here: the wire `1024th` and the recased `1024th` are both kept, and a code
  target mangles to e.g. `_1024th`. The Galley never silently changes the casing to make it legal;
  it records the ideal and lets the sanitizer (and the collision check) act on the result.
- the empty value shows the fallback word vector `["empty"]`; the wire form remains the empty
  string, which is what a serializer must emit.

A note on the dynamics elements `p`, `pp`, `ppp`, `f`, `ff`, ... and `sfz`: these tokenize to single
words and PascalCase to `Pp`, `Ppp`, `Sfz`, which is ugly. That is the textbook motivation for
per-convention overrides (section 6): a target can force `PascalCase = PP` per element without
disturbing the wire form.

## 6. The override system

### 6.1 Two tiers (R4)

- **(a) Fundamental rename.** Rename the canonical root once; every convention re-expands from the
  new root automatically. `attributes` -> `properties` makes PascalCase `Properties`, snake
  `properties`, and so on, with no per-flavor work.
- **(b) Per-convention override.** When one flavor's auto-expansion is unacceptable, override that
  single flavor and leave the rest auto-expanded. Keep fundamental `note`, force `PascalCase =
  MusicNote`, and snake_case still resolves to `note`.

Both tiers are available for any fundamental element name, attribute name, type name, enum type
name, and enum value/variant.

### 6.2 Addressing scheme (R5)

Override keys are namespaced by target-kind so they are unambiguous. Enum values are not globally
unique (`start`, `stop`, `up`, `down` recur across dozens of enums), so an enum-value key is scoped
to its enum type:

| target kind         | key path                                        | notes                                  |
|---------------------|-------------------------------------------------|----------------------------------------|
| type (cplx/val/enum)| `rename.type.<wire-type-name>`                  | one namespace; no collisions invariant |
| element             | `rename.element.<wire-element-name>`            | name -> type is 1:1 (invariant)        |
| attribute (global)  | `rename.attribute.<wire-attr-name>`             | applies on every owner                 |
| attribute (scoped)  | `rename.attribute.<owner-type>.<wire-attr-name>`| more specific; wins over global        |
| enum value          | `rename.enum-value.<enum-type-name>.<wire-value>` | scoped to the enum (R5)              |
| group               | `rename.group.<wire-group-name>`                | for targets emitting shared fragments  |
| attribute group     | `rename.attribute-group.<wire-name>`            | for targets emitting mixins            |

The empty enum value is addressed by the TOML empty-string key `"" = ...` under its enum's table.

### 6.3 TOML schema

Each override entry is a table. A bare `fundamental` key sets the root rename; convention keys
(`pascal`, `camel`, `snake`, `kebab`, `screaming`, or any registered convention) override individual
flavors. A string shorthand `type.note = "tone"` is sugar for a table with only `fundamental`.

```toml
# ---- existing config, untouched ----
[input]
xsd = "../../docs/musicxml-4.0-ed15c23.xsd"
[output]
dir = "../../src/private/mx/core"
[sounds]
xml = "../../docs/sounds-4.0-ed15c23.xml"

# ---- new Galley config ----
[target]
language  = "cpp"
namespace = "mx::core"     # Go: package; C: leave empty and use prefix
prefix    = ""             # global symbol prefix (C uses e.g. "Mx")

[naming]
extends          = "../naming.base.toml"   # optional shared base (section 6.4)
acronyms         = ["midi", "id", "xml", "css", "smufl"]
type-convention  = "pascal"   # which casing type identifiers use
field-convention = "snake"    # which casing member identifiers use
variant-convention = "pascal" # which casing enum variants use
field-prefix     = ""         # e.g. "m_" for member fields (section 8.7)
empty-value-word = "empty"    # fallback word vector for the "" wire value
pluralize-vectors = false     # see section 8.7

[reserved]
words   = ["class", "namespace", "for", "default", "operator"]  # extends language defaults
policy  = "suffix-underscore"        # reserved word -> append "_"
invalid-prefix = "_"                 # leading-digit / empty identifier -> prepend "_"

[types]                              # IR primitive -> target type (overrides defaults)
decimal              = "Decimal"
integer              = "int"
positive_integer     = "unsigned"
non_negative_integer = "unsigned"
string               = "std::string"
token                = "std::string"
nmtoken              = "std::string"
date                 = "std::string"

[layout]
partition     = "per-type"           # "per-type" | "grouped" | "single"
include-style = "quoted"

[docs]
style = "triple-slash"               # "//" | "///" | "/** */"
wrap  = 100

# ---- (a) fundamental rename: all flavors re-expand ----
[rename.type.attributes]
fundamental = "properties"

# shorthand form, identical effect:
# rename.element.default-x = "origin-x"

# ---- (b) per-convention override: keep root, override one flavor ----
[rename.type.note]
pascal = "MusicNote"                 # snake_case still resolves to "note"

# ---- scoped enum-value rename (R5): key scoped to the enum type ----
[rename.enum-value.up-down]
"up"   = "upward"                    # variant 'up' of enum 'up-down' only
"down" = "downward"

[rename.enum-value.breath-mark-value]
"" = "none"                          # the empty variant, scoped to this enum

# ---- scoped vs global attribute rename ----
[rename.attribute]
default-x = "origin-x"               # every owner
[rename.attribute.note]
type = "kind"                        # only the 'type' attribute on 'note'; wins over global
```

### 6.4 Where overrides live and precedence (R6)

Both per-target and shared:

- **Per-target** (the common case): renames are almost always language-driven -- avoiding a C++
  keyword, a Go predeclared identifier -- so they live in each target's `config.toml`.
- **Shared base** (optional): a `naming.base.toml` referenced via `[naming] extends = "..."` holds
  renames common to all targets (rare). A target's own entries win over the base on any conflict.

Precedence, highest first:

1. A per-convention override key (`pascal`, `snake`, ...) for the exact target kind.
2. A `fundamental` rename for that target kind.
3. Auto-expansion from the wire name.

Orthogonally: per-target config beats the shared base; a scoped attribute key beats a global one.

### 6.5 Validation (R6)

Every rename key is validated against the IR at build time and the run **fails loud** on a miss:
`rename.type.<X>` must name a type in the IR; `rename.element.<X>` an element that occurs;
`rename.enum-value.<E>.<V>` an enum `E` that actually lists value `V`; and so on. This matches the
analyze-as-gate ethos: a typo in a rename key (or a key left stale after a schema bump) is a build
error, not a silently ignored line. Chosen and recommended.

## 7. Collision detection (R7)

After tokenizing, recasing, applying renames, and reserved-word / validity mangling, two distinct
fundamental names can collapse to one identifier. The Galley detects these and reports them as
errors (`--check` exits non-zero), the way `analyze` guards the DAG and no-collision invariants
today. The IR's "no element-name collisions" invariant guarantees nothing here, because collisions
are *induced* by the projection (casing, mangling, prefixing), not present in the wire names.

Scopes checked, each in the convention(s) the target actually uses:

- **Type identifiers**: all emitted type identifiers (complex + value + enum) must be unique within
  the target's namespace/package, in the type-convention. (`default-x` the element and `default_x`
  some other name could both snake to `default_x`, etc.)
- **Enum variant identifiers**: unique within each enum type, in the variant-convention. (Distinct
  wire values that mangle to the same identifier -- e.g. several empty/invalid values all sanitized
  to the same fallback -- are caught here, per-enum.)
- **Member identifiers**: within a single complex type's flat member list (attributes + child
  elements + the value body), the field identifiers must be unique in the field-convention. This is
  where an attribute and a child element sharing a recased name, or a pluralized vector member
  colliding with another member, would surface.
- **Group / attribute-group identifiers**: for targets that emit them as shared structs/mixins,
  unique within the relevant namespace.
- **File stems** (when partitioning): unique within the output directory, checked
  **case-insensitively** so `Note.h` and `note.h` are flagged -- a real hazard on macOS and Windows
  filesystems.

The report lists, per collision: the scope, the colliding fundamental (wire) names, and the
identifier they share -- enough to write a targeted rename to resolve it.

## 8. The transformation catalog

### 8.1 Shape -> emit strategy

Each IR shape carries an explicit `strategy` tag the template switches on; the template never
re-derives the shape. The eight shapes and their default strategies:

| IR shape (kind)     | Galley strategy                | Template emits (typical code target)                    |
|---------------------|--------------------------------|---------------------------------------------------------|
| value: enum         | `enum-class`                   | enum class + wire<->variant lookup tables               |
| value: number       | `numeric-wrapper`              | wrapper over a target numeric type, range-validating    |
| value: string       | `string-wrapper`               | wrapper over the target string type, optional pattern   |
| value: union        | `tagged-variant`               | a small tagged variant over the member types            |
| complex: value      | `value-class`                  | class with a `value` field (typed by `value_type`) + attrs |
| complex: composite  | `composite-class`              | class, one member per child element + attrs, order kept |
| complex: empty      | `flag` or `attrs-class`        | bool if `presence_only`, else an attributes-only class  |
| complex: derived    | `inherit` or `flatten`         | base-class inheritance, or a flattened copy (8.4)       |

### 8.2 Cardinality -> optional/collection representation

Each member's `cardinality` (required / optional / vector, already normalized by the IR) projects to
a `MemberRepr` describing the concrete wrapper, filled from a config mapping and the type map:

- **required** -> a by-value member (the DAG invariant means no indirection is ever needed).
- **optional** -> the target's optional: C++ `std::optional<T>`, Go a pointer `*T`, C a `bool has_x`
  plus a value field.
- **vector** -> the target's collection: C++ `std::vector<T>`, Go `[]T`, C a `T* xs; size_t n_xs`.

The Galley computes the descriptor; the template prints the concrete spelling via the type map, so
the choice of wrapper is data, not template logic.

### 8.3 IR primitive -> target type

The IR's primitive set (`string`, `token`, `decimal`, `integer`, `positive_integer`,
`non_negative_integer`, `date`, `nmtoken`) maps to target types through `Galley.type_map`, seeded
with per-language defaults and overridable in `[types]` (section 6.3). The map is the single place a
target decides that `decimal` is a `Decimal` wrapper or that `token` is just `std::string`.

### 8.4 Derived types: inheritance vs flattened

The Galley exposes *both* the `base` edge (for a target with inheritance) and `all_members` (the
base chain merged via `Resolver.all_attributes`, for a target without it). A per-target switch
(`[target] inheritance = true|false`, default true for C++/Go-style structs that can embed, false
for C) selects which the `derived` strategy resolves to (`inherit` vs `flatten`). Templates read
whichever the strategy names; both are present so the choice is config, not a template fork.

### 8.5 Enum variant identifiers and default/fixed resolution

Enum variants are generated from arbitrary wire strings (dots, spaces, leading digits, empty), and
the variant identifier is always distinct from the wire literal, which is retained for serialization
(R3). A `Variant` carries `wire`, the `Name` bundle, and the sanitized `ident`.

When an attribute's `default` or `fixed` value names an enum variant -- e.g. `strong-accent.type`
defaults to `up` against enum `up-down`, `barline.location` defaults to `right` against
`right-left-middle` -- the Galley resolves that wire literal to the variant's target `ident` and
stores it as `Member.default_variant`, so the emitter writes the enum member (`UpDown::Up`) rather
than a raw string, while the wire literal stays available for the serializer. A `default`/`fixed` on
a non-enum member (e.g. `beam.number` default `1`, or `xlink:type` fixed `simple`) is formatted as a
literal of the member's target type (section 8.8), not resolved to a variant.

### 8.6 Identifier validity and reserved words

After recasing and renames, the binding applies a sanitizer per the `[reserved]` policy:

- **Reserved words** (language built-ins plus `[reserved] words`) are mangled by the configured
  policy (default: append `_`, so `class` -> `class_`).
- **Invalid identifiers** -- leading digit (`1024th`), empty result, or any non-identifier character
  that survived -- get the configured `invalid-prefix` (default `_`, so `1024th` -> `_1024th`).

The pre-sanitized casing and the final identifier are both retained; collision detection (section 7)
runs on the *final* identifiers.

### 8.7 Structure: resolved tree and flat member list

Both are exposed, because emitters need different views:

- `GalleyType.content` is the resolved sequence/choice tree (from `Resolver.content`, groups
  spliced), for a target that cares about order and choice structure (a schema emitter).
- `GalleyType.members` is the flat, deduped, cardinality-tagged member list (attributes from
  `Resolver.attributes`/`all_attributes` + elements from `Resolver.elements`), for a code target
  that emits one field per member.

### 8.8 File / layout partitioning (optional)

`[layout] partition` selects the strategy:

- `per-type` -- one type per file. Each `GalleyType` gets a `file`, and `Galley.files` carries, per
  file, the include/import list derived from `deps`: each dependency's file, mapped through the same
  assignment, deduped, self-excluded.
- `grouped` -- types grouped (by shape or by name prefix) into a fixed set of files; same include
  graph, coarser.
- `single` -- one document, `file` is `None`, `Galley.files` is `None`, no include graph. This is
  the JSON Schema case and the explicit reason partitioning is optional rather than assumed.

### 8.9 Namespaces, docs, ordering

- **Namespaces/packages/prefixes**: `TargetInfo.namespace` (C++ `mx::core`, Go package `mx`), and
  `prefix` for languages without namespaces (C symbols `MxNote...`).
- **Doc comments**: the neutral core keeps the raw `doc` text (so JSON Schema can use it verbatim as
  `description`); `TargetInfo.doc_style` carries the comment syntax, wrap column, and escape rules,
  and the template applies them. The Galley does not pre-bake comment syntax into the doc string.
- **Ordering**: the Galley preserves the IR's deps-first order for types (so a single-file emit is a
  valid total order) and document order for members and variants. All config-driven maps are
  iterated deterministically. Determinism is a hard rule: the same IR + config always yields
  byte-identical output.

### 8.10 Optional niceties: accept / reject

- **English pluralization of vector members** -- *rejected as a default*, available as opt-in
  (`[naming] pluralize-vectors`, default `false`). Irregular plurals need a dictionary, the wire
  name is singular, and a wrong plural is worse than a singular member name. Default leaves vector
  members singular; a target that wants plurals enables the flag (naive `+s`) or renames the
  offending member explicitly.
- **Prefix/suffix policy** (`m_` fields, `Enum`/`Type` suffixes) -- *accepted as config*, off by
  default (`[naming] field-prefix`, and analogous type suffix keys). Applied after recasing and
  before collision detection, so a prefix that induces a collision is still caught.
- **Numeric formatting of decimal defaults** -- *accepted*. A `default`/`fixed` literal on a numeric
  member is normalized to the target's spelling for that primitive (e.g. `8` stays `8` for an
  integer field, becomes `8.0` for a `decimal` field if the target wants explicit decimals), with
  the wire literal retained. This reuses the corpus normalization spirit (trailing-zero handling) at
  the identifier layer.

## 9. Forcing function: a JSON Schema emitter

A template emitting a JSON Schema (Draft 2020-12) version of the MusicXML spec reads only the
neutral core of the Galley, and configures `[layout] partition = "single"`. Walkthrough of what it
touches and what it ignores:

What it reads (all neutral-core fields):

- **Names: the wire form, never a casing.** `$defs` keys are the type wire names; object property
  names are element/attribute wire names; enum members are wire values. JSON property names can be
  any string, so `default-x`, `brass.alphorn`, and `up down` are used verbatim -- exactly the data
  `Name.wire` preserves, and exactly why the wire form is a first-class field (R3).
- **Resolved structure.** `GalleyType.content` (the spliced sequence/choice tree) maps directly: a
  `sequence` -> an `object` with ordered `properties` and a `required` list for required members; a
  `choice` -> `oneOf`; a `vector` member -> `{ "type": "array", "items": ... }`; optional vs
  required -> presence in `required`. No group references remain to chase (the Resolver already
  spliced them).
- **Enum wire-literal lists.** `GalleyEnum.variants[*].wire` -> `{ "enum": [...] }`. The variant
  identifiers (`ident`, casings) are not read at all -- proof the casing machinery is inert here.
- **Union members -> `anyOf`.** `GalleyUnion.members` -> `anyOf` of member schemas.
- **The open-enum** (`instrument-sound` = `sound-id` enum unioned with open string) -> `anyOf: [ {
  "enum": [ ...sound ids... ] }, { "type": "string" } ]`. The Galley represents it as an ordinary
  union with an enum member and a string member, so the schema falls out with no special case.
- **Number facets.** `GalleyNumber.bounds` -> `minimum` / `maximum` / `exclusiveMinimum` /
  `exclusiveMaximum`.
- **String facets.** `GalleyString.patterns` -> `pattern`; length -> `minLength` / `maxLength`.
- **Docs -> `description`.** The raw `doc` text, used verbatim.

What it never touches (the entire target binding):

- casings (`Name.cased`), reserved-word and validity mangling (`ident` sanitization), the primitive
  type map, namespaces/prefixes, doc comment style, file partitioning, and the include/import graph.
  It sets `partition = single` and reads no `file` or `files`.

This is the proof the layer is general, not C++-shaped: the JSON Schema target consumes a strict
subset of the same object every code target consumes, needing only the neutral core, while the code
targets layer their binding on top. It is the concrete justification for the one-object,
two-field-group decision in section 3: the neutral core is demonstrably self-sufficient (so the
split is real), but it is delivered as fields of one tree the template walks once (so templates stay
dumb).

## 10. Open questions and future work

- **Pluralization dictionary.** If natural plurals for vector members ever become desirable, a small
  irregular-plural table would be needed; deferred behind the off-by-default flag.
- **Per-context attribute meaning.** The same attribute wire name (`type`, `number`) carries
  different meaning across owners. Scoped attribute overrides handle it manually today;
  auto-detecting divergent uses and warning could be future work.
- **`xs:list` support.** The IR currently maps the (unused) `xs:list` case defensively to a token
  string; if a future schema uses real lists, the Galley would need a list `MemberRepr`.
- **Header/implementation split.** `FileId` is one file per type today; C++ may want a type to span
  a header and a source file, making file assignment one-type-to-many-files.
- **Acronym splitting of arbitrary camel input.** The case-transition tokenizer rule is specified
  but effectively unexercised by MusicXML's kebab names; it would need test coverage before relying
  on it for a future mixed-case schema.
- **Configurable per-enum invalid-identifier prefix.** A single global `invalid-prefix` is assumed;
  some targets might want it per enum (e.g. note-type values prefixed `N`).
