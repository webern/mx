"""Collapsed views over the named structure the IR preserves.

The IR keeps the schema's reusable structure addressable: a complex type lists
its attribute groups by name and leaves group references in its content tree, so
an emitter that wants mixins or shared structs can mirror them. Most emitters
instead want the collapsed view -- the full ordered attribute list, the content
with groups spliced in. Producing it means expanding attribute-group and
model-group references, deduping, and guarding cycles. That is schema reasoning,
so it lives here, once, rather than re-derived in every target's templates.

Resolver is a pure read over the IR; it never mutates it. It depends only on the
three reusable tables (groups, attribute groups, complex types), not the whole
Ir, so build can use it mid-construction to compute dependencies.
"""

from __future__ import annotations

from gen.ir import model as ir


class Resolver:
    """Collapsed views over an IR's preserved named structure."""

    def __init__(
        self,
        groups: list[ir.Group],
        attribute_groups: list[ir.AttributeGroup],
        complex_types: list[ir.ComplexType],
    ):
        self._groups = {g.name: g for g in groups}
        self._agroups = {a.name: a for a in attribute_groups}
        self._complex = {c.name: c for c in complex_types}

    @classmethod
    def from_ir(cls, m: ir.Ir) -> "Resolver":
        return cls(m.groups, m.attribute_groups, m.complex_types)

    # ----- attributes ------------------------------------------------------ #

    def attributes(self, ct: ir.ComplexType) -> list[ir.Attr]:
        """ct's own attributes with its attribute groups expanded inline, in
        declaration order, deduped by name (first wins). Excludes the base."""
        out: list[ir.Attr] = []
        self._add_attrs(ct.attributes, ct.attribute_groups, out, set(), set())
        return out

    def all_attributes(self, ct: ir.ComplexType) -> list[ir.Attr]:
        """attributes() plus the base chain's attributes (base-most first), for
        the flattened set an emitter needs when the target has no inheritance."""
        chain: list[ir.ComplexType] = []
        cur: ir.ComplexType | None = ct
        while cur is not None:
            chain.append(cur)
            cur = self._complex.get(cur.base) if cur.base else None
        out: list[ir.Attr] = []
        seen: set[str] = set()
        for c in reversed(chain):
            self._add_attrs(c.attributes, c.attribute_groups, out, seen, set())
        return out

    def _add_attrs(self, attrs, group_names, out, seen, seen_groups) -> None:
        for a in attrs:
            if a.name not in seen:
                seen.add(a.name)
                out.append(a)
        for name in group_names:
            ag = self._agroups.get(name)
            if ag is not None and name not in seen_groups:
                seen_groups.add(name)
                self._add_attrs(ag.attributes, ag.attribute_groups, out, seen, seen_groups)

    # ----- content --------------------------------------------------------- #

    def content(self, ct: ir.ComplexType) -> ir.Particle | None:
        """ct.content with every group reference spliced in: a self-contained
        tree of elements/sequences/choices with no GroupRef nodes. Nesting and
        all min/max bounds are preserved. None for types with no content."""
        return None if ct.content is None else self._inline(ct.content, ())

    def _inline(self, p: ir.Particle, path: tuple[str, ...]) -> ir.Particle:
        if isinstance(p, ir.Sequence):
            return ir.Sequence([self._inline(i, path) for i in p.items], p.min, p.max)
        if isinstance(p, ir.Choice):
            return ir.Choice([self._inline(i, path) for i in p.items], p.min, p.max)
        if isinstance(p, ir.GroupRef):
            g = self._groups.get(p.name)
            if g is None or p.name in path:  # unknown or cyclic: leave the leaf
                return p
            body = self._inline(g.content, path + (p.name,))
            # The ref's occurrence wraps the group body's own. Drop the wrapper
            # when the ref is exactly-one and so contributes nothing.
            if p.min == 1 and p.max == 1:
                return body
            return ir.Sequence([body], p.min, p.max)
        return p  # Element: a leaf with an already-resolved Ref

    # ----- elements -------------------------------------------------------- #

    def elements(self, ct: ir.ComplexType) -> list[ir.Element]:
        """Every element occurrence in ct's resolved content, in document order,
        flattened across sequences/choices/groups. Drops the choice/sequence
        grouping; use content() when that structure matters."""
        out: list[ir.Element] = []
        self._collect_elements(self.content(ct), out)
        return out

    def _collect_elements(self, p, out) -> None:
        if isinstance(p, (ir.Sequence, ir.Choice)):
            for i in p.items:
                self._collect_elements(i, out)
        elif isinstance(p, ir.Element):
            out.append(p)

    # ----- dependencies ---------------------------------------------------- #

    def deps(self, ct: ir.ComplexType) -> set[str]:
        """Complex types ct structurally depends on: its child element types
        (groups resolved) plus its base. Drives the topological emit order."""
        d = {e.type.name for e in self.elements(ct) if e.type.category == "complex"}
        if ct.base:
            d.add(ct.base)
        return d
