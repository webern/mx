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
    Alternative,
    ChoicePlate,
    ClampStep,
    ComplexPlate,
    ContentField,
    EnumPlate,
    GroupPlate,
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

# The name-token IR primitives: NCName/NMTOKEN-derived lexical types that are
# narrower than a free string (no whitespace, no empty value, and ID/IDREF
# disallow a leading digit). A neutral schema fact; a validating target
# repairs them, a non-validating one ignores the flag.
_PRIM_NAME_TOKEN = {"id", "idref", "idrefs", "nmtoken", "nmtokens"}

# The primitives a validating target maps to a wrapper class rather than a raw
# target string: the name tokens plus the language tag (a builtin lexical
# space, like them, but not a name).
_PRIM_WRAPPER = _PRIM_NAME_TOKEN | {"language"}


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


# ASCII subsets of the XML name-character classes XSD's \i and \c denote.
# The full classes add non-ASCII ranges whose spelling is engine-specific
# (\x{...} vs \uXXXX); every identifier vocabulary a MusicXML pattern
# describes (SMuFL canonical glyph names) is ASCII, and the strict parse is
# the only consumer, so the approximation can only under-accept.
_XSD_NAME_START = "[:A-Z_a-z]"
_XSD_NAME_CHAR = "[-.0-9:A-Z_a-z]"


def _translate_pattern(pattern: str) -> str:
    """One XSD pattern, re-spelled in the portable dialect. Constructs with
    no portable spelling (class subtraction, \\C/\\I complements, \\p
    properties) fail loud: a new schema construct is a decision, not a
    silent pass-through."""
    out: list[str] = []
    in_class = False
    i, n = 0, len(pattern)
    while i < n:
        ch = pattern[i]
        if ch == "\\":
            if i + 1 >= n:
                raise ValueError(f"trailing backslash in pattern {pattern!r}")
            esc = pattern[i + 1]
            if esc in "ci":
                if in_class:
                    raise ValueError(
                        f"\\{esc} inside a character class has no portable "
                        f"expansion: {pattern!r}"
                    )
                out.append(_XSD_NAME_CHAR if esc == "c" else _XSD_NAME_START)
            elif esc in "CIpP":
                raise ValueError(
                    f"\\{esc} has no portable spelling: {pattern!r}"
                )
            else:
                out.append(ch + esc)
            i += 2
            continue
        if in_class:
            if ch == "[":
                raise ValueError(
                    f"character class subtraction is not portable: {pattern!r}"
                )
            if ch == "]":
                in_class = False
        elif ch == "[":
            in_class = True
        elif ch in "^$":
            # XSD has no anchors; ^ and $ are ordinary characters there and
            # must be escaped to stay ordinary in the portable form.
            out.append("\\")
        out.append(ch)
        i += 1
    return "".join(out)


def prefix_facets(patterns: list[str]) -> tuple[list[str], bool] | None:
    """The structured view of a literal-prefixed name-token pattern: for a
    single facet of the shape `lit\\c*`, `lit\\c+`, or `(a|b|...)\\c+`,
    return (prefix literals, suffix required). None for any other shape.
    Neutral pattern analysis: the schema's SMuFL glyph-name vocabularies all
    have this form, and a structural target stores only the suffix."""
    if len(patterns) != 1:
        return None
    m = re.fullmatch(
        r"\(?([A-Za-z]+(?:\|[A-Za-z]+)*)\)?\(?\\c([*+])\)?", patterns[0]
    )
    if m is None:
        return None
    return m.group(1).split("|"), m.group(2) == "+"


def portable_pattern(patterns: list[str]) -> str | None:
    """The type's pattern facets as one anchored portable regex, or None.

    XSD patterns match the whole value (implicit anchoring), so the portable
    form is explicitly anchored. Multiple pattern facets on one restriction
    step are alternatives (XSD OR semantics); MusicXML never re-restricts an
    already-patterned type, so the IR's accumulated list is always a single
    step and the facets OR-join."""
    if not patterns:
        return None
    translated = [f"(?:{_translate_pattern(p)})" for p in patterns]
    if len(translated) == 1:
        return f"^{translated[0]}$"
    return "^(?:" + "|".join(translated) + ")$"


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
        self.groups_by_name = {g.name: g for g in m.groups}
        self._base_wires = {c.base for c in m.complex_types if c.base}

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

        # The grammar-preserving content projection (plan §2.9): normalize
        # each content tree once (the discovery and the build must see the
        # SAME particle objects), walk it to find every structural group the
        # schema actually references and every anonymous choice/sequence
        # that needs hoisting into a named shared type.
        self.field_nodes: dict[str, list] = {}  # owner wire -> field nodes
        self.synth_content: dict[str, ir.Particle] = {}  # synth wire -> node
        self.synth_kind: dict[str, str] = {}  # synth wire -> "choice"|"group"
        self.synth_by_id: dict[int, str] = {}  # id(particle) -> synth wire
        # Referenced structural groups, in first-reference order. A group's
        # PLATE wire is `{name}-group` / `{name}-choice`: schema group names
        # can collide with complex type names (clef, slash, transpose), so
        # the suffix keeps the type namespace collision-free by construction
        # (and reads as the legacy convention: FullNoteGroup, MusicDataChoice).
        self.struct_groups: list[str] = []
        self.group_wire: dict[str, str] = {}  # group name -> plate wire
        self.group_shape_of: dict[str, str] = {}  # plate wire -> "group"|"choice"
        self.group_choice_node: dict[str, ir.Choice] = {}  # plate wire -> node
        self._synth_counts: dict[tuple[str, str], int] = {}
        self._discover()

        # Every type's Name and final identifier, computed up front so any
        # reference can be resolved to its target spelling in one lookup.
        # Structural groups and synthesized content types share the type
        # namespace (and therefore the collision gate).
        self.type_names: dict[str, Name] = {}
        self.type_idents: dict[str, str] = {}
        all_type_wires = (
            list(self.values_by_name)
            + list(self.complex_by_name)
            + [self.group_wire[g] for g in self.struct_groups]
            + list(self.synth_content)
        )
        for type_wire in all_type_wires:
            name = self._type_name(type_wire)
            self.type_names[type_wire] = name
            self.type_idents[type_wire] = self._sanitize(
                config.target.symbol_prefix + name.cased[naming.type_convention]
            )

    # ----- content discovery (plan §2.9) -------------------------------------- #
    #
    # One deterministic preorder walk over every composite's normalized
    # content, recursing through structural group references (each group
    # walked once, at first reference). Anonymous choices get the wire name
    # `{owner}-choice` (`-2`, `-3`, ... on repeats within one owner);
    # anonymous sequences get `{owner}-group`. Nested hoists are owned by
    # the hoisted type, so the names compose (`note-choice-group`). All of
    # these wires are addressable by [rename.type].

    def _discover(self) -> None:
        for ct in self.m.complex_types:
            if ct.content is not None:
                self._walk_owner(ct.name, ct.content)

    def _walk_owner(self, owner: str, content: ir.Particle) -> None:
        nodes = self.resolver.field_nodes(content)
        self.field_nodes[owner] = nodes
        for node in nodes:
            self._visit(owner, node)

    def _visit(self, owner: str, node: ir.Particle) -> None:
        if isinstance(node, ir.Element):
            return
        if isinstance(node, ir.GroupRef):
            self._see_group(node.name)
            return
        if isinstance(node, ir.Choice):
            wire = self._hoist(owner, "choice", node)
            self._visit_choice_items(wire, node)
            return
        # An optional/repeated anonymous Sequence: hoist as a group whose
        # content is the exactly-once body.
        body = ir.Sequence(node.items, 1, 1)
        wire = self._hoist(owner, "group", body)
        self.synth_by_id[id(node)] = wire
        self._walk_owner(wire, body)

    def _visit_choice_items(self, choice_wire: str, choice: ir.Choice) -> None:
        for alt in choice.items:
            if isinstance(alt, ir.Element):
                continue
            if isinstance(alt, ir.GroupRef):
                self._see_group(alt.name)
            elif isinstance(alt, ir.Choice):
                wire = self._hoist(choice_wire, "choice", alt)
                self.synth_by_id[id(alt)] = wire
                self._visit_choice_items(wire, alt)
            else:  # Sequence alternative
                body = ir.Sequence(alt.items, 1, 1)
                wire = self._hoist(choice_wire, "group", body)
                self.synth_by_id[id(alt)] = wire
                self._walk_owner(wire, body)

    def _hoist(self, owner: str, kind: str, content: ir.Particle) -> str:
        n = self._synth_counts.get((owner, kind), 0) + 1
        self._synth_counts[(owner, kind)] = n
        wire = f"{owner}-{kind}" if n == 1 else f"{owner}-{kind}-{n}"
        self.synth_content[wire] = content
        self.synth_kind[wire] = kind
        self.synth_by_id[id(content)] = wire
        return wire

    def _see_group(self, name: str) -> None:
        if name in self.group_wire:
            return
        group = self.groups_by_name.get(name)
        if group is None:
            return
        shape, node = self.resolver.group_shape(name)
        wire = f"{name}-{shape}"
        self.group_wire[name] = wire
        self.group_shape_of[wire] = shape
        self.struct_groups.append(name)
        if shape == "choice":
            # The group IS the choice (reference sites absorb the choice's
            # occurrence); its alternatives may still need hoisting.
            self.group_choice_node[wire] = node
            self._visit_choice_items(wire, node)
        else:
            self._walk_owner(wire, group.content)

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
            groups=self._group_plates(),
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
                name_token=ref.name in _PRIM_NAME_TOKEN,
                wrapper=ref.name in _PRIM_WRAPPER,
                unique_id=ref.name == "id",
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
            facets = prefix_facets(list(v.patterns))
            prefixes, suffix_required = facets if facets else ([], False)
            return StringPlate(
                name=name,
                ident=ident,
                base=v.base,
                patterns=list(v.patterns),
                pattern=portable_pattern(list(v.patterns)),
                prefixes=[self._variant(v.name, p) for p in prefixes],
                multi_prefix=len(prefixes) > 1,
                suffix_required=suffix_required,
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

        fields: list[ContentField] = []
        element_names: list[str] = []
        if ct.content is not None:
            fields = [self._field(ct.name, n) for n in self.field_nodes[ct.name]]
            element_names = self.resolver.particle_element_names(ct.content)

        plate = ComplexPlate(
            name=self.type_names[ct.name],
            ident=self.type_idents[ct.name],
            shape=ct.kind,
            strategy=strategy,
            members=members,
            content=self.resolver.content(ct),
            fields=fields,
            element_names=element_names,
            base=self._plate_ref(ir.Ref(ct.base, "complex")) if ct.base else None,
            all_members=all_members,
            is_base=ct.name in self._base_wires,
            presence_only=ct.presence_only,
            doc=ct.doc,
            doc_lines=self._doc_lines(ct.doc),
        )
        refs = [m.type_ref for m in plate.members]
        refs += [m.type_ref for m in (plate.all_members or [])]
        if plate.base is not None:
            refs.append(plate.base)
        plate.deps = _dep_refs(refs)
        # What a grammar-preserving target depends on instead: attribute
        # (and value-body) members plus the content fields, not the flat
        # element view.
        srefs = [m.type_ref for m in plate.members if m.kind != "element"]
        srefs += [f.type_ref for f in plate.fields]
        if plate.base is not None:
            srefs.append(plate.base)
        plate.structural_deps = _dep_refs(srefs)
        return plate

    # ----- group, choice, and synthesized content plates (plan §2.9) --------- #

    def _group_plates(self) -> list:
        out: list = []
        for name in self.struct_groups:
            wire = self.group_wire[name]
            g = self.groups_by_name[name]
            if self.group_shape_of[wire] == "choice":
                out.append(
                    self._choice_plate(
                        wire, self.group_choice_node[wire], synthesized=False, doc=g.doc
                    )
                )
            else:
                out.append(
                    GroupPlate(
                        name=self.type_names[wire],
                        ident=self.type_idents[wire],
                        fields=[self._field(wire, n) for n in self.field_nodes[wire]],
                        element_names=self.resolver.particle_element_names(g.content),
                        synthesized=False,
                        doc=g.doc,
                        doc_lines=self._doc_lines(g.doc),
                    )
                )
        for wire, content in self.synth_content.items():
            if self.synth_kind[wire] == "choice":
                out.append(self._choice_plate(wire, content, synthesized=True, doc=None))
            else:
                out.append(
                    GroupPlate(
                        name=self.type_names[wire],
                        ident=self.type_idents[wire],
                        fields=[self._field(wire, n) for n in self.field_nodes[wire]],
                        element_names=self.resolver.particle_element_names(content),
                        synthesized=True,
                    )
                )
        for plate in out:
            refs = [f.type_ref for f in getattr(plate, "fields", [])]
            refs += [a.type_ref for a in getattr(plate, "alternatives", [])]
            plate.deps = _dep_refs(refs)
        return out

    def _choice_plate(self, wire, choice, synthesized: bool, doc) -> ChoicePlate:
        alternatives = [self._alternative(wire, alt) for alt in choice.items]
        # The natural zero of a nullable choice is its first nullable
        # alternative (empty); otherwise the first alternative.
        nullable_alts = [
            i
            for i, alt in enumerate(choice.items)
            if self.resolver.nullable(alt)
        ]
        alternatives[nullable_alts[0] if nullable_alts else 0].default_alt = True
        return ChoicePlate(
            name=self.type_names[wire],
            ident=self.type_idents[wire],
            alternatives=alternatives,
            element_names=self.resolver.particle_element_names(choice),
            nullable=bool(nullable_alts),
            synthesized=synthesized,
            doc=doc,
            doc_lines=self._doc_lines(doc),
        )

    def _group_like_ref(self, plate_wire: str, kind: str) -> PlateRef:
        return PlateRef(
            wire=plate_wire,
            category=kind,
            ident=self.type_idents[plate_wire],
            name=self.type_names[plate_wire],
            kind=kind,
        )

    def _local_name(self, type_wire: str, owner_wire: str) -> Name:
        """The field/alternative name for a hoisted type: the type's words
        with the owning type's leading words stripped (a synthesized
        `note-choice` is field `choice` on `note`); the full name when the
        type's (possibly renamed) name is not prefixed by the owner's."""
        t = self.type_names[type_wire]
        o = self.type_names.get(owner_wire) or self.factory.make(owner_wire)
        tw, ow = tuple(t.words), tuple(o.words)
        if len(tw) > len(ow) and tw[: len(ow)] == ow:
            return self.factory.make("-".join(tw[len(ow):]))
        return self.factory.make("-".join(tw))

    def _occurrence(self, node: ir.Particle) -> tuple[int, int | str]:
        """A field node's effective occurrence. A reference to a
        choice-shaped group absorbs the choice's own occurrence (the type is
        ONE alternative instance; the repetition moves to the field)."""
        if isinstance(node, ir.GroupRef):
            wire = self.group_wire.get(node.name)
            if wire is not None and self.group_shape_of[wire] == "choice":
                inner = self.group_choice_node[wire]
                return self.resolver.occurs_product(
                    node.min, node.max, inner.min, inner.max
                )
        return node.min, node.max

    def _presence_only_ref(self, ref: PlateRef) -> bool:
        if ref.category != "complex":
            return False
        ct = self.complex_by_name.get(ref.wire)
        return ct is not None and ct.presence_only

    def _field_facts(self, owner_wire: str, node: ir.Particle):
        """The shared element/group/choice classification behind fields and
        alternatives: (name, kind, type_ref, tag, occurrence, first, doc)."""
        if isinstance(node, ir.Element):
            mn, mx = node.min, node.max
            pluralize = (
                self.cfg.naming.pluralize_vectors
                and (mx == ir.UNBOUNDED or mx > 1)
            )
            name = self._element_name(node.name, pluralize)
            return (
                name, "element", self._plate_ref(node.type), node.name,
                (mn, mx), [node.name], node.doc,
            )
        if isinstance(node, ir.GroupRef):
            plate_wire = self.group_wire[node.name]
            kind = self.group_shape_of[plate_wire]
            return (
                self.factory.make(node.name), kind,
                self._group_like_ref(plate_wire, kind), None,
                self._occurrence(node), self.resolver.first_names(node), None,
            )
        # A hoisted anonymous choice or sequence.
        wire = self.synth_by_id[id(node)]
        kind = self.synth_kind[wire]
        return (
            self._local_name(wire, owner_wire), kind,
            self._group_like_ref(wire, kind), None,
            (node.min, node.max), self.resolver.first_names(node), None,
        )

    def _field(self, owner_wire: str, node: ir.Particle) -> ContentField:
        name, kind, ref, tag, (mn, mx), first, doc = self._field_facts(owner_wire, node)
        if mx == ir.UNBOUNDED or mx > 1:
            cardinality = "vector"
        elif mn == 0:
            cardinality = "optional"
        else:
            cardinality = "required"
        return ContentField(
            name=name,
            ident=self._field_ident(name),
            kind=kind,
            type_ref=ref,
            cardinality=cardinality,
            tag=tag,
            presence=self._presence_only_ref(ref),
            min1=cardinality == "vector" and mn >= 1,
            max=str(mx) if isinstance(mx, int) and mx > 1 else None,
            nullable=self._instance_nullable(node),
            first=first,
            doc=doc,
        )

    def _instance_nullable(self, node: ir.Particle) -> bool:
        """Whether ONE instance of the field's type can match the empty
        element sequence -- about the content, not the field's occurrence
        bounds (those are the cardinality)."""
        if isinstance(node, ir.Element):
            return False
        if isinstance(node, ir.GroupRef):
            wire = self.group_wire.get(node.name)
            if wire is not None and self.group_shape_of[wire] == "choice":
                inner = self.group_choice_node[wire]
                return any(self.resolver.nullable(a) for a in inner.items)
            group = self.groups_by_name.get(node.name)
            return group is not None and self.resolver.nullable(group.content)
        if isinstance(node, ir.Choice):
            return any(self.resolver.nullable(a) for a in node.items)
        return all(self.resolver.nullable(i) for i in node.items)  # Sequence

    def _alternative(self, choice_wire: str, node: ir.Particle) -> Alternative:
        name, kind, ref, tag, (mn, mx), first, doc = self._field_facts(choice_wire, node)
        if mx == ir.UNBOUNDED or (isinstance(mx, int) and mx > 1):
            cardinality = "vector"
        else:
            if mn == 0:
                # A single optional alternative would make the choice itself
                # optional in a way no current schema uses; fail loud so a
                # future schema construct is a decision, not a silent guess.
                raise PlatesError(
                    [f"{choice_wire}: choice alternative with occurrence (0, 1) "
                     f"has no projection"]
                )
            cardinality = "required"
        return Alternative(
            name=name,
            ident=self._field_ident(name),
            kind=kind,
            type_ref=ref,
            cardinality=cardinality,
            tag=tag,
            presence=self._presence_only_ref(ref),
            min1=cardinality == "vector" and mn >= 1,
            max=str(mx) if isinstance(mx, int) and mx > 1 else None,
            first=first,
            doc=doc,
        )

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
            import_default=self.cfg.import_attribute_defaults.get(
                (owner_wire, a.name)
            ),
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

        # Structural-group and synthesized content-type wires are addressable
        # for renames too (plan §2.9).
        type_wires = set(self.values_by_name) | set(self.complex_by_name)
        type_wires |= {self.group_wire[g] for g in self.struct_groups}
        type_wires |= set(self.synth_content)
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

        for (owner, attr), _literal in self.cfg.import_attribute_defaults.items():
            ct = self.complex_by_name.get(owner)
            if ct is None:
                errors.append(
                    f"import.attribute-defaults.{owner}: no such complex type"
                )
                continue
            match = next(
                (a for a in self.resolver.all_attributes(ct) if a.name == attr), None
            )
            if match is None:
                errors.append(
                    f"import.attribute-defaults.{owner}.{attr}: type has no "
                    f"such attribute"
                )
            elif not match.required:
                errors.append(
                    f"import.attribute-defaults.{owner}.{attr}: attribute is "
                    f"optional; a default would change present-vs-absent "
                    f"round-trip fidelity"
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
