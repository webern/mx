# Codegen Override Mechanism

Are hand-coded exceptions to codegen rules allowed? What form do they take, and how does
`CODEGEN_PROGRAM_QUALITY` treat them? The
design below is grounded in a catalog of the actual non-derivable points in `mx/core`, found
by deriving code from `docs/musicxml.xsd` and diffing against the checked-in source.

## Context

Two facts shape every decision here:

- **4.0 codegen is a one-time run.** There is no recurring build to protect, so "hard-fail on
  schema drift" is not the right tool. The output of the 4.0 run becomes the new
  hand-maintained `mx/core`.
- **The spec is too large to eyeball.** The risk is not score-gaming. It is that an override
  correct for MusicXML 3.x silently produces wrong code among thousands of 4.0 symbols, with
  no human able to review all of it.

## The mechanism

**Overrides are a first-class declarative input to the generator**, not an afterthought:
`generator(musicxml.xsd, overrides) -> mx/core`. `CODEGEN_PROGRAM_QUALITY` judges the design
of the override *mechanism*, not the number of override entries.

**Generality is the organizing axis.** A general rule is keyed on a structural condition
(collides with a C++ keyword, XSD type is extensible, name clashes with an element class). It
scales to 4.0's new symbols for free and needs zero per-symbol human attention. A named
per-symbol exception covers exactly one 3.x symbol, does not scale, and is one more thing a
human must re-check at 4.0 time.

**The optimization target is minimizing the count of named per-symbol exceptions.** General
rules are effectively free. The rule is the default and the named exception is the last
resort: a named exception is admissible only with a justification that (a) no structural
condition can express the behavior and (b) records the 3.x schema assumption it freezes. That
recorded assumption is the entire 4.0 review checklist - the only realistic substitute for the
hard-fail that a one-time run rules out. `attributes` -> `Properties` is the canonical test:
it is attempted first as a general "XSD name collides with a C++ keyword/convention ->
deterministic rename" rule, not written as a one-off table row.

**Irreducible code exceptions enter via snippet injection into named template slots.** The
element template declares slots (`parse_body`, `extra_private_methods`, `stream_contents`,
...). The declarative manifest binds a slot to a small verbatim `.cpp`/`.h` snippet file. The
generator renders a schema-derived skeleton and fills declared slots; it never names a symbol
in generator code. There are no whole-file escape hatches: even `Note.cpp` is brought into the
mechanism by decomposition into several small, individually justified slots
(`parse_note_choice`, `jit_allocation`, `add_beam_maxoccurs_guard`), not one mega-slot.

**Anti-cheating is structural, not size-based:**

1. The skeleton is a pure function of the XSD - class declaration, member layout (content
   model + cardinality), accessors, `makeXxx` factories, `streamName`, standard interface
   methods. A file whose skeleton is not schema-derived is a checked-in file and is
   disqualified.
2. Slots carry only justified, irreducible bodies, decomposed small.
3. A slot is illegitimate if it has no valid justification, reproduces content a structural
   rule could derive, or subsumes the skeleton. There is no hard per-slot size cap;
   justification plus impartial reviewer is the gate, because an arbitrary cap only moves the
   gaming to artificial slot-splitting.

**Substrate is frozen and namespace-derived.** Foundational types the generator depends on
rather than emits: types resolving to an XSD built-in (`xs:string` -> `XsString`, `xs:token`,
`xs:ID`, `xs:anyURI`, `xs:decimal`, `xs:integer`), to an imported foreign namespace (XLink ->
`XlinkHref/Role/Title`; `xml:` -> `XmlLang/XmlSpace`), the generic numeric base machinery
(`Integer`, `IntRange`, `Decimal`, `DecimalRange`, `PreciseDecimal` - pure clamp/compare/store,
no MusicXML semantics), and runtime infrastructure (`ElementInterface`, `FromXElement.h`,
`ForwardDeclare` macros). Anything defined in the MusicXML target namespace is never
substrate: `Color` looks like a leaf scalar but `color` is a MusicXML `simpleType`, and its
hand-rolled ARGB/RGB parse freezes a 3.x assumption, so it stays generated-or-slotted.
Substrate membership is derived from namespace origin and enforced by static analysis, not
declared by the author; the analyzer rejects any MusicXML-namespace type that resolves to
substrate. "Everything in musicxml.xsd must result in generated output" stays intact because
substrate is the non-MusicXML foundation the schema imports.

## Classification taxonomy

Every non-derivable point in `mx/core` is exactly one of:

- **RULE** - reducible to a structural rule; scales to 4.0 for free; the large majority.
- **EXC** - irreducible named per-symbol exception; needs a slot + justification + recorded
  schema assumption.
- **SUBSTRATE** - frozen, not generation output; membership namespace-derived and
  analyzer-enforced.
- **FIX** - XSD content `mx/core` never emitted. The faithful generator emits it and accepts
  the diff. Treating a FIX as an override-to-preserve, to lower `CHANGE_PENALTY`, is the
  cheating the agenda forbids.
- **ANOMALY** - a suspected legacy bug; verify against the XSD before deciding whether to
  reproduce or correct it.

EXC and FIX are handled oppositely: EXC preserves a deliberate human choice; FIX corrects a
legacy gap and *expects* a diff. The mechanism must therefore classify, not merely override.

## Catalog of non-derivable points

Found by deriving from `docs/musicxml.xsd` (5428 lines; 421 `xs:element`, 116 `xs:simpleType`,
179 `xs:complexType`) and diffing against `mx/core`. Quantitative claims below are measured,
not estimated.

### What is RULE (the derivable bulk)

The generator derives the following without per-symbol input. These scale to 4.0.

| Concern | Rule | Coverage measured |
|---|---|---|
| Class name | PascalCase of XSD element/type name | - |
| `streamName` | verbatim XSD name | - |
| Enum class name | PascalCase; append `Enum` iff it collides with an element/complexType class name | 81 of 85 exact |
| Enum members | camelCase(value split on `-`/`_`/space); append `_` iff C++ keyword; `""` -> `emptystring`; numbered 0..n in XSD order | 77 of 81 exact |
| Numeric type collision suffix | `Value` (vs `Enum` for enums) | - |
| Attributes struct | complexType own attrs + transitive `attributeGroup` refs, flattened in document order; named after the complexType (or the element if the type is anonymous); shared by all elements of that type | 79 of 129 matched exact (rest are FIX-tainted, not rule failures) |
| Sequence content | ordered `<Child>Ptr my<Child>`; optional adds `bool myHas<Child>`; unbounded -> `<Child>Set`; order = XSD sequence order | - |
| Numeric scalars | `<Name> : public IntRange`/`DecimalRange`, bounds from `min`/`maxInclusive` facets (unbounded if absent) | - |
| `measure`/`part` partwise vs timewise | same element name, two content models -> `Partwise*`/`Timewise*` split, both stream the bare name | - |
| Open-extension types | `<Name>Enum` enum + `<Name>`/`<Name>Value` wrapper (`enum + std::string myCustomValue`); inner enum gets `Enum` from collision with the wrapper | - |
| Imported `xml:` attributes | `xml:lang`/`xml:space` -> `XmlLang lang;`/`XmlSpace space;` (SUBSTRATE boundary) | - |
| `xs:group` wrapper classes | `*Group` class with empty `streamName` (transparent wrapper); parent streams its contents | all `*Group` except real elements `Group`/`PartGroup` and the 1 `LayoutGroup` anomaly |
| Document/PI/conversion infrastructure | `Document`, `DocumentHeader`, `ProcessingInstruction`, `ScoreConversions` - top-level machinery and the `XDoc`/ezxml bridge | SUBSTRATE (not MusicXML schema content) |

### EXC - irreducible named exceptions

This is the complete irreducible set. It is small and bounded.

| XSD symbol | C++ artifact | Special handling | Frozen 3.x assumption to record |
|---|---|---|---|
| `attributes` (complexType/element) | `Properties` class, `streamName` "attributes" | renamed; collides with the `<X>Attributes` struct naming convention | `attributes` is the only element whose name collides with the attributes-struct convention |
| `accidental-value`/`accidental` | `Accidental.cpp` remaps `"double-flat"` -> `"flat-flat"` + logs | semantic error-tolerance | `accidental-value` enumeration excludes `double-flat` in 3.x |
| `metronome-relation` | `MetronomeRelation` ctor hardcodes `myValue("equals")`; `setValue` neutered | value frozen | `metronome-relation` enumeration == {`equals`} |
| `dynamics` | `setAttributes` by const-ref value; hand-written `streamContents` emits child tags | non-standard setter + self-serialized child XML | `dynamics` content model is the fixed mark set + `other-dynamics` |
| `note`, `full-note`, music-data group, `measure` (partwise) | `MX_JIT_ALLOCATE`/`MX_MUTEX` lazy child allocation | performance choice, no XSD source | none (performance hint, not a schema fact) |
| `distance-type`, `line-width-type`, `mode` | enum member lists | values mined from `xs:documentation` prose; the schema has no `xs:enumeration` | the value list is the 3.x doc-prose enumeration |
| `note-type-value` | `NoteTypeValue` members | digit-leading values hand-spelled to English (`1024th` -> `oneThousandTwentyFourth`) | the digit-to-English value mapping |
| instrument-sound list | `PlaybackSound.h` X-macro, not the standard enum suite | different enum codegen path | the sound list contents |
| `color`, `comma-separated-text`, `number-or-normal`, `positive-integer-or-empty`, `yes-no-number`, `font-size`, date/time types | `Color`, `CommaSeparated*`, `NumberOrNormal`, `PositiveIntegerOrEmpty`, `YesNoNumber`, `FontSize`, `Date`, `TimeOnly`, `EndingNumber` | bespoke C++ shapes (pImpl, custom parse), not the generic range pattern | each type's lexical space per its 3.x restriction |

### FIX - legacy infidelity the faithful generator must correct

`mx/core` does not faithfully represent the XSD attribute model. These are bugs to fix, not
overrides to preserve.

| XSD symbol | Gap | Measured |
|---|---|---|
| `color` attribute reachable via `print-style` | struct omits the `color` field non-uniformly | 71 complexTypes reach `color` and have a struct; 26 emit it; **45 omit it** (`AccidentalAttributes` confirmed) |
| `text-formatting` `dir`, `valign` | fields omitted in some structs | - |
| required-attribute error string | 46 `.cpp` hardcode `'number' is a required attribute` regardless of the actual attribute | 46 files |

### ANOMALY - verify against the XSD before reproducing

| Symbol | Suspected issue |
|---|---|
| `LayoutGroup` | streams `"work"`; the sole `*Group` with an erroneous non-empty `streamName` (matches the forensics misnamed-group-script finding) |
| `*Choice` inner `enum class Choice` start value | inconsistent with no semantic significance: 15 start at `1` (`ArticulationsChoice`, `BendChoice`, `CreditChoice`, `EncodingChoice`, `FullNoteTypeChoice`, `HarmonicInfoChoice`, `HarmonicTypeChoice`, `LyricTextChoice`, `MusicDataChoice`, `NotationsChoice`, `NoteChoice`, `OrnamentsChoice`, `PercussionChoice`, `SoloOrEnsembleChoice`, `TechnicalChoice`), 6 start at `0`. Normalize to one convention |
| `note-type-value` | hand-spelled value contains a typo: `twoHundredFifthySixth` ("Fifthy") |
| `DocumentChoice` | `DocumentSpec.h` uses old-style `enum`, not `enum class` |
| `PositiveDivisionsValue` vs `PositiveDivisions` vs `DivisionsValue` | redundant DecimalRange subclasses; one carries `/// TODO` |
| `EmptyPrintObjectStyleAlignAttributes` | only `*Attributes` placed in `core/` not `elements/`; criterion unclear |
| wrapper suffix (`DistanceType` vs `DynamicsValue` vs `LineWidthType` vs `ModeValue`) | inconsistent `Type`/`Value` suffix with no derivable reason |
| `dynamics` choice-of-empties -> enum collapse | confirm the structural-collapse rule generalizes rather than being a one-off |
| `articulations`/`encoding`/`notehead-text`/`technical` `*Choice` | confirm streamName-of-parent vs the empty-streamName choice pattern |

## Consequences for the wider plan

- **The mechanism must classify, not just override.** EXC and FIX require opposite treatment.
  A mechanism that cannot express "this delta is a FIX, score it as correct" forces an agent
  to either cheat (preserve the bug) or absorb an unfair `CHANGE_PENALTY`.
- **`scoring.md` and the systematic-gap design must give faithfulness precedence over diff
  size.** The missing-`color` case is concrete proof: ~45 structs of correct `.h` additions
  (5x multiplier) plus `.cpp` would lose under the rubric as written, which would reward
  reproducing a legacy bug. This finding belongs in those two grill sessions.
- **An EXC's justification must record its frozen 3.x schema assumption** (see the EXC table's
  last column). That column is the 4.0 reviewer's checklist.

## Method and scope

Derivation probes live in `gen/` (Python, stdlib `xml.etree`, chosen for
zero-dependency iteration in this investigation; the production generator language is a
separate Phase 1 question and is not decided here). Each probe derives one slice from the XSD
and diffs against `mx/core`; the measured counts above are reproducible by re-running them.

All slices have been probed: enum names, enum members, attributes structs, element content
models, numeric scalars, choice classes, group classes, the bespoke `core/` scalars, and
document/conversion infrastructure. After `FIX` emerged no further category appeared, and the
later slices produced only further instances of existing categories, now enumerated in the
tables above (the 21 `*Choice` start-value split, the sole `LayoutGroup` group anomaly, the 10
bespoke scalars, the infrastructure SUBSTRATE set). The catalog is both category-complete and
instance-complete for the override-mechanism decision; the ANOMALY rows marked "confirm" are
deliberately left as verification tasks for the Phase 1 implementation, not open design
questions.
