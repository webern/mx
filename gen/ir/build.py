"""Lower a parsed XSD schema (gen.xsd.model) into the IR (gen.ir.model)."""

from __future__ import annotations

from collections import Counter

from gen.xsd import model as xsd
from gen.xsd.analyze import content_particle, reachable_types
from gen.ir import model as ir
from gen.ir.resolve import Resolver

# Map XSD builtin types to canonical IR primitive names.
_XS_PRIMITIVE = {
    "xs:string": "string",
    "xs:token": "token",
    "xs:NMTOKEN": "nmtoken",
    "xs:decimal": "decimal",
    "xs:integer": "integer",
    "xs:int": "integer",
    "xs:positiveInteger": "positive_integer",
    "xs:nonNegativeInteger": "non_negative_integer",
    "xs:date": "date",
    "xs:anyURI": "string",
    "xs:language": "token",
    # The identity builtins are NCName-derived (whitespace-collapsed tokens);
    # no target gives them identity semantics, so they canonicalize to token
    # rather than leaking ID/IDREF as accidental extra primitives.
    "xs:ID": "token",
    "xs:IDREF": "token",
}

# The 10 attribute refs into the imported xml/xlink schemas, resolved to the
# primitive the emitter should use. This is the only place the IR reaches
# outside the main schema.
_EXTERNAL_ATTR = {
    "xml:lang": "token",
    "xml:space": "token",
    "xlink:href": "string",
    "xlink:type": "token",
    "xlink:role": "string",
    "xlink:title": "string",
    "xlink:show": "token",
    "xlink:actuate": "token",
}

_NUMERIC = {"decimal", "integer", "positive_integer", "non_negative_integer"}


def _primitive(name: str) -> str:
    if name in _XS_PRIMITIVE:
        return _XS_PRIMITIVE[name]
    if name in _EXTERNAL_ATTR:
        return _EXTERNAL_ATTR[name]
    return name.split(":")[-1]


def _occ(value: int) -> int | str:
    return ir.UNBOUNDED if value == xsd.UNBOUNDED else value


def _cardinality(min_occurs: int, max_occurs: int) -> str:
    if max_occurs == xsd.UNBOUNDED or max_occurs > 1:
        return "vector"
    return "optional" if min_occurs == 0 else "required"


def build_ir(schema: xsd.Schema, source: str) -> ir.Ir:
    return _Builder(schema, source).build()


class _Builder:
    def __init__(self, schema: xsd.Schema, source: str):
        self.schema = schema
        self.source = source
        self.anon_names: dict[int, str] = {}  # id(ComplexType) -> synthesized name
        self.synth: list[tuple[str, xsd.ComplexType]] = []

    # ----- top level ------------------------------------------------------- #

    def build(self) -> ir.Ir:
        self._hoist_anonymous()
        reachable = reachable_types(self.schema)

        value_types = [
            self._value_type(st)
            for name, st in self.schema.simple_types.items()
            if name in reachable
        ]
        value_types = self._topo_sort_values(value_types)
        groups = [
            ir.Group(name, self._particle(g.particle), g.doc)
            for name, g in self.schema.groups.items()
        ]
        attribute_groups = [
            ir.AttributeGroup(
                name,
                [self._attr(a) for a in ag.attributes],
                [r.ref for r in ag.group_refs],
                ag.doc,
            )
            for name, ag in self.schema.attribute_groups.items()
        ]

        complex_types = [
            self._complex_type(name, ct)
            for name, ct in self.schema.complex_types.items()
            if name in reachable
        ]
        complex_types += [self._complex_type(name, ct) for name, ct in self.synth]

        resolver = Resolver(groups, attribute_groups, complex_types)
        for ct in complex_types:
            ct.deps = sorted(resolver.deps(ct))
        complex_types = self._topo_sort(complex_types)

        all_named = set(self.schema.simple_types) | set(self.schema.complex_types)
        dropped = sorted(all_named - reachable)

        return ir.Ir(
            source=self.source,
            builtins={**_XS_PRIMITIVE, **_EXTERNAL_ATTR},
            value_types=value_types,
            groups=groups,
            attribute_groups=attribute_groups,
            complex_types=complex_types,
            roots=[ir.Root(top.name, top.name) for top in self.schema.elements],
            dropped_dead=dropped,
            stats=self._stats(value_types, complex_types, dropped),
        )

    # ----- anonymous type hoisting ----------------------------------------- #

    def _hoist_anonymous(self) -> None:
        used = set(self.schema.complex_types) | set(self.schema.simple_types)
        # Document roots: the root element name is free; descendants are
        # qualified by the partwise/timewise hierarchy to keep part/measure
        # (which differ between the two) distinct.
        for top in self.schema.elements:
            if top.inline_type:
                qualifier = top.name.replace("score-", "")
                self._hoist(top.inline_type, top.name, qualifier, used)
        # Anonymous types nested inside named types (e.g. directive).
        for ct in self.schema.complex_types.values():
            self._scan(content_particle(ct), "", used)
        for g in self.schema.groups.values():
            self._scan(g.particle, "", used)

    def _hoist(self, ct: xsd.ComplexType, name: str, qualifier: str, used: set) -> None:
        self.anon_names[id(ct)] = name
        self.synth.append((name, ct))
        used.add(name)
        self._scan(content_particle(ct), qualifier, used)

    def _scan(self, particle, qualifier: str, used: set) -> None:
        for ep in _iter_elements(particle):
            if isinstance(ep.inline_type, xsd.ComplexType):
                candidate = f"{qualifier}-{ep.name}" if qualifier else ep.name
                if candidate in used:
                    candidate = f"{qualifier}-{ep.name}" if qualifier else f"{ep.name}-type"
                self._hoist(ep.inline_type, candidate, qualifier, used)

    # ----- value types ----------------------------------------------------- #

    def _value_type(self, st: xsd.SimpleType) -> ir.ValueType:
        if isinstance(st.content, xsd.Union):
            return self._union(st)
        if isinstance(st.content, xsd.ListType):
            # MusicXML uses no xs:list; represent defensively as a token string.
            return ir.StringType(st.name, "token", doc=st.doc)
        primitive, facets = self._resolve_restriction(st.name)
        if facets.enumerations:
            return ir.EnumType(
                st.name, primitive, [e.value for e in facets.enumerations], st.doc
            )
        if primitive in _NUMERIC:
            return ir.NumberType(
                st.name,
                primitive,
                facets.min_inclusive,
                facets.max_inclusive,
                facets.min_exclusive,
                facets.max_exclusive,
                st.doc,
            )
        return ir.StringType(
            st.name,
            primitive,
            list(facets.patterns),
            facets.min_length,
            facets.max_length,
            facets.length,
            st.doc,
        )

    def _resolve_restriction(self, type_name: str) -> tuple[str, xsd.Facets]:
        """Collapse a restriction chain to (primitive, merged facets). Child
        facets override inherited ones; patterns accumulate."""
        st = self.schema.simple_types.get(type_name)
        if st is None or not isinstance(st.content, xsd.Restriction):
            return _primitive(type_name), xsd.Facets()
        base = st.content.base
        if base in self.schema.simple_types:
            primitive, merged = self._resolve_restriction(base)
        else:
            primitive, merged = _primitive(base), xsd.Facets()
        _merge_facets(merged, st.content.facets)
        return primitive, merged

    def _union(self, st: xsd.SimpleType) -> ir.UnionType:
        members: list[ir.UnionMember] = []
        for m in st.content.member_types:
            if m in self.schema.simple_types:
                members.append(ir.UnionMember(ir.Ref(m, "value")))
            else:
                members.append(ir.UnionMember(ir.Ref(_primitive(m), "primitive")))
        for inline in st.content.inline_members:
            if isinstance(inline.content, xsd.Restriction) and inline.content.facets.enumerations:
                members.append(
                    ir.UnionMember(
                        literals=[e.value for e in inline.content.facets.enumerations]
                    )
                )
        return ir.UnionType(st.name, members, st.doc)

    # ----- attributes ------------------------------------------------------ #

    def _attr(self, a: xsd.Attribute) -> ir.Attr:
        if a.ref:
            ref = ir.Ref(_primitive(a.ref), "primitive")
            name = a.ref
        else:
            ref = self._type_ref(a.type) if a.type else ir.Ref("string", "primitive")
            name = a.name or ""
        return ir.Attr(name, ref, a.use == "required", a.default, a.fixed, a.doc)

    # ----- complex types --------------------------------------------------- #

    def _complex_type(self, name: str, ct: xsd.ComplexType) -> ir.ComplexType:
        c = ct.content
        attrs = [self._attr(a) for a in c.attributes]
        agrefs = [r.ref for r in c.attribute_group_refs]

        if isinstance(c, xsd.SimpleContent):
            return ir.ComplexType(
                name, "value", attrs, agrefs, value_type=self._type_ref(c.base), doc=ct.doc
            )
        if isinstance(c, xsd.ComplexContent):
            content = self._particle(c.particle) if c.particle else None
            return ir.ComplexType(
                name, "derived", attrs, agrefs, base=c.base, content=content, doc=ct.doc
            )
        # ImplicitContent
        if c.particle is not None:
            return ir.ComplexType(
                name, "composite", attrs, agrefs, content=self._particle(c.particle), doc=ct.doc
            )
        presence = not attrs and not agrefs
        return ir.ComplexType(name, "empty", attrs, agrefs, presence_only=presence, doc=ct.doc)

    # ----- particles ------------------------------------------------------- #

    def _particle(self, p) -> ir.Particle:
        if isinstance(p, xsd.Sequence):
            return ir.Sequence([self._particle(i) for i in p.items], p.min_occurs, _occ(p.max_occurs))
        if isinstance(p, xsd.Choice):
            return ir.Choice([self._particle(i) for i in p.items], p.min_occurs, _occ(p.max_occurs))
        if isinstance(p, xsd.GroupRef):
            return ir.GroupRef(p.ref, p.min_occurs, _occ(p.max_occurs))
        if isinstance(p, xsd.ElementParticle):
            return ir.Element(
                p.name,
                self._element_ref(p),
                _cardinality(p.min_occurs, p.max_occurs),
                p.min_occurs,
                _occ(p.max_occurs),
                p.doc,
            )
        raise ValueError(f"unexpected particle: {type(p).__name__}")

    def _element_ref(self, ep: xsd.ElementParticle) -> ir.Ref:
        if isinstance(ep.inline_type, xsd.ComplexType):
            return ir.Ref(self.anon_names[id(ep.inline_type)], "complex")
        if ep.type:
            return self._type_ref(ep.type)
        return ir.Ref("string", "primitive")

    def _type_ref(self, type_name: str) -> ir.Ref:
        if type_name in self.schema.complex_types:
            return ir.Ref(type_name, "complex")
        if type_name in self.schema.simple_types:
            return ir.Ref(type_name, "value")
        if type_name.startswith(("xs:", "xml:", "xlink:")):
            return ir.Ref(_primitive(type_name), "primitive")
        return ir.Ref(type_name, "complex")

    # ----- dependency ordering --------------------------------------------- #

    def _topo_sort(self, types: list[ir.ComplexType]) -> list[ir.ComplexType]:
        by_name = {t.name: t for t in types}
        ordered: list[ir.ComplexType] = []
        state: dict[str, int] = {}  # 0 visiting, 1 done

        def visit(name: str) -> None:
            if state.get(name) == 1 or name not in by_name:
                return
            state[name] = 0
            for dep in by_name[name].deps:
                visit(dep)
            state[name] = 1
            ordered.append(by_name[name])

        for name in sorted(by_name):
            visit(name)
        return ordered

    def _topo_sort_values(self, values: list[ir.ValueType]) -> list[ir.ValueType]:
        """Order value types deps-first. Only unions reference other value
        types (their members); every other kind resolves to a primitive."""
        by_name = {v.name: v for v in values}
        ordered: list[ir.ValueType] = []
        state: dict[str, int] = {}

        def deps(v: ir.ValueType) -> list[str]:
            if isinstance(v, ir.UnionType):
                return [
                    m.ref.name
                    for m in v.members
                    if m.ref and m.ref.category == "value" and m.ref.name in by_name
                ]
            return []

        def visit(name: str) -> None:
            if state.get(name) == 1 or name not in by_name:
                return
            state[name] = 1
            for dep in sorted(deps(by_name[name])):
                visit(dep)
            ordered.append(by_name[name])

        for name in sorted(by_name):
            visit(name)
        return ordered

    # ----- stats ----------------------------------------------------------- #

    def _stats(self, value_types, complex_types, dropped) -> dict:
        return {
            "value_types": len(value_types),
            "value_kinds": dict(Counter(v.kind for v in value_types)),
            "complex_types": len(complex_types),
            "complex_kinds": dict(Counter(c.kind for c in complex_types)),
            "groups": len(self.schema.groups),
            "attribute_groups": len(self.schema.attribute_groups),
            "synthesized_types": len(self.synth),
            "dropped_dead_types": len(dropped),
        }


# --------------------------------------------------------------------------- #
# Helpers
# --------------------------------------------------------------------------- #


def _iter_elements(particle):
    """Yield element particles directly contained in a particle (not inside
    group refs, which are scanned at the group definition)."""
    if isinstance(particle, (xsd.Sequence, xsd.Choice)):
        for item in particle.items:
            yield from _iter_elements(item)
    elif isinstance(particle, xsd.ElementParticle):
        yield particle


def _merge_facets(into: xsd.Facets, src: xsd.Facets) -> None:
    if src.enumerations:
        into.enumerations = src.enumerations
    into.patterns = into.patterns + src.patterns
    for f in ("min_inclusive", "max_inclusive", "min_exclusive", "max_exclusive",
              "min_length", "max_length", "length"):
        v = getattr(src, f)
        if v is not None:
            setattr(into, f, v)
