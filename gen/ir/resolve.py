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
        out: list[ir.Attr] = []
        seen: set[str] = set()
        for c in self.base_chain(ct):
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
        grouping and keeps each occurrence's LOCAL cardinality; use content()
        when the structure matters and flat_elements() for the effective,
        deduplicated field view an emitter wants."""
        out: list[ir.Element] = []
        self._collect_elements(self.content(ct), out)
        return out

    def _collect_elements(self, p, out) -> None:
        if isinstance(p, (ir.Sequence, ir.Choice)):
            for i in p.items:
                self._collect_elements(i, out)
        elif isinstance(p, ir.Element):
            out.append(p)

    def flat_elements(self, ct: ir.ComplexType) -> list[tuple[ir.Element, str]]:
        """Each distinct element name in ct's resolved content, in document
        order of first occurrence, with its EFFECTIVE cardinality for a flat
        one-field-per-name view:

          - an element under any repeated particle (max != 1) is a vector;
          - an element under a choice, or under an optional wrapper, is at
            most optional;
          - only an element required along a spine of exactly-once sequences
            stays required.

        Occurrences of the same name merge by co-occurrence analysis: if two
        occurrences sit in different branches of one choice they are mutually
        exclusive (at most one per instance: optional), but otherwise both
        can appear in a single instance and the merged field must be a vector
        (e.g. metronome's beat-unit, which appears on a branch's spine and
        again inside that same branch's inner choice)."""
        merged: dict[str, int] = {}  # name -> index into out
        paths: dict[str, list[tuple]] = {}  # name -> choice paths seen
        out: list[tuple[ir.Element, str]] = []
        rank = {"required": 0, "optional": 1, "vector": 2}

        def exclusive(a: tuple, b: tuple) -> bool:
            """True when the two occurrence paths diverge at two different
            branches of one choice node, so they can never co-occur."""
            i = 0
            while i < len(a) and i < len(b) and a[i] == b[i]:
                i += 1
            return (
                i < len(a)
                and i < len(b)
                and a[i][0] == b[i][0]  # same choice node
                and a[i][1] != b[i][1]  # different branches
            )

        def walk(node, forced: bool, repeated: bool, path: tuple) -> None:
            if node is None:
                return
            if isinstance(node, ir.Element):
                if repeated or node.card == "vector":
                    card = "vector"
                elif forced and node.card == "required":
                    card = "required"
                else:
                    card = "optional"
                if node.name not in merged:
                    merged[node.name] = len(out)
                    paths[node.name] = [path]
                    out.append((node, card))
                    return
                i = merged[node.name]
                prev_el, prev_card = out[i]
                if all(exclusive(path, seen) for seen in paths[node.name]):
                    # Alternative branches: at most one occurs, but none is
                    # statically guaranteed.
                    card = max(card, prev_card, key=lambda c: rank[c])
                    if card == "required":
                        card = "optional"
                else:
                    # The occurrences can co-occur in one instance.
                    card = "vector"
                paths[node.name].append(path)
                out[i] = (prev_el, card)
                return
            if node.max == 0:
                return  # a never-occurring particle contributes nothing
            once = node.min >= 1 and node.max == 1
            again = repeated or node.max != 1
            if isinstance(node, ir.Sequence):
                for item in node.items:
                    walk(item, forced and once, again, path)
            elif isinstance(node, ir.Choice):
                for branch, item in enumerate(node.items):
                    walk(item, False, again, path + ((id(node), branch),))
            # GroupRef leaves cannot appear: content() spliced them.

        walk(self.content(ct), True, False, ())
        return out

    def all_flat_elements(self, ct: ir.ComplexType) -> list[tuple[ir.Element, str]]:
        """flat_elements() merged across the base chain (base-most first,
        first occurrence of a name wins), mirroring all_attributes, for the
        flattened view a target without inheritance emits."""
        out: list[tuple[ir.Element, str]] = []
        seen: set[str] = set()
        for c in self.base_chain(ct):
            for element, card in self.flat_elements(c):
                if element.name not in seen:
                    seen.add(element.name)
                    out.append((element, card))
        return out

    # ----- derivation ------------------------------------------------------ #

    def base_chain(self, ct: ir.ComplexType) -> list[ir.ComplexType]:
        """ct's derivation chain, base-most first, ending with ct itself."""
        chain: list[ir.ComplexType] = []
        cur: ir.ComplexType | None = ct
        while cur is not None:
            chain.append(cur)
            cur = self._complex.get(cur.base) if cur.base else None
        chain.reverse()
        return chain

    # ----- dependencies ---------------------------------------------------- #

    def deps(self, ct: ir.ComplexType) -> set[str]:
        """Complex types ct structurally depends on: its child element types
        (groups resolved) plus its base. Drives the topological emit order."""
        d = {e.type.name for e in self.elements(ct) if e.type.category == "complex"}
        if ct.base:
            d.add(ct.base)
        return d
