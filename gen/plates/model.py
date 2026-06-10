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


@dataclass
class Name:
    """The neutral/bound name bundle. `wire` is the immutable on-the-wire
    string (never a code identifier); `words` is the tokenized vector the
    casings expand from; `cased` maps convention name -> identifier, filled
    by iterating the convention registry (gen.plates.names.CONVENTIONS)."""

    wire: str
    words: tuple[str, ...]
    cased: dict[str, str]

    @property
    def pascal(self) -> str:
        return self.cased["pascal"]

    @property
    def camel(self) -> str:
        return self.cased["camel"]

    @property
    def snake(self) -> str:
        return self.cased["snake"]

    @property
    def kebab(self) -> str:
        return self.cased["kebab"]

    @property
    def screaming(self) -> str:
        return self.cased["screaming"]


@dataclass
class PlateRef:
    """A reference to another type, resolved for the target: `wire` and
    `category` mirror the IR Ref; `ident` is the spelling a template prints --
    the referenced plate's type identifier, or the mapped target type when the
    category is `primitive`."""

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
    doc_style: DocStyle
    reserved: list[str]  # language defaults + [reserved] words, sorted
    partition: str  # "per-type" | "single"


# --------------------------------------------------------------------------- #
# Value plates (mirror the IR's 4 value shapes)
# --------------------------------------------------------------------------- #


@dataclass
class Variant:
    """One enum value. `wire` is retained for serialization; `ident` is the
    sanitized identifier in the variant convention. Both are kept: a target
    whose enum constants are scoped by composition (Go `NoteTypeValue1024th`,
    C `MX_NOTE_TYPE_VALUE_1024TH`) composes from `name.cased`, while a target
    whose constants stand alone uses `ident` (where `1024th` -> `_1024th`)."""

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
    bounds: NumberBounds = field(default_factory=NumberBounds)
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
    """Exactly one is set: a resolved reference to a member type, or an inline
    literal set projected like a tiny anonymous enum (each literal carries its
    wire form and a variant identifier)."""

    ref: PlateRef | None = None
    literals: list[Variant] | None = None


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
    sequence/choice particle tree (groups spliced; IR node types) for a
    target that cares about order and choice structure.

    A derived plate exposes both the `base` edge (for a target with
    inheritance) and `all_members` (the base chain merged, for one without);
    `strategy` says which one this target uses."""

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
    type_map: dict[str, str] = field(default_factory=dict)

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
