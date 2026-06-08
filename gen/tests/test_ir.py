"""Integrity and invariant tests for the XSD parser and the IR.

Most tests run against every MusicXML version in docs/, so the assumptions the
generator leans on are guarded for any schema it might be pointed at:

  - the complex-type graph is acyclic (lets the emitter use value types with no
    indirection and emit in dependency order);
  - element names do not collide (lets dispatch be a flat name -> type table);
  - every reference in the lowered IR resolves, in dependency order.

Run with: python3 -m unittest gen.tests.test_ir
"""

from __future__ import annotations

import sys
import unittest
from pathlib import Path

from gen.ir import build_ir
from gen.ir import model as ir
from gen.xsd import parse
from gen.xsd.analyze import element_index, type_graph

DOCS = Path(__file__).resolve().parents[2] / "docs"
XSDS = sorted(DOCS.glob("musicxml-*.xsd"))
XSD_40 = DOCS / "musicxml-4.0-ed15c23.xsd"


def _find_cycle(graph: dict[str, set[str]]) -> list[str]:
    """Independent DFS cycle finder (does not reuse the production SCC code).
    Returns a cycle as a node path, or [] if the graph is acyclic."""
    WHITE, GREY, BLACK = 0, 1, 2
    color = {n: WHITE for n in graph}
    sys.setrecursionlimit(10000)

    def dfs(node: str, path: list[str]) -> list[str]:
        color[node] = GREY
        for nxt in graph[node]:
            if color[nxt] == GREY:
                return path[path.index(nxt):] + [nxt]
            if color[nxt] == WHITE:
                found = dfs(nxt, path + [nxt])
                if found:
                    return found
        color[node] = BLACK
        return []

    for n in graph:
        if color[n] == WHITE:
            cycle = dfs(n, [n])
            if cycle:
                return cycle
    return []


class SchemaInvariants(unittest.TestCase):
    def test_xsds_present(self):
        self.assertTrue(XSDS, f"no MusicXML XSDs found in {DOCS}")

    def test_acyclic_type_graph(self):
        for xsd in XSDS:
            with self.subTest(xsd=xsd.name):
                cycle = _find_cycle(type_graph(parse(xsd)))
                self.assertEqual(cycle, [], f"cycle: {' -> '.join(cycle)}")

    def test_no_element_name_collisions(self):
        for xsd in XSDS:
            with self.subTest(xsd=xsd.name):
                index = element_index(parse(xsd))
                collisions = {n: sorted(set(t)) for n, t in index.items() if len(set(t)) > 1}
                self.assertEqual(collisions, {}, f"name collisions: {collisions}")


class IRIntegrity(unittest.TestCase):
    def test_references_resolve(self):
        for xsd in XSDS:
            with self.subTest(xsd=xsd.name):
                self._check_references(build_ir(parse(xsd), xsd.stem))

    def test_dependency_order(self):
        """Both type lists are emitted deps-first: a dependency never follows
        the type that uses it."""
        for xsd in XSDS:
            with self.subTest(xsd=xsd.name):
                m = build_ir(parse(xsd), xsd.stem)
                cpos = {c.name: i for i, c in enumerate(m.complex_types)}
                for c in m.complex_types:
                    for dep in c.deps:
                        self.assertLess(cpos[dep], cpos[c.name], f"{dep} after {c.name}")
                vpos = {v.name: i for i, v in enumerate(m.value_types)}
                for v in m.value_types:
                    if isinstance(v, ir.UnionType):
                        for mem in v.members:
                            if mem.category == "value" and mem.type in vpos:
                                self.assertLess(
                                    vpos[mem.type], vpos[v.name],
                                    f"union {v.name} member {mem.type} out of order",
                                )

    def _check_references(self, m: ir.Ir) -> None:
        complex_names = {c.name for c in m.complex_types}
        value_names = {v.name for v in m.value_types}
        group_names = {g.name for g in m.groups}
        ag_names = {a.name for a in m.attribute_groups}

        def check_ref(ref: ir.Ref, where: str) -> None:
            if ref.category == "complex":
                self.assertIn(ref.name, complex_names, f"{where}: missing complex {ref.name}")
            elif ref.category == "value":
                self.assertIn(ref.name, value_names, f"{where}: missing value {ref.name}")

        def walk(node, where: str) -> None:
            if isinstance(node, (ir.Sequence, ir.Choice)):
                for item in node.items:
                    walk(item, where)
            elif isinstance(node, ir.GroupRef):
                self.assertIn(node.name, group_names, f"{where}: missing group {node.name}")
            elif isinstance(node, ir.Element):
                check_ref(node.type, f"{where} element {node.name}")

        for c in m.complex_types:
            for a in c.attributes:
                check_ref(a.type, f"{c.name} attr {a.name}")
            for ref in c.attribute_groups:
                self.assertIn(ref, ag_names, f"{c.name}: missing attribute group {ref}")
            if c.value_type:
                check_ref(c.value_type, f"{c.name} value")
            if c.base:
                self.assertIn(c.base, complex_names, f"{c.name}: missing base {c.base}")
            if c.content:
                walk(c.content, c.name)
            for dep in c.deps:
                self.assertIn(dep, complex_names, f"{c.name}: dep {dep} not a complex type")

        for ag in m.attribute_groups:
            for ref in ag.attribute_groups:
                self.assertIn(ref, ag_names, f"attribute group {ag.name}: missing {ref}")
            for a in ag.attributes:
                check_ref(a.type, f"attribute group {ag.name} attr {a.name}")
        for g in m.groups:
            walk(g.content, f"group {g.name}")


@unittest.skipUnless(XSD_40.exists(), "MusicXML 4.0 XSD not present")
class DeadTypeRegression(unittest.TestCase):
    """Guards the reachability analysis against the bug where a type used only
    via an inline type's attribute group was wrongly dropped."""

    DEAD = {
        "empty-print-style",
        "empty-print-style-align",
        "formatted-symbol",
        "positive-decimal",
        "start-stop-change-continue",
    }

    def test_dead_dropped_live_kept(self):
        m = build_ir(parse(XSD_40), "musicxml-4.0")
        names = {c.name for c in m.complex_types} | {v.name for v in m.value_types}
        self.assertEqual(set(m.dropped_dead), self.DEAD)
        self.assertFalse(self.DEAD & names, "dead types leaked into the IR")
        # measure-text is reachable via measure-attributes/@text, not dead.
        self.assertIn("measure-text", names)


if __name__ == "__main__":
    unittest.main()
