"""In-memory model of the subset of W3C XML Schema that MusicXML uses.

This is deliberately not a general XSD object model. It captures only the
constructs that actually appear in the MusicXML 4.0 schema (see the analyze
module for the inventory). Anything outside that subset -- xs:all, xs:any,
substitution groups, abstract types, mixed content -- is absent because the
schema does not use it.
"""

from __future__ import annotations

from dataclasses import dataclass, field

# Sentinel for an unbounded maxOccurs. We keep occurrence bounds as ints so the
# rest of the generator can do arithmetic on them; unbounded is the one value
# that cannot be an int.
UNBOUNDED = -1


# --------------------------------------------------------------------------- #
# Simple types
# --------------------------------------------------------------------------- #


@dataclass
class Enumeration:
    value: str
    doc: str | None = None


@dataclass
class Facets:
    """Constraining facets attached to a restriction."""

    enumerations: list[Enumeration] = field(default_factory=list)
    patterns: list[str] = field(default_factory=list)
    min_inclusive: str | None = None
    max_inclusive: str | None = None
    min_exclusive: str | None = None
    max_exclusive: str | None = None
    min_length: str | None = None
    max_length: str | None = None
    length: str | None = None


@dataclass
class Restriction:
    base: str
    facets: Facets


@dataclass
class Union:
    # Named or builtin members listed in the memberTypes attribute.
    member_types: list[str]
    # Anonymous simpleType members nested inside the union.
    inline_members: list[SimpleType]


@dataclass
class ListType:
    item_type: str


@dataclass
class SimpleType:
    name: str | None  # None for anonymous (inline) simple types
    doc: str | None
    content: Restriction | Union | ListType


# --------------------------------------------------------------------------- #
# Attributes
# --------------------------------------------------------------------------- #


@dataclass
class Attribute:
    """An xs:attribute, either a local declaration or a ref to an external one.

    A local declaration has a name and a type (or an inline simple type). A ref
    (xml:lang, xlink:href, ...) carries only the qualified ref name; its type
    lives in an imported schema.
    """

    name: str | None
    ref: str | None
    type: str | None
    inline_type: SimpleType | None
    use: str  # "optional" | "required" | "prohibited"
    default: str | None
    fixed: str | None
    doc: str | None


@dataclass
class AttributeGroupRef:
    ref: str


@dataclass
class AttributeGroup:
    name: str
    doc: str | None
    attributes: list[Attribute]
    group_refs: list[AttributeGroupRef]


# --------------------------------------------------------------------------- #
# Particles (element content models)
# --------------------------------------------------------------------------- #


@dataclass
class ElementParticle:
    """A local element declaration appearing inside a content model.

    MusicXML uses no global element refs, so every element is declared in place
    with a name and either a named type or an anonymous inline type.
    """

    name: str
    type: str | None
    inline_type: ComplexType | SimpleType | None
    min_occurs: int
    max_occurs: int  # UNBOUNDED for "unbounded"
    doc: str | None


@dataclass
class GroupRef:
    ref: str
    min_occurs: int
    max_occurs: int


@dataclass
class Sequence:
    items: list[Particle]
    min_occurs: int
    max_occurs: int


@dataclass
class Choice:
    items: list[Particle]
    min_occurs: int
    max_occurs: int


Particle = ElementParticle | GroupRef | Sequence | Choice


@dataclass
class Group:
    name: str
    doc: str | None
    particle: Particle


# --------------------------------------------------------------------------- #
# Complex types
# --------------------------------------------------------------------------- #


@dataclass
class SimpleContent:
    """A complex type whose body is text plus attributes (xs:simpleContent)."""

    base: str
    derivation: str  # "extension" | "restriction"
    attributes: list[Attribute]
    attribute_group_refs: list[AttributeGroupRef]


@dataclass
class ComplexContent:
    """A complex type derived from another complex type (xs:complexContent)."""

    base: str
    derivation: str
    particle: Particle | None
    attributes: list[Attribute]
    attribute_group_refs: list[AttributeGroupRef]


@dataclass
class ImplicitContent:
    """The shorthand form: a particle and/or attributes directly under
    xs:complexType, with no simpleContent/complexContent wrapper."""

    particle: Particle | None
    attributes: list[Attribute]
    attribute_group_refs: list[AttributeGroupRef]


ComplexContentModel = SimpleContent | ComplexContent | ImplicitContent


@dataclass
class ComplexType:
    name: str | None  # None for anonymous (inline) complex types
    doc: str | None
    content: ComplexContentModel


# --------------------------------------------------------------------------- #
# Schema root
# --------------------------------------------------------------------------- #


@dataclass
class TopLevelElement:
    """A document root element (score-partwise, score-timewise)."""

    name: str
    type: str | None
    inline_type: ComplexType | None
    doc: str | None


@dataclass
class Schema:
    simple_types: dict[str, SimpleType]
    complex_types: dict[str, ComplexType]
    groups: dict[str, Group]
    attribute_groups: dict[str, AttributeGroup]
    elements: list[TopLevelElement]
    imports: list[tuple[str, str]]  # (namespace, schemaLocation)
