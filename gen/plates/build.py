"""Project the IR onto one target: build the Plates.

The build consumes the IR and its Resolver (it never re-derives a schema
fact: splicing, base-chain merging, and effective cardinality all come from
gen.ir.resolve) plus a Config, and produces the materialized Plates tree.
Three phases, each failing loud:

  1. Config-against-IR validation: every [rename.*] key must name something
     the IR actually contains, and every [types] key a real primitive (a
     stale or misspelled key is a build error).
  2. Projection: names are tokenized and recased, renames and overrides
     applied, identifiers composed per the target's scoping and sanitized,
     types mapped, strategies and files assigned.
  3. Collision detection (gen.plates.check): distinct wire names that
     collapsed to one identifier under the projection are reported together.
"""

from __future__ import annotations

import re

from gen.config import Config
from gen.ir import model as ir
from gen.ir.build import PRIMITIVES
from gen.ir.resolve import Resolver
from gen.names import DEFAULT_ACRONYMS, JOINERS, NameFactory, sanitize_identifier
from gen.plates.check import run_checks
from gen.plates.model import (
    ClampStep,
    ComplexPlate,
    EnumPlate,
    Member,
    Name,
    NumberBounds,
    NumberPlate,
    PlateRef,
    Plates,
    StringPlate,
    TargetInfo,
    UnionPlate,
    UnionPlateMember,
    Variant,
)


# Primitive-implied lower bounds the schema leaves unstated; part of the
# uniform clamp policy (see model.ClampStep and data/README.md).
_IMPLIED_MIN = {"positive_integer": 1, "non_negative_integer": 0}

# The epsilon an exclusive DECIMAL bound clamps past (an exclusive integer
# bound clamps to the next integer). Matches the corpus duration fixup.
_EPSILON = 1e-6

# The numeric IR primitives ((see gen.ir.build.PRIMITIVES for the full set).
_PRIM_NUMERIC = {"decimal", "integer", "positive_integer", "non_negative_integer"}


def wrap_doc(doc: str | None, width: int) -> list[str]:
    """Greedy word-wrap of raw doc text at `width` (the wrapped TEXT width;
    templates add their own comment syntax). The break points reproduce the
    house comment style: a 3-character prefix plus width 97 is column 100."""
    if not doc:
        return []
    words = doc.split()
    lines: list[str] = []
    current = ""
    for word in words:
        if current and len(current) + 1 + len(word) > width:
            lines.append(current)
            current = word
        else:
            current = f"{current} {word}" if current else word
    if current:
        lines.append(current)
    return lines


def _dep_refs(refs) -> list:
    """The unique non-primitive references a plate's emitted code depends on,
    sorted by wire name -- the data templates compose include/import lines
    from. Primitive refs are excluded by CATEGORY (a primitive's name can
    coincide with a type's wire name)."""
    unique = {}
    for ref in refs:
        if ref.category != "primitive":
            unique.setdefault(ref.wire, ref)
    return [unique[wire] for wire in sorted(unique)]


def _number_family(base: str) -> str:
    return "decimal" if base == "decimal" else "integer"


def _spell(value: float, family: str) -> str:
    """A numeric literal valid in every current target language."""
    if family == "integer":
        return str(int(value))
    return repr(float(value))


def clamp_steps(base: str, bounds: NumberBounds) -> list[ClampStep]:
    """Resolve facets plus primitive-implied bounds into the ordered clamp
    rules a wrapper applies after parsing. The tightest lower bound wins (an
    exclusive bound at v is tighter than an inclusive one at the same v)."""
    family = _number_family(base)
    steps: list[ClampStep] = []

    lows: list[tuple[float, bool]] = []  # (value, exclusive)
    if bounds.min_inclusive is not None:
        lows.append((float(bounds.min_inclusive), False))
    if bounds.min_exclusive is not None:
        lows.append((float(bounds.min_exclusive), True))
    if base in _IMPLIED_MIN:
        lows.append((float(_IMPLIED_MIN[base]), False))
    if lows:
        value, exclusive = max(lows)
        if exclusive:
            past = value + (1 if family == "integer" else _EPSILON)
            steps.append(ClampStep("<=", _spell(value, family), _spell(past, family)))
        else:
            bound = _spell(value, family)
            steps.append(ClampStep("<", bound, bound))

    highs: list[tuple[float, bool]] = []
    if bounds.max_inclusive is not None:
        highs.append((float(bounds.max_inclusive), False))
    if bounds.max_exclusive is not None:
        highs.append((float(bounds.max_exclusive), True))
    if highs:
        value, exclusive = min((v, not e) for v, e in highs)
        exclusive = not exclusive
        if exclusive:
            past = value - (1 if family == "integer" else _EPSILON)
            steps.append(ClampStep(">=", _spell(value, family), _spell(past, family)))
        else:
            bound = _spell(value, family)
            steps.append(ClampStep(">", bound, bound))
    return steps


class PlatesError(Exception):
    """One or more projection failures, collected so a run reports every
    problem at once rather than the first."""

    def __init__(self, errors: list[str]):
        self.errors = errors
        super().__init__("\n".join(errors))


def build_plates(m: ir.Ir, config: Config) -> Plates:
    plates = _Builder(m, config).build()
    errors = run_checks(plates)
    if errors:
        raise PlatesError(errors)
    return plates


class _Builder:
    def __init__(self, m: ir.Ir, config: Config):
        self.m = m
        self.cfg = config
        self.resolver = Resolver.from_ir(m)
        self.values_by_name: dict[str, ir.ValueType] = {v.name: v for v in m.value_types}
        self.complex_by_name = {c.name: c for c in m.complex_types}

        naming = config.naming
        self.factory = NameFactory(
            naming.acronyms if naming.acronyms is not None else DEFAULT_ACRONYMS
        )
        # All of this is config data: the generator has no per-language
        # defaults (the cardinal rule -- see generator-agnosticism.md).
        self.reserved = frozenset(config.reserved.words)
        self.invalid_prefix = config.reserved.invalid_prefix
        self.type_map = dict(config.types)
        self.variant_scope = config.target.variant_scope

        # Every type's Name and final identifier, computed up front so any
        # reference can be resolved to its target spelling in one lookup.
        self.type_names: dict[str, Name] = {}
        self.type_idents: dict[str, str] = {}
        for type_wire in list(self.values_by_name) + list(self.complex_by_name):
            name = self._type_name(type_wire)
            self.type_names[type_wire] = name
            self.type_idents[type_wire] = self._sanitize(
                config.target.symbol_prefix + name.cased[naming.type_convention]
            )

    # ----- entry ------------------------------------------------------------ #

    def build(self) -> Plates:
        errors = self._validate_config_against_ir()
        if errors:
            raise PlatesError(errors)

        version = re.search(r"musicxml-(\d+\.\d+)", self.m.source)
        plates = Plates(
            source=self.m.source,
            schema_version=version.group(1) if version else "",
            target=self._target_info(),
            value_types=[self._value_plate(v) for v in self.m.value_types],
            complex_types=[self._complex_plate(c) for c in self.m.complex_types],
            roots=[self._plate_ref(ir.Ref(r.type, "complex")) for r in self.m.roots],
        )
        return plates

    def _target_info(self) -> TargetInfo:
        t, n = self.cfg.target, self.cfg.naming
        return TargetInfo(
            symbol_prefix=t.symbol_prefix,
            type_convention=n.type_convention,
            field_convention=n.field_convention,
            variant_convention=n.variant_convention,
            inheritance=t.inheritance,
            variant_scope=self.variant_scope,
            doc_wrap=self.cfg.docs.wrap,
            reserved=sorted(self.reserved),
            reserved_members=sorted(self.cfg.reserved.members),
            reserved_type_suffixes=sorted(self.cfg.reserved.type_suffixes),
            vars=dict(self.cfg.vars),
        )

    # ----- names and references ---------------------------------------------- #

    def _sanitize(self, raw: str) -> str:
        return sanitize_identifier(raw, self.reserved, self.invalid_prefix)

    def _type_name(self, wire: str) -> Name:
        entry = self.cfg.renames.types.get(wire)
        return self.factory.make(
            wire,
            fundamental=entry.fundamental if entry else None,
            overrides=entry.cased if entry else None,
        )

    def _element_name(self, wire: str, pluralize: bool) -> Name:
        entry = self.cfg.renames.elements.get(wire)
        return self.factory.make(
            wire,
            fundamental=entry.fundamental if entry else None,
            overrides=entry.cased if entry else None,
            pluralize=pluralize,
        )

    def _attribute_name(self, owner: str, wire: str) -> Name:
        # A scoped key (this attribute on this owner) wins over a global one.
        entry = self.cfg.renames.scoped_attributes.get((owner, wire))
        if entry is None:
            entry = self.cfg.renames.attributes.get(wire)
        return self.factory.make(
            wire,
            fundamental=entry.fundamental if entry else None,
            overrides=entry.cased if entry else None,
        )

    def _variant(self, scope_wire: str, value_wire: str) -> Variant:
        """Project one enum value (or union literal). The final constant
        identifier follows the target's variant scope: `bare` sanitizes the
        variant casing alone; `composed` joins the owning type's casing (and
        symbol prefix) in the variant convention's join style, because the
        constant will live in a flat namespace."""
        entry = self.cfg.renames.enum_values.get((scope_wire, value_wire))
        name = self.factory.make(
            value_wire,
            fundamental=entry.fundamental if entry else None,
            overrides=entry.cased if entry else None,
        )
        conv = self.cfg.naming.variant_convention
        if self.variant_scope == "composed":
            joiner = JOINERS.get(conv, "_")
            if joiner:
                parts = []
                if self.cfg.target.symbol_prefix:
                    prefix_name = self.factory.make(self.cfg.target.symbol_prefix)
                    parts.append(prefix_name.cased[conv])
                parts.append(self.type_names[scope_wire].cased[conv])
                parts.append(name.cased[conv])
                raw = joiner.join(parts)
            else:
                # Concatenating conventions: the type identifier (which
                # already carries the prefix) plus the variant casing.
                raw = self.type_idents[scope_wire] + name.cased[conv]
        else:
            raw = name.cased[conv]
        return Variant(wire=value_wire, name=name, ident=self._sanitize(raw))

    def _field_ident(self, name: Name) -> str:
        raw = self.cfg.naming.field_prefix + name.cased[self.cfg.naming.field_convention]
        return self._sanitize(raw)

    def _plate_ref(self, ref: ir.Ref) -> PlateRef:
        """Resolve a reference with the referenced type's name bundle and kind
        denormalized onto it, so templates never perform lookups."""
        if ref.category == "primitive":
            return PlateRef(
                wire=ref.name,
                category="primitive",
                ident=self.type_map.get(ref.name, ref.name),
                name=self.factory.make(ref.name),
                kind="primitive-" + _number_family(ref.name)
                if ref.name in _PRIM_NUMERIC
                else "primitive-string",
            )
        if ref.category == "value":
            kind = self.values_by_name[ref.name].kind
        else:
            kind = "complex"
        return PlateRef(
            wire=ref.name,
            category=ref.category,
            ident=self.type_idents[ref.name],
            name=self.type_names[ref.name],
            kind=kind,
        )

    # ----- value plates -------------------------------------------------------- #

    def _doc_lines(self, doc: str | None) -> list[str]:
        return wrap_doc(doc, self.cfg.docs.wrap)

    def _value_plate(self, v: ir.ValueType):
        name = self.type_names[v.name]
        ident = self.type_idents[v.name]
        if isinstance(v, ir.EnumType):
            return EnumPlate(
                name=name,
                ident=ident,
                base=v.base,
                variants=[self._variant(v.name, value) for value in v.values],
                doc=v.doc,
                doc_lines=self._doc_lines(v.doc),
            )
        if isinstance(v, ir.NumberType):
            bounds = NumberBounds(
                v.min_inclusive, v.max_inclusive, v.min_exclusive, v.max_exclusive
            )
            return NumberPlate(
                name=name,
                ident=ident,
                base=v.base,
                bounds=bounds,
                family=_number_family(v.base),
                clamp=clamp_steps(v.base, bounds),
                target_type=self.type_map.get(v.base, v.base),
                doc=v.doc,
                doc_lines=self._doc_lines(v.doc),
            )
        if isinstance(v, ir.StringType):
            return StringPlate(
                name=name,
                ident=ident,
                base=v.base,
                patterns=list(v.patterns),
                min_length=v.min_length,
                max_length=v.max_length,
                length=v.length,
                target_type=self.type_map.get(v.base, v.base),
                doc=v.doc,
                doc_lines=self._doc_lines(v.doc),
            )
        members = []
        for m in v.members:
            if m.ref is not None:
                member_name = self.type_names.get(m.ref.name) or self.factory.make(m.ref.name)
                clamp = []
                if m.ref.category == "primitive" and m.ref.name in _IMPLIED_MIN:
                    # The primitive's implied bounds apply inside a union just
                    # as they would on a named number type.
                    clamp = clamp_steps(m.ref.name, NumberBounds())
                members.append(
                    UnionPlateMember(
                        ref=self._plate_ref(m.ref),
                        name=member_name,
                        # The member's discriminator constant: scoped, renamed,
                        # and collision-gated exactly like an enum variant.
                        tag=self._variant(v.name, m.ref.name),
                        clamp=clamp,
                    )
                )
            else:
                # An inline literal set projects like a tiny anonymous enum;
                # its variants are addressable for renames under the union's
                # own type name and double as the discriminator constants.
                members.append(
                    UnionPlateMember(
                        literals=[self._variant(v.name, lit) for lit in m.literals or []]
                    )
                )
        plate = UnionPlate(
            name=name,
            ident=ident,
            members=members,
            open_ended=any(
                m.ref is not None and m.ref.kind in ("primitive-string", "string")
                for m in members
            ),
            doc=v.doc,
            doc_lines=self._doc_lines(v.doc),
        )
        plate.deps = _dep_refs(
            m.ref for m in plate.members if m.ref is not None
        )
        return plate

    # ----- complex plates ------------------------------------------------------ #

    def _complex_plate(self, ct: ir.ComplexType) -> ComplexPlate:
        strategy = {
            "value": "value-class",
            "composite": "composite-class",
            "empty": "flag" if ct.presence_only else "attrs-class",
            "derived": "inherit" if self.cfg.target.inheritance else "flatten",
        }[ct.kind]

        members = self._members(ct, flatten=False)
        all_members = None
        if ct.kind == "derived":
            # Built under either strategy, so the collision gate covers the
            # merged chain even for inheriting targets.
            all_members = self._members(ct, flatten=True)

        plate = ComplexPlate(
            name=self.type_names[ct.name],
            ident=self.type_idents[ct.name],
            shape=ct.kind,
            strategy=strategy,
            members=members,
            content=self.resolver.content(ct),
            base=self._plate_ref(ir.Ref(ct.base, "complex")) if ct.base else None,
            all_members=all_members,
            presence_only=ct.presence_only,
            doc=ct.doc,
            doc_lines=self._doc_lines(ct.doc),
        )
        refs = [m.type_ref for m in plate.members]
        refs += [m.type_ref for m in (plate.all_members or [])]
        if plate.base is not None:
            refs.append(plate.base)
        plate.deps = _dep_refs(refs)
        return plate

    def _members(self, ct: ir.ComplexType, flatten: bool) -> list[Member]:
        """The flat field list: attributes first, then the text value body,
        then child elements in document order. The flattened variant merges
        the base chain (base-most first) via the Resolver's chain views."""
        if flatten:
            attrs = self.resolver.all_attributes(ct)
            elements = self.resolver.all_flat_elements(ct)
            chain = self.resolver.base_chain(ct)
        else:
            attrs = self.resolver.attributes(ct)
            elements = self.resolver.flat_elements(ct)
            chain = [ct]

        members = [self._attr_member(ct.name, a) for a in attrs]
        for c in chain:
            if c.value_type is not None:
                members.append(self._value_member(c.value_type))
        members += [self._element_member(e, card) for e, card in elements]
        return members

    def _attr_member(self, owner_wire: str, a: ir.Attr) -> Member:
        name = self._attribute_name(owner_wire, a.name)
        literal = a.fixed if a.fixed is not None else a.default
        return Member(
            name=name,
            ident=self._field_ident(name),
            kind="attribute",
            type_ref=self._plate_ref(a.type),
            cardinality="required" if a.required else "optional",
            default=a.default,
            fixed=a.fixed,
            default_variant=self._default_variant(a.type, literal),
            doc=a.doc,
        )

    def _value_member(self, value_type: ir.Ref) -> Member:
        # The text body of a value-shaped type has no wire name of its own;
        # it is projected under the fixed root "value".
        name = self.factory.make("", fundamental="value")
        return Member(
            name=name,
            ident=self._field_ident(name),
            kind="value",
            type_ref=self._plate_ref(value_type),
            cardinality="required",
        )

    def _element_member(self, element: ir.Element, cardinality: str) -> Member:
        pluralize = self.cfg.naming.pluralize_vectors and cardinality == "vector"
        name = self._element_name(element.name, pluralize)
        return Member(
            name=name,
            ident=self._field_ident(name),
            kind="element",
            type_ref=self._plate_ref(element.type),
            cardinality=cardinality,
            doc=element.doc,
        )

    def _default_variant(self, type_ref: ir.Ref, literal: str | None) -> str | None:
        """When a default/fixed literal names a variant of the member's enum
        type, resolve it to the variant's target identifier (the wire literal
        stays in `default`/`fixed` for the serializer)."""
        if literal is None or type_ref.category != "value":
            return None
        vt = self.values_by_name.get(type_ref.name)
        if isinstance(vt, ir.EnumType) and literal in vt.values:
            return self._variant(vt.name, literal).ident
        return None

    # ----- config-against-IR validation ----------------------------------------- #

    def _validate_config_against_ir(self) -> list[str]:
        """Every rename key must address something in the IR, and every
        [types] key a real primitive (design 6.5): a typo or a key left stale
        after a schema bump is a build error, not a silently ignored line."""
        r = self.cfg.renames
        errors: list[str] = []

        for primitive in self.cfg.types:
            if primitive not in PRIMITIVES:
                errors.append(
                    f"[types] {primitive}: not an IR primitive "
                    f"({', '.join(sorted(PRIMITIVES))})"
                )

        type_wires = set(self.values_by_name) | set(self.complex_by_name)
        for wire in r.types:
            if wire not in type_wires:
                errors.append(f"rename.type.{wire}: no such type in the IR")

        element_wires: set[str] = set()
        for ct in self.m.complex_types:
            for e in self.resolver.elements(ct):
                element_wires.add(e.name)
        element_wires.update(root.element for root in self.m.roots)
        for wire in r.elements:
            if wire not in element_wires:
                errors.append(f"rename.element.{wire}: no element by that name occurs")

        attribute_wires = {
            a.name for ct in self.m.complex_types for a in self.resolver.attributes(ct)
        }
        for wire in r.attributes:
            if wire not in attribute_wires:
                errors.append(f"rename.attribute.{wire}: no attribute by that name occurs")

        for owner, attr in r.scoped_attributes:
            ct = self.complex_by_name.get(owner)
            if ct is None:
                errors.append(f"rename.attribute.{owner}.{attr}: no such complex type")
            elif attr not in {a.name for a in self.resolver.all_attributes(ct)}:
                errors.append(
                    f"rename.attribute.{owner}.{attr}: type '{owner}' has no such attribute"
                )

        for enum, value in r.enum_values:
            vt = self.values_by_name.get(enum)
            if isinstance(vt, ir.EnumType):
                if value not in vt.values:
                    errors.append(
                        f"rename.enum-value.{enum}.{value!r}: enum has no such value"
                    )
            elif isinstance(vt, ir.UnionType):
                addressable = {lit for m in vt.members for lit in (m.literals or [])}
                addressable |= {m.ref.name for m in vt.members if m.ref is not None}
                if value not in addressable:
                    errors.append(
                        f"rename.enum-value.{enum}.{value!r}: union has no such "
                        f"literal or member"
                    )
            else:
                errors.append(f"rename.enum-value.{enum}: no such enum type")

        return errors
