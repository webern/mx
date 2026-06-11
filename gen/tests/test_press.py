"""Tests for the press (gen.press.engine).

Two layers:

  1. Spec conformance: every case of the five vendored core modules of the
     official Mustache spec suite (gen/tests/mustache_spec/). The suite is
     run with the spec's own semantics (lenient misses, HTML escaping) via
     the engine's test-affordance knobs, so the resolution, section,
     standalone-whitespace, set-delimiter, and partial-indentation machinery
     is pinned to the spec with zero skips.
  2. The three documented deviations, under production defaults: missing
     keys raise with template:line, interpolation is verbatim, callables are
     refused.

Run with: python3 -m unittest gen.tests.test_press
"""

from __future__ import annotations

import json
import unittest
from pathlib import Path

from gen.press import Press, PressError

SPEC_DIR = Path(__file__).resolve().parent / "mustache_spec"
SPEC_MODULES = ("interpolation", "sections", "inverted", "partials", "comments")


def _html_escape(s: str) -> str:
    """The escape repertoire the spec suite expects."""
    return (
        s.replace("&", "&amp;")
        .replace("<", "&lt;")
        .replace(">", "&gt;")
        .replace('"', "&quot;")
    )


class SpecConformance(unittest.TestCase):
    def test_spec_suite(self):
        total = 0
        for module in SPEC_MODULES:
            cases = json.loads((SPEC_DIR / f"{module}.json").read_text())["tests"]
            self.assertTrue(cases)
            for case in cases:
                with self.subTest(module=module, name=case["name"]):
                    press = Press(
                        partials=case.get("partials", {}),
                        strict=False,
                        escape=_html_escape,
                    )
                    got = press.render(case["template"], case["data"], case["name"])
                    self.assertEqual(got, case["expected"], case["desc"])
                total += 1
        self.assertGreaterEqual(total, 5)


class Deviations(unittest.TestCase):
    """Production semantics: the three deliberate departures from spec."""

    def test_missing_key_is_an_error_with_location(self):
        with self.assertRaises(PressError) as caught:
            Press().render("line one\n{{indent}}", {"ident": "x"}, "t.tmpl")
        self.assertIn("t.tmpl:2", str(caught.exception))
        self.assertIn("indent", str(caught.exception))

    def test_missing_section_key_is_an_error(self):
        with self.assertRaises(PressError):
            Press().render("{{#nope}}x{{/nope}}", {})

    def test_present_but_none_renders_empty_and_is_falsey(self):
        press = Press()
        self.assertEqual(press.render("[{{doc}}]", {"doc": None}), "[]")
        self.assertEqual(
            press.render("{{^doc}}none{{/doc}}", {"doc": None}), "none"
        )

    def test_no_html_escaping_by_default(self):
        out = Press().render("{{code}}", {"code": 'if (a < b && c) { s = "x"; }'})
        self.assertEqual(out, 'if (a < b && c) { s = "x"; }')

    def test_lambdas_are_refused(self):
        with self.assertRaises(PressError) as caught:
            Press().render("{{fn}}", {"fn": lambda: "no"})
        self.assertIn("lambdas", str(caught.exception))

    def test_missing_partial_is_an_error(self):
        with self.assertRaises(PressError):
            Press(partials={}).render("{{> nope}}", {})


class Robustness(unittest.TestCase):
    def test_unclosed_section_reports_open_line(self):
        with self.assertRaises(PressError) as caught:
            Press().render("a\n{{#list}}\nb", {"list": [1]}, "t.tmpl")
        self.assertIn("t.tmpl:2", str(caught.exception))
        self.assertIn("unclosed section", str(caught.exception))

    def test_mismatched_close_is_an_error(self):
        with self.assertRaises(PressError):
            Press().render("{{#a}}{{/b}}", {"a": True, "b": True})

    def test_partial_recursion_is_depth_limited(self):
        press = Press(partials={"loop": "{{> loop}}"})
        with self.assertRaises(PressError) as caught:
            press.render("{{> loop}}", {})
        self.assertIn("recursion", str(caught.exception))

    def test_callable_partial_loader(self):
        loader = {"greet": "hi {{name}}"}.__getitem__
        out = Press(partials=loader).render("{{> greet}}", {"name": "press"})
        self.assertEqual(out, "hi press")

    def test_booleans_interpolate_lowercase(self):
        self.assertEqual(Press().render("{{f}}", {"f": True}), "true")

    def test_context_stack_lookup(self):
        # An inner frame's miss falls through to outer frames (spec
        # resolution), which is how templates reach plate fields from inside
        # member loops.
        out = Press().render(
            "{{#members}}{{type}}.{{ident}};{{/members}}",
            {"type": "Note", "members": [{"ident": "a"}, {"ident": "b"}]},
        )
        self.assertEqual(out, "Note.a;Note.b;")


if __name__ == "__main__":
    unittest.main()
