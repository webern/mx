"""Structural analysis of a parsed MusicXML schema.

The generator does not need this to emit code, but the shape of the schema
drives every design decision in the generator, so it is worth measuring up
front. ``report`` produces a human-readable summary; the helper functions
(element_index, reachable_types, type_graph) compute structures the generator
itself will reuse later.
"""

from __future__ import annotations

from collections import Counter, defaultdict

from gen.xsd.model import (
    Attribute,
    AttributeGroup,
    AttributeGroupRef,
    Choice,
    ComplexContent,
    ComplexType,
    ElementParticle,
    GroupRef,
    ImplicitContent,
    ListType,
    Restriction,
    Schema,
    Sequence,
    SimpleContent,
    SimpleType,
    UNBOUNDED,
    Union,
)

# Builtin xs: types are not defined in the schema; treat any name with this
# prefix (or an external namespace prefix) as a leaf, not a dead reference.
_BUILTIN_PREFIXES = ("xs:", "xml:", "xlink:")


def _is_named(name: str | None) -> bool:
    return bool(name) and not name.startswith(_BUILTIN_PREFIXES)


# --------------------------------------------------------------------------- #
# Particle walking
# --------------------------------------------------------------------------- #


def content_particle(ct: ComplexType):
    """The particle of a complex type, or None for value/empty types."""
    if isinstance(ct.content, (ComplexContent, ImplicitContent)):
        return ct.content.particle
    return None


def _walk_local(particle):
    """Yield element particles in a content model, recursing into nested
    sequences/choices and inline complex types but NOT into group refs (group
    contents are counted at the group definition to avoid double counting)."""
    if isinstance(particle, (Sequence, Choice)):
        for item in particle.items:
            yield from _walk_local(item)
    elif isinstance(particle, ElementParticle):
        yield particle
        if isinstance(particle.inline_type, ComplexType):
            yield from _walk_local(content_particle(particle.inline_type))


def _element_type_targets(particle, groups, _seen=None) -> set[str]:
    """All element type names reachable from a particle, following group refs
    and inline complex types. Used to build the type reference graph."""
    seen_groups = set() if _seen is None else _seen
    targets: set[str] = set()
    stack = [particle]
    while stack:
        p = stack.pop()
        if isinstance(p, (Sequence, Choice)):
            stack.extend(p.items)
        elif isinstance(p, GroupRef):
            if p.ref in groups and p.ref not in seen_groups:
                seen_groups.add(p.ref)
                stack.append(groups[p.ref].particle)
        elif isinstance(p, ElementParticle):
            if p.type:
                targets.add(p.type)
            if isinstance(p.inline_type, ComplexType):
                stack.append(content_particle(p.inline_type))
    return targets


# --------------------------------------------------------------------------- #
# Reusable index structures
# --------------------------------------------------------------------------- #


def element_index(schema: Schema) -> dict[str, list[str]]:
    """Map each element name to the list of type names it is declared with
    across every content model. Inline types show as '(anonymous)'."""
    index: dict[str, list[str]] = defaultdict(list)

    def record(particle):
        for ep in _walk_local(particle):
            index[ep.name].append(ep.type or "(anonymous)")

    for ct in schema.complex_types.values():
        record(content_particle(ct))
    for g in schema.groups.values():
        record(g.particle)
    for top in schema.elements:
        if top.inline_type:
            record(content_particle(top.inline_type))
    return index


def _attr_type_names(attrs, agrefs, attribute_groups, seen=None) -> set[str]:
    """Type names referenced by a set of attributes, expanding attributeGroup
    refs (deduplicated across nested groups)."""
    seen = set() if seen is None else seen
    names: set[str] = set()
    for a in attrs:
        if a.type:
            names.add(a.type)
    for ref in agrefs:
        if ref.ref in attribute_groups and ref.ref not in seen:
            seen.add(ref.ref)
            ag = attribute_groups[ref.ref]
            names |= _attr_type_names(ag.attributes, ag.group_refs, attribute_groups, seen)
    return names


def _complex_refs(ct: ComplexType, schema: Schema) -> set[str]:
    """All named type names a complex type references: base, element types, and
    attribute types (through attribute groups). Recurses into anonymous inline
    types so their attribute and element references are reachable too."""
    refs: set[str] = set()
    _gather_complex_refs(ct, schema, refs)
    return {r for r in refs if _is_named(r)}


def _gather_complex_refs(ct: ComplexType, schema: Schema, refs: set[str]) -> None:
    c = ct.content
    if isinstance(c, (SimpleContent, ComplexContent)):
        refs.add(c.base)
    if isinstance(c, (SimpleContent, ComplexContent, ImplicitContent)):
        refs |= _attr_type_names(c.attributes, c.attribute_group_refs, schema.attribute_groups)
    stack = [content_particle(ct)]
    seen_groups: set[str] = set()
    while stack:
        p = stack.pop()
        if isinstance(p, (Sequence, Choice)):
            stack.extend(p.items)
        elif isinstance(p, GroupRef):
            if p.ref in schema.groups and p.ref not in seen_groups:
                seen_groups.add(p.ref)
                stack.append(schema.groups[p.ref].particle)
        elif isinstance(p, ElementParticle):
            if p.type:
                refs.add(p.type)
            if isinstance(p.inline_type, ComplexType):
                _gather_complex_refs(p.inline_type, schema, refs)


def _simple_refs(st: SimpleType) -> set[str]:
    if isinstance(st.content, Union):
        return {m for m in st.content.member_types if _is_named(m)}
    if isinstance(st.content, ListType):
        return {st.content.item_type} if _is_named(st.content.item_type) else set()
    if isinstance(st.content, Restriction) and _is_named(st.content.base):
        return {st.content.base}
    return set()


def reachable_types(schema: Schema) -> set[str]:
    """Named types reachable from the document roots. The complement is dead."""
    seen: set[str] = set()
    frontier: list[str] = []
    for top in schema.elements:
        if top.type:
            frontier.append(top.type)
        if top.inline_type:
            frontier += _complex_refs(top.inline_type, schema)
    while frontier:
        t = frontier.pop()
        if t in seen or not _is_named(t):
            continue
        seen.add(t)
        if t in schema.complex_types:
            frontier += _complex_refs(schema.complex_types[t], schema)
        elif t in schema.simple_types:
            frontier += _simple_refs(schema.simple_types[t])
    return seen


def type_graph(schema: Schema) -> dict[str, set[str]]:
    """Directed graph among complex types: X -> Y when X structurally contains
    an element of complex type Y (or extends Y). Drives recursion analysis."""
    graph: dict[str, set[str]] = {}
    cts = schema.complex_types
    for name, ct in cts.items():
        targets = _element_type_targets(content_particle(ct), schema.groups)
        c = ct.content
        if isinstance(c, (SimpleContent, ComplexContent)):
            targets.add(c.base)
        graph[name] = {t for t in targets if t in cts}
    return graph


def _sccs(graph: dict[str, set[str]]) -> list[list[str]]:
    """Tarjan's strongly connected components (iterative)."""
    index: dict[str, int] = {}
    low: dict[str, int] = {}
    on_stack: set[str] = set()
    stack: list[str] = []
    result: list[list[str]] = []
    counter = 0

    for root in graph:
        if root in index:
            continue
        work = [(root, iter(graph[root]))]
        while work:
            node, children = work[-1]
            if node not in index:
                index[node] = low[node] = counter
                counter += 1
                stack.append(node)
                on_stack.add(node)
            advanced = False
            for child in children:
                if child not in index:
                    work.append((child, iter(graph[child])))
                    advanced = True
                    break
                if child in on_stack:
                    low[node] = min(low[node], index[child])
            if advanced:
                continue
            if low[node] == index[node]:
                comp = []
                while True:
                    w = stack.pop()
                    on_stack.discard(w)
                    comp.append(w)
                    if w == node:
                        break
                result.append(comp)
            work.pop()
            if work:
                parent = work[-1][0]
                low[parent] = min(low[parent], low[node])
    return result


# --------------------------------------------------------------------------- #
# Categorization
# --------------------------------------------------------------------------- #

_NUMERIC_BASES = {
    "xs:decimal",
    "xs:integer",
    "xs:positiveInteger",
    "xs:nonNegativeInteger",
    "xs:int",
}


def _classify_simple(st: SimpleType) -> str:
    c = st.content
    if isinstance(c, Union):
        return "union"
    if isinstance(c, ListType):
        return "list"
    f = c.facets
    if f.enumerations:
        return "enumeration"
    if f.patterns:
        return "pattern"
    if any((f.min_inclusive, f.max_inclusive, f.min_exclusive, f.max_exclusive)):
        return "numeric range"
    if any((f.min_length, f.max_length, f.length)):
        return "length constrained"
    return "alias"


def _classify_complex(ct: ComplexType) -> str:
    c = ct.content
    if isinstance(c, SimpleContent):
        return "simple content (value + attrs)"
    if isinstance(c, ComplexContent):
        return "complex content (derived)"
    # ImplicitContent
    if c.particle is not None:
        return "composite (sequence/choice)"
    if c.attributes or c.attribute_group_refs:
        return "attributes only (empty element)"
    return "empty"


# --------------------------------------------------------------------------- #
# Report
# --------------------------------------------------------------------------- #


def _row(label: str, value) -> str:
    return f"  {label:<40} {value:>7}"


def _h(title: str) -> list[str]:
    return ["", title, "-" * len(title)]


def report(schema: Schema) -> str:
    out: list[str] = []
    out.append("=" * 60)
    out.append("MusicXML XSD structural analysis")
    out.append("=" * 60)

    # ---- component inventory ----
    out += _h("Component inventory")
    out.append(_row("named simple types", len(schema.simple_types)))
    out.append(_row("named complex types", len(schema.complex_types)))
    out.append(_row("model groups", len(schema.groups)))
    out.append(_row("attribute groups", len(schema.attribute_groups)))
    out.append(_row("document root elements", len(schema.elements)))
    out.append(_row("imported schemas", len(schema.imports)))

    # ---- simple types ----
    simple_kinds = Counter(_classify_simple(st) for st in schema.simple_types.values())
    out += _h("Simple types by kind")
    for kind, n in simple_kinds.most_common():
        out.append(_row(kind, n))

    enums = {n: st for n, st in schema.simple_types.items() if _classify_simple(st) == "enumeration"}
    total_values = sum(len(st.content.facets.enumerations) for st in enums.values())
    out += _h("Enumerations")
    out.append(_row("enumeration types", len(enums)))
    out.append(_row("total enumeration values", total_values))
    largest = sorted(enums.items(), key=lambda kv: len(kv[1].content.facets.enumerations), reverse=True)
    out.append("  largest:")
    for name, st in largest[:8]:
        out.append(_row(f"    {name}", len(st.content.facets.enumerations)))

    bases = Counter(
        st.content.base
        for st in schema.simple_types.values()
        if isinstance(st.content, Restriction)
    )
    out += _h("Simple type restriction bases")
    for base, n in bases.most_common():
        out.append(_row(base, n))

    # ---- complex types ----
    complex_kinds = Counter(_classify_complex(ct) for ct in schema.complex_types.values())
    out += _h("Complex types by content model")
    for kind, n in complex_kinds.most_common():
        out.append(_row(kind, n))

    # ---- elements ----
    index = element_index(schema)
    sites = sum(len(v) for v in index.values())
    collisions = {n: sorted(set(t)) for n, t in index.items() if len(set(t)) > 1}
    same_name_type = sum(
        1 for n, types in index.items() for t in types if t == n
    )
    out += _h("Elements")
    out.append(_row("distinct element names", len(index)))
    out.append(_row("element declaration sites", sites))
    out.append(_row("sites where element name == type name", same_name_type))
    out.append(_row("names declared with >1 distinct type", len(collisions)))
    if collisions:
        out.append("  collisions (same name, different types):")
        for name in sorted(collisions)[:12]:
            out.append(f"      {name}: {', '.join(collisions[name])}")

    # type reuse (fan-in by element sites)
    fan_in = Counter(t for types in index.values() for t in types if t != "(anonymous)")
    out += _h("Most reused types (by element declaration sites)")
    for name, n in fan_in.most_common(12):
        out.append(_row(name, n))

    # ---- recursion ----
    graph = type_graph(schema)
    self_loops = sorted(n for n, deps in graph.items() if n in deps)
    cycles = [c for c in _sccs(graph) if len(c) > 1]
    out += _h("Recursion (complex type reference graph)")
    out.append(_row("complex types in a cycle (SCC > 1)", sum(len(c) for c in cycles)))
    out.append(_row("self-referential types", len(self_loops)))
    if self_loops:
        out.append("  self-referential: " + ", ".join(self_loops))
    for comp in sorted(cycles, key=len, reverse=True)[:6]:
        out.append(f"  cycle ({len(comp)}): {', '.join(sorted(comp))}")

    # ---- dead types ----
    reachable = reachable_types(schema)
    all_named = set(schema.simple_types) | set(schema.complex_types)
    dead = sorted(all_named - reachable)
    out += _h("Reachability from document roots")
    out.append(_row("named types reachable", len(reachable)))
    out.append(_row("named types unreachable (dead)", len(dead)))
    if dead:
        out.append("  dead: " + ", ".join(dead[:20]) + (" ..." if len(dead) > 20 else ""))

    # ---- attributes ----
    out += _h("Attributes")
    all_attrs: list[Attribute] = []
    for ct in schema.complex_types.values():
        c = ct.content
        if isinstance(c, (SimpleContent, ComplexContent, ImplicitContent)):
            all_attrs += c.attributes
    for ag in schema.attribute_groups.values():
        all_attrs += ag.attributes
    required = sum(1 for a in all_attrs if a.use == "required")
    defaults = sum(1 for a in all_attrs if a.default is not None)
    fixed = sum(1 for a in all_attrs if a.fixed is not None)
    external = sum(1 for a in all_attrs if a.ref and a.ref.startswith(_BUILTIN_PREFIXES))
    out.append(_row("attribute declarations", len(all_attrs)))
    out.append(_row("required", required))
    out.append(_row("optional", len(all_attrs) - required))
    out.append(_row("with default value", defaults))
    out.append(_row("with fixed value", fixed))
    out.append(_row("external refs (xml:/xlink:)", external))

    # attribute group reuse
    ag_fan_in: Counter[str] = Counter()
    for ct in schema.complex_types.values():
        c = ct.content
        if isinstance(c, (SimpleContent, ComplexContent, ImplicitContent)):
            for r in c.attribute_group_refs:
                ag_fan_in[r.ref] += 1
    for ag in schema.attribute_groups.values():
        for r in ag.group_refs:
            ag_fan_in[r.ref] += 1
    out += _h("Most reused attribute groups")
    for name, n in ag_fan_in.most_common(10):
        out.append(_row(name, n))

    # model group reuse
    group_fan_in: Counter[str] = Counter()

    def count_group_refs(particle):
        stack = [particle]
        while stack:
            p = stack.pop()
            if isinstance(p, (Sequence, Choice)):
                stack.extend(p.items)
            elif isinstance(p, GroupRef):
                group_fan_in[p.ref] += 1
            elif isinstance(p, ElementParticle) and isinstance(p.inline_type, ComplexType):
                stack.append(content_particle(p.inline_type))

    for ct in schema.complex_types.values():
        count_group_refs(content_particle(ct))
    for g in schema.groups.values():
        count_group_refs(g.particle)
    for top in schema.elements:
        if top.inline_type:
            count_group_refs(content_particle(top.inline_type))
    out += _h("Most reused model groups")
    for name, n in group_fan_in.most_common(10):
        out.append(_row(name, n))

    out.append("")
    return "\n".join(out)
