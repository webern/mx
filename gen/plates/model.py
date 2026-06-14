"""The Plates: the template-facing, per-target projection of the IR.

The IR (gen.ir) is a pure, language-agnostic function of the schema inputs.
The Plates are its opposite number: one plate per emitted type, carrying
everything a template needs to print code without thinking -- identifier
casings, resolved target types, emit strategy tags, file assignment. This is
where config.toml meets the IR; templates stay dumb renderers.

Each plate is internally partitioned into two field groups:

  - a neutral core: wire-faithful, target-independent facts (wire name, shape,
    resolved structure, value lists, facets, docs), mirrored from the IR and
    its Resolver; and
  - a target binding: the per-target overlay (casings, sanitized identifiers,
    resolved target types, strategy tags, file assignment).

A code target reads both groups. A neutral target (e.g. a JSON Schema
emitter) reads only the neutral core and renders once-per-target templates,
paying nothing for the binding it ignores.

The Plates are materialized (built once per target, dumpable via
gen.ir.dump.to_jsonable) rather than computed on demand: collision detection
and rename validation are global build-then-check passes, and templates want
random access to fully resolved plates. Design: docs/ai/design/plates.md.
"""

from __future__ import annotations

from dataclasses import dataclass, field

from gen.ir import model as ir
from gen.names import Name

__all__ = ["Name"]  # re-exported: templates reach all plate vocabulary here


@dataclass
class PlateRef:
    """A reference to another type, resolved for the target: `wire` and
    `category` mirror the IR Ref; `ident` is the spelling a template prints --
    the referenced plate's type identifier, or the mapped target type when the
    category is `primitive`. For primitives, `wire` carries the IR's canonical
    primitive name (e.g. `non_negative_integer`), not an XSD spelling: builtins
    never appear on the wire themselves.

    `name` and `kind` are denormalized from the referenced plate so templates
    never perform lookups: `name` is the referenced type's name bundle (for a
    primitive, its tokenized canonical name), and `kind` is the referenced
    plate's kind (enum/number/string/union/complex) or, for primitives, the
    family-qualified `primitive-decimal` / `primitive-integer` /
    `primitive-string`."""

    wire: str
    category: str  # "complex" | "value" | "primitive"
    ident: str
    name: Name | None = None
    kind: str = ""
    # True when the referent is a name-token primitive (id/idref/nmtoken/...):
    # lexically narrower than a free string. A validating target renders a
    # repairing wrapper for it; others ignore the flag. Neutral schema fact.
    name_token: bool = False


@dataclass
class TargetInfo:
    """The per-target facts that are global to the projection, not per-type.
    Every field here is part of the projection contract: definable without
    reference to any language. Anything language-flavored belongs in `vars`,
    which passes through to templates verbatim and is never interpreted."""

    symbol_prefix: str  # prepended to type idents and composed constants
    type_convention: str
    field_convention: str
    variant_convention: str
    inheritance: bool  # derived strategy: True -> inherit, False -> flatten
    variant_scope: str  # "bare" | "composed" (see Variant)
    doc_wrap: int  # width doc text is wrapped to (doc_lines), excluding comment syntax
    reserved: list[str] = field(default_factory=list)  # the target's reserved words, sorted
    reserved_members: list[str] = field(default_factory=list)  # template-reserved member idents
    reserved_type_suffixes: list[str] = field(default_factory=list)  # template compositions
    vars: dict[str, str] = field(default_factory=dict)  # freeform, for templates


# --------------------------------------------------------------------------- #
# Value plates (mirror the IR's 4 value shapes)
# --------------------------------------------------------------------------- #


@dataclass
class Variant:
    """One enum value. `wire` is retained for serialization; `ident` is the
    FINAL emitted constant identifier -- templates print it verbatim, and the
    collision gate certifies it. Its shape follows the target's variant scope
    ([target] variant-scope): `bare` when the target's constants live inside
    the type (`_1024th`), `composed` when they share one flat namespace
    (`NoteTypeValue1024th`, `MX_NOTE_TYPE_VALUE_1024TH`)."""

    wire: str
    name: Name
    ident: str


@dataclass
class NumberBounds:
    """Numeric facets, verbatim from the schema (strings, not parsed)."""

    min_inclusive: str | None = None
    max_inclusive: str | None = None
    min_exclusive: str | None = None
    max_exclusive: str | None = None


@dataclass
class ClampStep:
    """One resolved clamping rule: `if v <op> <bound> then v = <replacement>`.
    This is the corpus leniency POLICY (the thing the .fixup.xml sidecars
    encode), decided once in the projection: facet bounds and the
    primitive-implied lower bounds are merged, the tightest wins, and an
    exclusive bound's replacement is the nearest representable in-range value
    (next integer, or bound +/- 1e-6 for decimals). The literals are spelled
    neutrally (valid in every current target language); templates print them
    verbatim."""

    op: str  # "<" | "<=" | ">" | ">="
    bound: str
    replacement: str


@dataclass
class EnumPlate:
    name: Name
    ident: str
    base: str  # IR primitive the tokens are drawn from
    variants: list[Variant]
    doc: str | None = None
    doc_lines: list[str] = field(default_factory=list)  # doc wrapped at doc_wrap
    deps: list[PlateRef] = field(default_factory=list)  # non-primitive types referenced
    kind: str = "enum"
    strategy: str = "enum-class"


@dataclass
class NumberPlate:
    name: Name
    ident: str
    base: str  # IR primitive: decimal/integer/positive_integer/non_negative_integer
    bounds: NumberBounds = field(default_factory=NumberBounds)  # neutral core: raw facets
    family: str = ""  # "decimal" | "integer": which parse/format family applies
    clamp: list[ClampStep] = field(default_factory=list)  # resolved policy (see ClampStep)
    target_type: str = ""  # type_map[base]: what the wrapper wraps
    doc: str | None = None
    doc_lines: list[str] = field(default_factory=list)
    deps: list[PlateRef] = field(default_factory=list)
    kind: str = "number"
    strategy: str = "numeric-wrapper"


@dataclass
class StringPlate:
    name: Name
    ident: str
    base: str  # IR primitive: string/token/nmtoken/date
    patterns: list[str] = field(default_factory=list)  # neutral core: raw XSD facets
    # The pattern facets as ONE anchored regex in the portable dialect
    # (literals, character classes, quantifiers, alternation, grouping --
    # parses identically in RE2, PCRE, ECMAScript, Python). XSD's implicit
    # whole-value anchoring is made explicit and its \i/\c name-class
    # escapes are expanded; see build.portable_pattern. None when the type
    # has no pattern facet. A target that enforces patterns compiles this;
    # one that does not simply never mentions it.
    pattern: str | None = None
    # The structured view of a literal-prefixed name-token pattern
    # (`coda\c*`, `(acc|medRenFla|...)\c+`): the literal prefix alternatives
    # projected like enum variants (renameable, collision-gated) plus
    # whether a non-empty suffix is required. Empty for any other pattern
    # shape; see build.prefix_facets. A structural target stores the suffix
    # and emits the prefix from the serializer.
    prefixes: list[Variant] = field(default_factory=list)
    multi_prefix: bool = False
    suffix_required: bool = False
    min_length: str | None = None
    max_length: str | None = None
    length: str | None = None
    target_type: str = ""  # type_map[base]
    doc: str | None = None
    doc_lines: list[str] = field(default_factory=list)
    deps: list[PlateRef] = field(default_factory=list)
    kind: str = "string"
    strategy: str = "string-wrapper"


@dataclass
class UnionPlateMember:
    """Exactly one of ref/literals is set: a resolved reference to a member
    type, or an inline literal set projected like a tiny anonymous enum (each
    literal carries its wire form and a variant identifier). A ref member also
    carries `name`, the referenced type's name bundle, so a template can spell
    the member's field without inventing a name (a primitive member like
    `positive_integer` has no plate to look it up on), and `tag`, the final
    discriminator-constant identifier for this member, scoped exactly like an
    enum variant and covered by the same collision gate. A primitive numeric
    member carries its `clamp` policy (the primitive-implied bounds), so the
    union enforces the same leniency as a named number type would."""

    ref: PlateRef | None = None
    name: Name | None = None
    tag: Variant | None = None
    literals: list[Variant] | None = None
    clamp: list[ClampStep] = field(default_factory=list)


@dataclass
class UnionPlate:
    name: Name
    ident: str
    members: list[UnionPlateMember] = field(default_factory=list)
    # True when a member accepts ANY string (a string-family member, last by
    # the union-order gate): in-order parsers never fall through.
    open_ended: bool = False
    doc: str | None = None
    doc_lines: list[str] = field(default_factory=list)
    deps: list[PlateRef] = field(default_factory=list)
    kind: str = "union"
    strategy: str = "tagged-variant"


ValuePlate = EnumPlate | NumberPlate | StringPlate | UnionPlate


# --------------------------------------------------------------------------- #
# Complex plates (mirror the IR's 4 complex shapes)
# --------------------------------------------------------------------------- #


@dataclass
class Member:
    """One field of a complex plate: an attribute, a child element, or the
    text value body of a `value`-shaped type. `cardinality` (required /
    optional / vector) plus the target's type map fully determine the concrete
    wrapper spelling (by-value, optional, collection); the template prints it.

    `default`/`fixed` keep the wire literal. When that literal names a variant
    of the member's enum type, `default_variant` carries the variant's target
    identifier so an emitter writes the enum member, not a raw string."""

    name: Name
    ident: str
    kind: str  # "attribute" | "element" | "value"
    type_ref: PlateRef
    cardinality: str  # "required" | "optional" | "vector"
    default: str | None = None
    fixed: str | None = None
    default_variant: str | None = None
    # The configured import repair for a required attribute MISSING from a
    # document ([import] attribute-defaults, plan §2.4): the parser injects
    # this wire literal; without one, missing is a parse error.
    import_default: str | None = None
    doc: str | None = None


@dataclass
class ComplexPlate:
    """One complex type, projected. `members` is the flat, deduped, ordered
    field list a code target emits (attributes, then the value body, then
    child elements in document order); `content` is the resolved
    sequence/choice particle tree for a target that cares about order and
    choice structure.

    `content` deliberately re-presents the IR's particle node types
    (Sequence/Choice/Element from gen.ir.model, groups already spliced): the
    neutral core IS the IR re-presented, and a parallel node hierarchy would
    only drift. Those node types are therefore part of this layer's public
    contract. A template joining a content occurrence back to the field it
    populates uses `member(wire, kind="element")` rather than re-walking.

    A derived plate exposes both the `base` edge (for a target with
    inheritance) and `all_members` (the base chain merged, for one without);
    `strategy` says which one this target uses. Both views are always
    populated for derived plates so the collision gate covers them under
    either strategy."""

    name: Name
    ident: str
    shape: str  # "value" | "composite" | "empty" | "derived"
    strategy: str  # value-class | composite-class | flag | attrs-class | inherit | flatten
    members: list[Member] = field(default_factory=list)
    content: ir.Particle | None = None
    # The grammar-preserving field projection of `content` (plan §2.9):
    # ordered fields a structural target renders instead of the flat
    # `members` element view. Empty for content-free shapes.
    fields: list[ContentField] = field(default_factory=list)
    # Every element name anywhere in the resolved content, for
    # misplaced-vs-unknown error classification in strict parsers.
    element_names: list[str] = field(default_factory=list)
    base: PlateRef | None = None
    all_members: list[Member] | None = None
    # True when some derived type extends this one (an inheriting target
    # cannot seal the class).
    is_base: bool = False
    presence_only: bool = False
    doc: str | None = None
    doc_lines: list[str] = field(default_factory=list)
    deps: list[PlateRef] = field(default_factory=list)
    # The dependency list of the grammar-preserving view: attribute/value
    # members plus content fields plus the base — what a structural target
    # imports instead of `deps` (which serves the flat member view).
    structural_deps: list[PlateRef] = field(default_factory=list)
    kind: str = "complex"

    def member(self, wire: str, kind: str | None = None) -> Member:
        """The member a content occurrence or attribute wire name populates.
        `kind` disambiguates the rare wire name carried by both an attribute
        and an element (e.g. barline's segno)."""
        for m in self.members:
            if m.name.wire == wire and (kind is None or m.kind == kind):
                return m
        raise KeyError(f"{self.name.wire}: no member {wire!r} (kind={kind})")

    def members_view(self) -> list[Member]:
        """The member list this plate's strategy renders: the merged
        base-chain view when flattening a derived type, own members
        otherwise. Backends render this; they never re-derive it."""
        if self.strategy == "flatten" and self.all_members is not None:
            return self.all_members
        return self.members


# --------------------------------------------------------------------------- #
# Content fields (the grammar-preserving projection, plan §2.9)
# --------------------------------------------------------------------------- #


@dataclass
class ContentField:
    """One ordered field of a grammar-preserving content projection: an
    element occurrence, a structural group reference, or a (possibly
    synthesized) choice. The cardinality plus the bounds fully determine the
    wrapper a structural target renders (by-value / optional / collection /
    capped collection); `first` is the element-name dispatch set a strict
    in-order parser tests the next element against.

    `presence` marks a field whose element type is presence-only (the only
    information is whether it appears): a flag in optional position, a
    marker value elsewhere. `min1` marks a repeated field that must never be
    empty (minOccurs >= 1); `max` carries a finite maxOccurs > 1 as a
    literal for bounded-append enforcement, None otherwise."""

    name: Name
    ident: str
    kind: str  # "element" | "group" | "choice"
    type_ref: PlateRef
    cardinality: str  # "required" | "optional" | "vector"
    tag: str | None = None  # the element wire name; None for group/choice
    presence: bool = False
    min1: bool = False
    max: str | None = None
    # True when the field's content can match the empty element sequence (a
    # required all-optional group, a nullable choice): a strict parser
    # leaves the default-constructed value instead of erroring.
    nullable: bool = False
    first: list[str] = field(default_factory=list)
    doc: str | None = None


@dataclass
class Alternative:
    """One alternative of a choice. `cardinality` is "required" (one
    instance) or "vector" (the alternative is itself a repeat, e.g.
    direction-type's `rehearsal+`); a vector alternative with `min1` False
    is nullable (the choice can be satisfied empty, e.g. key's
    non-traditional-key*). Alternatives are positional: a target whose sum
    type allows duplicate member types must dispatch by index, never by
    type (plan §2.3)."""

    name: Name
    ident: str
    kind: str  # "element" | "group" | "choice"
    type_ref: PlateRef
    cardinality: str  # "required" | "vector"
    tag: str | None = None
    presence: bool = False
    min1: bool = False
    max: str | None = None
    # True on the alternative a default-constructed choice value holds: the
    # first nullable alternative when the choice has one (the natural zero
    # of a nullable choice is "empty"), else the first alternative.
    default_alt: bool = False
    first: list[str] = field(default_factory=list)
    doc: str | None = None


@dataclass
class GroupPlate:
    """A named schema group (full-note, editorial, ...) or a synthesized
    anonymous sequence hoisted out of a content tree, projected as a shared
    type with ordered content fields. Groups are transparent on the wire
    (no enclosing element); `element_names` is every element name anywhere
    inside, for misplaced-vs-unknown error classification."""

    name: Name
    ident: str
    fields: list[ContentField] = field(default_factory=list)
    element_names: list[str] = field(default_factory=list)
    synthesized: bool = False
    doc: str | None = None
    doc_lines: list[str] = field(default_factory=list)
    deps: list[PlateRef] = field(default_factory=list)
    kind: str = "group"
    strategy: str = "group-class"


@dataclass
class ChoicePlate:
    """A choice projected as a shared sum type: a named schema group whose
    content is a single choice (music-data), or a synthesized anonymous
    choice hoisted out of a content tree. `nullable` is True when some
    alternative can match the empty element sequence (the choice can be
    satisfied without consuming anything)."""

    name: Name
    ident: str
    alternatives: list[Alternative] = field(default_factory=list)
    element_names: list[str] = field(default_factory=list)
    nullable: bool = False
    synthesized: bool = False
    doc: str | None = None
    doc_lines: list[str] = field(default_factory=list)
    deps: list[PlateRef] = field(default_factory=list)
    kind: str = "choice"
    strategy: str = "choice-class"


GroupLikePlate = GroupPlate | ChoicePlate


# --------------------------------------------------------------------------- #
# The whole projected target
# --------------------------------------------------------------------------- #


def attribute_members(members: list[Member]) -> list[Member]:
    """The shape queries backends partition a member list with. They live
    here, beside the data, so every backend asks the same question the same
    way instead of filtering inline."""
    return [m for m in members if m.kind == "attribute"]


def element_members(members: list[Member]) -> list[Member]:
    return [m for m in members if m.kind == "element"]


def value_member(members: list[Member]) -> Member | None:
    return next((m for m in members if m.kind == "value"), None)


@dataclass
class Plates:
    """The complete projection of one target: every plate, in the IR's
    deps-first order (value types never reference complex types, so
    `value_types + complex_types` is a valid total emit order)."""

    source: str  # provenance: the XSD stem the IR was lowered from
    target: TargetInfo
    schema_version: str = ""  # the MusicXML version in the source stem ("3.1")
    value_types: list[ValuePlate] = field(default_factory=list)
    complex_types: list[ComplexPlate] = field(default_factory=list)
    # The shared content types of the grammar-preserving projection (plan
    # §2.9): structural named schema groups plus the synthesized types
    # hoisted out of anonymous nested choices/sequences. A target consuming
    # only the flat member view never renders these.
    groups: list[GroupLikePlate] = field(default_factory=list)
    roots: list[PlateRef] = field(default_factory=list)

    def __post_init__(self):
        # Random-access index for templates; a plain attribute (not a
        # dataclass field) so JSON dumps stay free of the duplication.
        self._index = {p.name.wire: p for p in self.value_types}
        self._index.update({p.name.wire: p for p in self.complex_types})
        self._index.update({p.name.wire: p for p in self.groups})

    def plate(self, wire: str) -> ValuePlate | ComplexPlate:
        """Look up any plate by its wire type name."""
        return self._index[wire]

    def has_plate(self, wire: str) -> bool:
        return wire in self._index

    def children_owner(self, plate: ComplexPlate) -> ComplexPlate | None:
        """For an inheriting target: the base-chain plate whose child struct
        holds this type's children -- the nearest ancestor (or self) with
        element members. Schema reasoning, so it lives here, not in a
        template."""
        cur: ComplexPlate | None = plate
        while cur is not None:
            if element_members(cur.members):
                return cur
            cur = self.plate(cur.base.wire) if cur.base is not None else None
        return None
