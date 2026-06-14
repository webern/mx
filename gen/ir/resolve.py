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


def _card(min_occurs: int, max_occurs: int | str) -> str:
    """IR cardinality of an occurrence (mirrors gen.ir.build._cardinality,
    which works in pre-lowering XSD space)."""
    if max_occurs == ir.UNBOUNDED or max_occurs > 1:
        return "vector"
    return "optional" if min_occurs == 0 else "required"


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

    # ----- the field view (content projected onto ordered fields) ----------- #
    #
    # The grammar-preserving counterpart of flat_elements: the content tree
    # normalized so that every top-level item is one *field* a structural
    # emitter renders -- an element, a structural group reference, a choice,
    # or a repeated/optional anonymous sequence. Trivial single-element
    # groups are spliced inline (they add no structure, only an occurrence
    # wrapper); structural groups stay referenced so targets can emit them
    # as shared types. All of this is neutral schema
    # reasoning -- nothing here knows what any target does with the view.

    @staticmethod
    def occurs_product(
        a_min: int, a_max: int | str, b_min: int, b_max: int | str
    ) -> tuple[int, int | str]:
        """The effective occurrence of `b` nested under a singleton wrapper
        occurring `a` times."""
        if a_max == ir.UNBOUNDED or b_max == ir.UNBOUNDED:
            max_: int | str = ir.UNBOUNDED
        else:
            max_ = a_max * b_max
        return a_min * b_min, max_

    def trivial_group(self, name: str) -> ir.Element | None:
        """The single element occurrence a *trivial* group contributes
        (a group whose whole normalized content is one element), or None
        when the group is structural and keeps its identity."""
        g = self._groups.get(name)
        if g is None:
            return None
        norm = self.normalized(g.content)
        return norm if isinstance(norm, ir.Element) else None

    def group_shape(self, name: str) -> tuple[str, ir.Particle]:
        """How a structural group presents as a type: ("choice", node) when
        its normalized content collapses to a single choice (the type IS the
        choice; reference sites absorb the choice's own occurrence), else
        ("group", node) with the normalized content."""
        g = self._groups[name]
        norm = self.normalized(g.content)
        if isinstance(norm, ir.Choice):
            return "choice", norm
        return "group", norm

    def normalized(self, p: ir.Particle | None) -> ir.Particle | None:
        """The field-view normalization: trivial groups spliced inline,
        exactly-once singleton wrappers collapsed (with occurrence bounds
        multiplied through), nested exactly-once sequences inlined into
        their parent item list. Structural group references are preserved."""
        if p is None:
            return None
        return self._norm(p)

    def _norm(self, p: ir.Particle) -> ir.Particle:
        if isinstance(p, ir.Element):
            return p
        if isinstance(p, ir.GroupRef):
            e = self.trivial_group(p.name)
            if e is None:
                return p
            mn, mx = self.occurs_product(p.min, p.max, e.min, e.max)
            return ir.Element(e.name, e.type, _card(mn, mx), mn, mx, e.doc)
        items = [self._norm(i) for i in p.items]
        if isinstance(p, ir.Sequence):
            flat: list = []
            for i in items:
                # An exactly-once sequence nested in a sequence is the same
                # grammar with the parentheses removed.
                if isinstance(i, ir.Sequence) and i.min == 1 and i.max == 1:
                    flat.extend(i.items)
                else:
                    flat.append(i)
            items = flat
            if len(items) == 1:
                return self._collapse_singleton(items[0], p.min, p.max)
            return ir.Sequence(items, p.min, p.max)
        # Choice
        if len(items) == 1:
            return self._collapse_singleton(items[0], p.min, p.max)
        return ir.Choice(items, p.min, p.max)

    def _collapse_singleton(
        self, item: ir.Particle, w_min: int, w_max: int | str
    ) -> ir.Particle:
        """A wrapper holding exactly one item is that item with the wrapper's
        occurrence multiplied through."""
        mn, mx = self.occurs_product(w_min, w_max, item.min, item.max)
        if isinstance(item, ir.Element):
            return ir.Element(item.name, item.type, _card(mn, mx), mn, mx, item.doc)
        if isinstance(item, ir.GroupRef):
            return ir.GroupRef(item.name, mn, mx)
        if isinstance(item, ir.Sequence):
            return ir.Sequence(item.items, mn, mx)
        return ir.Choice(item.items, mn, mx)

    def field_nodes(self, content: ir.Particle | None) -> list[ir.Particle]:
        """The ordered top-level field nodes of a content tree: the items of
        the normalized exactly-once top sequence, or the single normalized
        node itself. Each is an Element, a structural GroupRef, a Choice, or
        a repeated/optional Sequence."""
        top = self.normalized(content)
        if top is None:
            return []
        if isinstance(top, ir.Sequence) and top.min == 1 and top.max == 1:
            return list(top.items)
        return [top]

    def nullable(self, p: ir.Particle) -> bool:
        """True when the particle can match the empty element sequence."""
        if p.min == 0:
            return True
        if isinstance(p, ir.Element):
            return False
        if isinstance(p, ir.GroupRef):
            g = self._groups.get(p.name)
            return g is not None and self.nullable(g.content)
        if isinstance(p, ir.Sequence):
            return all(self.nullable(i) for i in p.items)
        return any(self.nullable(i) for i in p.items)  # Choice

    def first_names(self, p: ir.Particle) -> list[str]:
        """The element names that can begin an instance of the particle, in
        document order, deduped -- the dispatch set a strict in-order parser
        tests an incoming element name against."""
        out: list[str] = []
        self._first(p, out)
        return out

    def _first(self, p: ir.Particle, out: list[str]) -> None:
        def add(name: str) -> None:
            if name not in out:
                out.append(name)

        if isinstance(p, ir.Element):
            add(p.name)
        elif isinstance(p, ir.GroupRef):
            g = self._groups.get(p.name)
            if g is not None:
                self._first(g.content, out)
        elif isinstance(p, ir.Choice):
            for i in p.items:
                self._first(i, out)
        else:  # Sequence: items up to and including the first non-nullable
            for i in p.items:
                self._first(i, out)
                if not self.nullable(i):
                    break

    def particle_element_names(self, p: ir.Particle | None) -> list[str]:
        """Every element name anywhere inside the particle (groups resolved),
        in document order, deduped -- the 'known names' set an emitter uses
        to tell a misplaced element from an unknown one."""
        out: list[str] = []
        self._names(p, out, ())
        return out

    def _names(self, p, out: list[str], path: tuple[str, ...]) -> None:
        if p is None:
            return
        if isinstance(p, ir.Element):
            if p.name not in out:
                out.append(p.name)
        elif isinstance(p, ir.GroupRef):
            g = self._groups.get(p.name)
            if g is not None and p.name not in path:
                self._names(g.content, out, path + (p.name,))
        else:
            for i in p.items:
                self._names(i, out, path)

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
