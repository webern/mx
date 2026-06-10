"""Project the IR onto one target: build the Plates.

The build consumes the IR and its Resolver (it never re-derives a schema
fact) plus a Config, and produces the materialized Plates tree. Three phases,
each failing loud:

  1. Rename validation: every [rename.*] key must name something the IR
     actually contains (a stale or misspelled key is a build error).
  2. Projection: names are tokenized and recased, renames and overrides
     applied, identifiers sanitized, types mapped, strategies and files
     assigned.
  3. Collision detection (gen.plates.check): distinct wire names that
     collapsed to one identifier under the projection are reported together.
"""

from __future__ import annotations

from typing import TYPE_CHECKING

from gen.ir import model as ir
from gen.ir.resolve import Resolver
from gen.plates import languages
from gen.plates.check import run_checks
from gen.plates.model import (
    ComplexPlate,
    EnumPlate,
    FileSpec,
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
from gen.plates.names import DEFAULT_ACRONYMS, NameFactory, sanitize_identifier

if TYPE_CHECKING:
    # Imported for annotations only: gen.config imports the convention
    # registry from gen.plates.names, so a runtime import here would cycle.
    from gen.config import Config


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
            naming.acronyms if naming.acronyms is not None else DEFAULT_ACRONYMS,
            naming.empty_value_word,
        )
        self.reserved = frozenset(languages.reserved_for(config.target.language)) | set(
            config.reserved.words
        )
        self.invalid_prefix = config.reserved.invalid_prefix
        self.type_map = languages.type_map_for(config.target.language)
        self.type_map.update(config.types)

        # Every type's Name and final identifier, computed up front so any
        # reference can be resolved to its target spelling in one lookup.
        self.type_names: dict[str, Name] = {}
        self.type_idents: dict[str, str] = {}
        for type_wire in list(self.values_by_name) + list(self.complex_by_name):
            name = self._type_name(type_wire)
            self.type_names[type_wire] = name
            self.type_idents[type_wire] = self._sanitize(
                config.target.prefix + name.cased[naming.type_convention]
            )

    # ----- entry ------------------------------------------------------------ #

    def build(self) -> Plates:
        errors = self._validate_renames()
        if errors:
            raise PlatesError(errors)

        plates = Plates(
            source=self.m.source,
            target=self._target_info(),
            value_types=[self._value_plate(v) for v in self.m.value_types],
            complex_types=[self._complex_plate(c) for c in self.m.complex_types],
            roots=[self._plate_ref(ir.Ref(r.type, "complex")) for r in self.m.roots],
            type_map=dict(self.type_map),
        )
        plates.files = self._assign_files(plates)
        return plates

    def _target_info(self) -> TargetInfo:
        t, n = self.cfg.target, self.cfg.naming
        doc_style = languages.doc_style_for(t.language)
        if self.cfg.docs.style is not None:
            doc_style.style = self.cfg.docs.style
        doc_style.wrap = self.cfg.docs.wrap
        return TargetInfo(
            language=t.language,
            namespace=t.namespace,
            prefix=t.prefix,
            type_convention=n.type_convention,
            field_convention=n.field_convention,
            variant_convention=n.variant_convention,
            file_convention=n.file_convention,
            inheritance=t.inheritance,
            doc_style=doc_style,
            reserved=sorted(self.reserved),
            partition=self.cfg.layout.partition,
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
        entry = self.cfg.renames.enum_values.get((scope_wire, value_wire))
        name = self.factory.make(
            value_wire,
            fundamental=entry.fundamental if entry else None,
            overrides=entry.cased if entry else None,
        )
        ident = self._sanitize(name.cased[self.cfg.naming.variant_convention])
        return Variant(wire=value_wire, name=name, ident=ident)

    def _field_ident(self, name: Name) -> str:
        raw = self.cfg.naming.field_prefix + name.cased[self.cfg.naming.field_convention]
        return self._sanitize(raw)

    def _plate_ref(self, ref: ir.Ref) -> PlateRef:
        if ref.category == "primitive":
            ident = self.type_map.get(ref.name, ref.name)
        else:
            ident = self.type_idents[ref.name]
        return PlateRef(wire=ref.name, category=ref.category, ident=ident)

    # ----- value plates -------------------------------------------------------- #

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
            )
        if isinstance(v, ir.NumberType):
            return NumberPlate(
                name=name,
                ident=ident,
                base=v.base,
                bounds=NumberBounds(
                    v.min_inclusive, v.max_inclusive, v.min_exclusive, v.max_exclusive
                ),
                target_type=self.type_map.get(v.base, v.base),
                doc=v.doc,
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
            )
        members = []
        for m in v.members:
            if m.ref is not None:
                members.append(UnionPlateMember(ref=self._plate_ref(m.ref)))
            else:
                # An inline literal set projects like a tiny anonymous enum;
                # its variants are addressable for renames under the union's
                # own type name.
                members.append(
                    UnionPlateMember(
                        literals=[self._variant(v.name, lit) for lit in m.literals or []]
                    )
                )
        return UnionPlate(name=name, ident=ident, members=members, doc=v.doc)

    # ----- complex plates ------------------------------------------------------ #

    def _complex_plate(self, ct: ir.ComplexType) -> ComplexPlate:
        strategy = {
            "value": "value-class",
            "composite": "composite-class",
            "empty": "flag" if ct.presence_only else "attrs-class",
            "derived": "inherit" if self.cfg.target.inheritance else "flatten",
        }[ct.kind]

        members = self._members(ct, self.resolver.attributes(ct))
        all_members = None
        if ct.kind == "derived":
            all_members = self._members(ct, self.resolver.all_attributes(ct), flatten=True)

        return ComplexPlate(
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
        )

    def _members(self, ct: ir.ComplexType, attrs: list[ir.Attr], flatten: bool = False) -> list[Member]:
        """The flat field list: attributes first, then the text value body,
        then child elements in document order (deduped by wire name). When
        flattening a derived type, the base chain's value body and elements
        are merged in (base-most first), mirroring all_attributes."""
        members = [self._attr_member(ct.name, a) for a in attrs]

        chain = [ct]
        if flatten:
            cur = ct
            while cur.base and cur.base in self.complex_by_name:
                cur = self.complex_by_name[cur.base]
                chain.append(cur)
            chain.reverse()  # base-most first

        for c in chain:
            if c.value_type is not None:
                members.append(self._value_member(c.value_type))
        seen: set[str] = set()
        for c in chain:
            for occ_name, element, cardinality in self._element_occurrences(c):
                if occ_name in seen:
                    continue
                seen.add(occ_name)
                members.append(self._element_member(element, cardinality))
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

    def _element_occurrences(self, ct: ir.ComplexType) -> list[tuple[str, ir.Element, str]]:
        """Walk the resolved content tree computing each element's effective
        cardinality for a flat member list: an element under a repeated
        sequence/choice is a vector; an element under a choice (or an optional
        wrapper) is at most optional; only an element required along a spine
        of exactly-once sequences stays required. Occurrences of the same
        name (e.g. the same element in two choice branches) are merged by the
        caller: vector beats optional beats required, first name wins order."""
        out: list[tuple[str, ir.Element, str]] = []
        merged: dict[str, int] = {}  # name -> index in out
        rank = {"required": 0, "optional": 1, "vector": 2}

        def walk(node, forced: bool, repeated: bool) -> None:
            if node is None:
                return
            if isinstance(node, ir.Element):
                if repeated:
                    card = "vector"
                elif node.card == "vector":
                    card = "vector"
                elif node.card == "required" and forced:
                    card = "required"
                else:
                    card = "optional"
                if node.name in merged:
                    i = merged[node.name]
                    prev_name, prev_el, prev_card = out[i]
                    # A second occurrence means alternative branches: the
                    # member cannot be statically required.
                    card = max(card, prev_card, key=lambda c: rank[c])
                    if card == "required":
                        card = "optional"
                    out[i] = (prev_name, prev_el, card)
                else:
                    merged[node.name] = len(out)
                    out.append((node.name, node, card))
                return
            once = node.min >= 1 and node.max == 1
            again = repeated or node.max == ir.UNBOUNDED or (node.max != 1)
            if isinstance(node, ir.Sequence):
                for item in node.items:
                    walk(item, forced and once, again)
            elif isinstance(node, ir.Choice):
                for item in node.items:
                    walk(item, False, again)
            # GroupRef leaves cannot appear: the Resolver spliced them.

        walk(self.resolver.content(ct), True, False)
        return out

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

    # ----- file assignment ------------------------------------------------------ #

    def _assign_files(self, plates: Plates) -> list[FileSpec] | None:
        partition = self.cfg.layout.partition
        if partition == "single":
            return None
        if partition == "grouped":
            raise PlatesError(
                ["[layout] partition = 'grouped' is not yet implemented; "
                 "use 'per-type' or 'single'"]
            )

        all_plates = list(plates.value_types) + list(plates.complex_types)
        stem_of = {
            p.name.wire: self.cfg.layout.file_prefix
            + p.name.cased[self.cfg.naming.file_convention]
            for p in all_plates
        }
        specs: list[FileSpec] = []
        for p in all_plates:
            p.file = stem_of[p.name.wire]
            deps = sorted(
                {stem_of[d] for d in self._type_deps(p) if d in stem_of} - {p.file}
            )
            specs.append(FileSpec(file=p.file, types=[p.name.wire], includes=deps))
        return specs

    def _type_deps(self, plate) -> set[str]:
        """Wire names of the types a plate's emitted code references: member
        and value types, union members, and the base edge. Primitives are not
        plates and are excluded by the caller's stem lookup."""
        deps: set[str] = set()
        if isinstance(plate, UnionPlate):
            deps.update(m.ref.wire for m in plate.members if m.ref is not None)
        if isinstance(plate, ComplexPlate):
            for member_list in (plate.members, plate.all_members or []):
                deps.update(m.type_ref.wire for m in member_list)
            if plate.base is not None:
                deps.add(plate.base.wire)
        return deps

    # ----- rename validation ------------------------------------------------------ #

    def _validate_renames(self) -> list[str]:
        """Every rename key must address something in the IR (design 6.5):
        a typo or a key left stale after a schema bump is a build error, not
        a silently ignored line."""
        r = self.cfg.renames
        errors: list[str] = []

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
                literals = {lit for m in vt.members for lit in (m.literals or [])}
                if value not in literals:
                    errors.append(
                        f"rename.enum-value.{enum}.{value!r}: union has no such literal"
                    )
            else:
                errors.append(f"rename.enum-value.{enum}: no such enum type")

        return errors
