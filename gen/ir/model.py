"""The intermediate representation: a resolved, language-agnostic model.

The raw XSD model (gen.xsd.model) mirrors the schema 1:1 and still speaks in
XSD terms (restriction chains, attribute-group refs, anonymous inline types).
The IR is what the language emitters consume instead. It is a pure function of
the XSD with every cross-reference resolved:

  - all types are named (anonymous types are hoisted, with context-qualified
    names for the partwise/timewise scaffolding);
  - simple-type restriction chains are collapsed to one primitive plus merged
    facets;
  - element occurrence is normalized to a cardinality (required/optional/vector);
  - dead types are dropped and complex types are emitted in dependency order.

The IR deliberately *preserves* named structure (aliases keep their names, the
five inheritance edges stay as derivations, model groups and attribute groups
remain addressable) so emitters can choose how much to collapse. See the
analyze module's recommendations for the reasoning.
"""

from __future__ import annotations

from dataclasses import dataclass, field

# Canonical maxOccurs="unbounded" marker in normalized particles. Kept as a
# string so it is self-describing in the JSON dump.
UNBOUNDED = "unbounded"


# --------------------------------------------------------------------------- #
# Type references
# --------------------------------------------------------------------------- #


@dataclass
class Ref:
    """A reference to another type by name, tagged with where to resolve it."""

    name: str
    category: str  # "complex" | "value" | "primitive"


# --------------------------------------------------------------------------- #
# Value types (lowered from simpleType and simpleContent bases)
# --------------------------------------------------------------------------- #


@dataclass
class EnumType:
    name: str
    base: str  # primitive the tokens are drawn from (token/string)
    values: list[str]
    doc: str | None = None
    kind: str = "enum"


@dataclass
class NumberType:
    name: str
    base: str  # decimal/integer/positive_integer/non_negative_integer
    min_inclusive: str | None = None
    max_inclusive: str | None = None
    min_exclusive: str | None = None
    max_exclusive: str | None = None
    doc: str | None = None
    kind: str = "number"


@dataclass
class StringType:
    name: str
    base: str  # string/token/nmtoken/date; a plain alias has no constraints
    patterns: list[str] = field(default_factory=list)
    min_length: str | None = None
    max_length: str | None = None
    length: str | None = None
    doc: str | None = None
    kind: str = "string"


@dataclass
class UnionMember:
    # Exactly one is set: a Ref to another type, or inline enumeration literals.
    ref: Ref | None = None
    literals: list[str] | None = None


@dataclass
class UnionType:
    name: str
    members: list[UnionMember]
    doc: str | None = None
    kind: str = "union"


ValueType = EnumType | NumberType | StringType | UnionType


# --------------------------------------------------------------------------- #
# Attributes
# --------------------------------------------------------------------------- #


@dataclass
class Attr:
    name: str
    type: Ref
    required: bool = False
    default: str | None = None
    fixed: str | None = None
    doc: str | None = None


@dataclass
class AttributeGroup:
    name: str
    attributes: list[Attr] = field(default_factory=list)
    attribute_groups: list[str] = field(default_factory=list)  # nested refs
    doc: str | None = None


# --------------------------------------------------------------------------- #
# Content model (normalized particles)
# --------------------------------------------------------------------------- #


@dataclass
class Element:
    name: str
    type: Ref
    card: str  # "required" | "optional" | "vector"
    min: int = 1
    max: int | str = 1  # int or UNBOUNDED
    doc: str | None = None
    node: str = "element"


@dataclass
class GroupRef:
    name: str
    min: int = 1
    max: int | str = 1
    node: str = "group"


@dataclass
class Sequence:
    items: list
    min: int = 1
    max: int | str = 1
    node: str = "sequence"


@dataclass
class Choice:
    items: list
    min: int = 1
    max: int | str = 1
    node: str = "choice"


Particle = Element | GroupRef | Sequence | Choice


@dataclass
class Group:
    name: str
    content: Particle
    doc: str | None = None


# --------------------------------------------------------------------------- #
# Complex types
# --------------------------------------------------------------------------- #


@dataclass
class ComplexType:
    name: str
    kind: str  # "value" | "composite" | "derived" | "empty"
    attributes: list[Attr] = field(default_factory=list)
    attribute_groups: list[str] = field(default_factory=list)
    value_type: Ref | None = None  # kind == "value" (text content type)
    base: str | None = None  # kind == "derived" (parent complex type)
    content: Particle | None = None  # composite/derived particle
    presence_only: bool = False  # empty element used as a boolean flag
    deps: list[str] = field(default_factory=list)  # complex types referenced
    doc: str | None = None


# --------------------------------------------------------------------------- #
# Schema root
# --------------------------------------------------------------------------- #


@dataclass
class Root:
    element: str
    type: str


@dataclass
class Ir:
    source: str
    builtins: dict[str, str]
    value_types: list[ValueType]
    groups: list[Group]
    attribute_groups: list[AttributeGroup]
    complex_types: list[ComplexType]  # dependency-ordered
    roots: list[Root]
    dropped_dead: list[str]
    stats: dict
