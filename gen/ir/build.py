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
    # The XML namespace's language tag: a token restricted by the builtin
    # pattern [a-zA-Z]{1,8}(-[a-zA-Z0-9]{1,8})* (XSD 1.0 Part 2). A distinct
    # primitive for the same reason the identity tokens are: a target that
    # wants validity-by-construction repairs it, and a non-validating target
    # maps it straight back to its token spelling (zero output change).
    "xs:language": "language",
    # NCName is NMTOKEN minus the colon; no MusicXML type rests on it
    # directly, but the imported xml:space enumeration does.
    "xs:NCName": "nmtoken",
    # The identity builtins are NCName-derived: lexically NARROWER than token
    # (no whitespace, no empty value, no leading digit or ':' for ID/IDREF;
    # ID values are document-unique). Preserved as distinct primitives -- a
    # neutral schema fact -- so a target that wants validity-by-construction
    # can repair them and a non-validating target maps them straight back to
    # its token spelling (zero output change). NMTOKEN is already distinct.
    "xs:ID": "id",
    "xs:IDREF": "idref",
}

# The imported xml/xlink attribute refs the IR resolves to a builtin
# primitive instead of following the import: xml:lang's declared type is the
# xs:language BUILTIN (no schema declares it) and href/role/title are
# xs:anyURI/xs:string, which accept nearly any string. Every other external
# ref is lowered from its declaration in the imported schema, which the XSD
# parser has read beside the main one.
_EXTERNAL_ATTR = {
    "xml:lang": "language",
    "xlink:href": "string",
    "xlink:role": "string",
    "xlink:title": "string",
}

_NUMERIC = {"decimal", "integer", "positive_integer", "non_negative_integer"}

# The closed set of canonical IR primitives (the values of the builtins map):
# everything a Ref with category "primitive" can name, and the keys a target
# type map may override.
PRIMITIVES = frozenset(_XS_PRIMITIVE.values()) | frozenset(_EXTERNAL_ATTR.values())


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
        # The imported schemas' attribute declarations, indexed by local name
        # (the wire ref keeps its prefix).
        self.external_attrs: dict[str, list[tuple[str, xsd.Attribute]]] = {}
        for namespace, ext in schema.imported.items():
            for attr_name, decl in ext.attributes.items():
                self.external_attrs.setdefault(attr_name, []).append((namespace, decl))
        # Value types lowered out of the imported schemas, by IR name. They
        # are picked up when an attribute ref reaches them (so no unreachable
        # external type ever enters the IR).
        self.external_values: dict[str, ir.ValueType] = {}

    # ----- top level ------------------------------------------------------- #

    def build(self) -> ir.Ir:
        self._hoist_anonymous()
        reachable = reachable_types(self.schema)

        value_types = [
            self._value_type(st, self.schema.simple_types)
            for name, st in self.schema.simple_types.items()
            if name in reachable
        ]
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

        # The attribute refs lowered above may have pulled value types out of
        # the imported schemas; they join the emit order deps-first.
        value_types += list(self.external_values.values())
        value_types = self._topo_sort_values(value_types)

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

    def _value_type(self, st: xsd.SimpleType, simple_types: dict, name: str | None = None) -> ir.ValueType:
        """Lower one simple type. `simple_types` is the schema the type is
        declared in (the main one, or an imported schema); `name` overrides
        the type's own name, for an anonymous type hoisted under a name the
        declaring schema gave it only by context."""
        type_name = st.name if name is None else name
        if isinstance(st.content, xsd.Union):
            return self._union(type_name, st, simple_types)
        if isinstance(st.content, xsd.ListType):
            # MusicXML uses no xs:list; represent defensively as a token string.
            return ir.StringType(type_name, "token", doc=st.doc)
        primitive, facets = self._restriction(simple_types, st)
        if facets.enumerations:
            return ir.EnumType(
                type_name, primitive, [e.value for e in facets.enumerations], st.doc
            )
        if primitive in _NUMERIC:
            return ir.NumberType(
                type_name,
                primitive,
                facets.min_inclusive,
                facets.max_inclusive,
                facets.min_exclusive,
                facets.max_exclusive,
                st.doc,
            )
        return ir.StringType(
            type_name,
            primitive,
            list(facets.patterns),
            facets.min_length,
            facets.max_length,
            facets.length,
            st.doc,
        )

    def _restriction(self, simple_types: dict, st: xsd.SimpleType) -> tuple[str, xsd.Facets]:
        """A restriction's own facets merged onto its base chain."""
        primitive, merged = self._resolve_restriction(simple_types, st.content.base)
        _merge_facets(merged, st.content.facets)
        return primitive, merged

    def _resolve_restriction(self, simple_types: dict, type_name: str) -> tuple[str, xsd.Facets]:
        """Collapse a restriction chain to (primitive, merged facets). Child
        facets override inherited ones; patterns accumulate."""
        st = simple_types.get(type_name)
        if st is None or not isinstance(st.content, xsd.Restriction):
            return _primitive(type_name), xsd.Facets()
        return self._restriction(simple_types, st)

    def _union(self, name: str, st: xsd.SimpleType, simple_types: dict) -> ir.UnionType:
        members: list[ir.UnionMember] = []
        for m in st.content.member_types:
            if m in simple_types:
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
            ref = self._external_ref(a.ref)
            name = a.ref
        else:
            ref = self._type_ref(a.type) if a.type else ir.Ref("string", "primitive")
            name = a.name or ""
        return ir.Attr(name, ref, a.use == "required", a.default, a.fixed, a.doc)

    # ----- imported attributes --------------------------------------------- #

    def _external_ref(self, ref_name: str) -> ir.Ref:
        """Resolve an imported attribute ref (xml:lang, xlink:type, ...) to the
        type its declaration names.

        The pinned refs in _EXTERNAL_ATTR never look at the import: their
        declared type is an XML-Schema BUILTIN with nothing to read. The rest
        are declared in the imported xml/xlink schemas, which the XSD parser
        has already read beside the main one.
        """
        pinned = _EXTERNAL_ATTR.get(ref_name)
        if pinned is not None:
            return ir.Ref(pinned, "primitive")
        candidates = self.external_attrs.get(ref_name.split(":")[-1], [])
        if len(candidates) != 1:
            raise ValueError(
                f"external attribute ref {ref_name!r} resolves to "
                f"{len(candidates)} declarations in the imported schemas"
            )
        namespace, decl = candidates[0]
        return self._external_decl_ref(namespace, decl, ref_name)

    def _external_decl_ref(self, namespace: str, decl: xsd.Attribute, ref_name: str) -> ir.Ref:
        """The type an imported attribute declaration carries: a named type
        from its own schema (xlink:typeType), or its inline type hoisted under
        the qualified ref name (xml:space)."""
        if decl.type:
            return self._external_type_ref(namespace, decl.type)
        if decl.inline_type is not None:
            name = ref_name.replace(":", "-")
            self._external_value(name, decl.inline_type, namespace)
            return ir.Ref(name, "value")
        return ir.Ref("string", "primitive")

    def _external_type_ref(self, namespace: str, type_name: str) -> ir.Ref:
        ext = self.schema.imported[namespace]
        name = type_name.split(":")[-1]
        st = ext.simple_types.get(name)
        if st is None:
            return ir.Ref(_primitive(type_name), "primitive")
        self._external_value(name, st, namespace)
        return ir.Ref(name, "value")

    def _external_value(self, name: str, st: xsd.SimpleType, namespace: str) -> None:
        """Lower (once) a value type declared in an imported schema."""
        if name not in self.external_values:
            self.external_values[name] = self._value_type(
                st, self.schema.imported[namespace].simple_types, name=name
            )

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
