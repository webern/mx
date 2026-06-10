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
emitter) reads only the neutral core and configures `[layout] partition =
"single"`, paying nothing for the binding it ignores.

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
    never appear on the wire themselves."""

    wire: str
    category: str  # "complex" | "value" | "primitive"
    ident: str


@dataclass
class DocStyle:
    """How the target writes doc comments. The plate keeps raw doc text (so a
    neutral target can use it verbatim); the template applies the syntax."""

    style: str  # e.g. "//", "///", "/* */"; "" for targets without comments
    wrap: int = 100


@dataclass
class TargetInfo:
    """The per-target facts that are global to the projection, not per-type."""

    language: str
    namespace: str  # C++ namespace / Go package; empty when prefix is used
    prefix: str  # global symbol prefix for languages without namespaces
    type_convention: str
    field_convention: str
    variant_convention: str
    file_convention: str
    inheritance: bool  # derived strategy: True -> inherit, False -> flatten
    variant_scope: str  # "bare" | "composed" (see Variant)
    doc_style: DocStyle
    reserved: list[str]  # language defaults + [reserved] words, sorted
    partition: str  # "per-type" | "single"
    file_prefix: str = ""  # [layout] file-prefix; backends name support files with it


# --------------------------------------------------------------------------- #
# Value plates (mirror the IR's 4 value shapes)
# --------------------------------------------------------------------------- #


@dataclass
class Variant:
    """One enum value. `wire` is retained for serialization; `ident` is the
    FINAL emitted constant identifier -- templates print it verbatim, and the
    collision gate certifies it. Its shape follows the target's variant scope
    (a language fact seeded in gen.plates.languages): `bare` for languages
    whose enum constants live inside the type (C++ `enum class` -> `_1024th`),
    `composed` for languages where they share one flat namespace (Go
    `NoteTypeValue1024th`, C `MX_NOTE_TYPE_VALUE_1024TH`)."""

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
    file: str | None = None
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
    file: str | None = None
    kind: str = "number"
    strategy: str = "numeric-wrapper"


@dataclass
class StringPlate:
    name: Name
    ident: str
    base: str  # IR primitive: string/token/nmtoken/date
    patterns: list[str] = field(default_factory=list)
    min_length: str | None = None
    max_length: str | None = None
    length: str | None = None
    target_type: str = ""  # type_map[base]
    doc: str | None = None
    file: str | None = None
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
    doc: str | None = None
    file: str | None = None
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
    base: PlateRef | None = None
    all_members: list[Member] | None = None
    presence_only: bool = False
    doc: str | None = None
    file: str | None = None
    kind: str = "complex"

    def member(self, wire: str, kind: str | None = None) -> Member:
        """The member a content occurrence or attribute wire name populates.
        `kind` disambiguates the rare wire name carried by both an attribute
        and an element (e.g. barline's segno)."""
        for m in self.members:
            if m.name.wire == wire and (kind is None or m.kind == kind):
                return m
        raise KeyError(f"{self.name.wire}: no member {wire!r} (kind={kind})")


# --------------------------------------------------------------------------- #
# The whole projected target
# --------------------------------------------------------------------------- #


@dataclass
class FileSpec:
    """One output file when partitioning: its stem (extension is the
    template's business), the wire names of the types assigned to it (in emit
    order), and the stems it must include/import (dependencies' files,
    deduped, self-excluded, sorted)."""

    file: str
    types: list[str] = field(default_factory=list)
    includes: list[str] = field(default_factory=list)


@dataclass
class Plates:
    """The complete projection of one target: every plate, in the IR's
    deps-first order (value types never reference complex types, so
    `value_types + complex_types` is a valid total emit order)."""

    source: str  # provenance: the XSD stem the IR was lowered from
    target: TargetInfo
    value_types: list[ValuePlate] = field(default_factory=list)
    complex_types: list[ComplexPlate] = field(default_factory=list)
    roots: list[PlateRef] = field(default_factory=list)
    files: list[FileSpec] | None = None  # None when partition == "single"

    def __post_init__(self):
        # Random-access index for templates; a plain attribute (not a
        # dataclass field) so JSON dumps stay free of the duplication.
        self._index = {p.name.wire: p for p in self.value_types}
        self._index.update({p.name.wire: p for p in self.complex_types})

    def plate(self, wire: str) -> ValuePlate | ComplexPlate:
        """Look up any plate by its wire type name."""
        return self._index[wire]

    def has_plate(self, wire: str) -> bool:
        return wire in self._index
