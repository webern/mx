"""The JSON Schema target: the proof of the cardinal rule.

This target is a config and one template -- no Python anywhere -- and it
consumes only the plates' neutral core (plates.md section 9): wire names,
enum literals, bounds, patterns, the open-enum, docs. These tests render it
through the same pipeline as any target and assert the neutral facts landed.

Run with: python3 -m unittest gen.tests.test_schema
"""

from __future__ import annotations

import json
import unittest
from pathlib import Path

from gen.plates import build_for_config
from gen.press.render import render_files

CONFIG = Path(__file__).resolve().parents[1] / "schema" / "config.toml"


class SchemaTarget(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        plates, cfg = build_for_config(CONFIG)
        files = render_files(plates, cfg)
        cls.schema = json.loads(files["musicxml.schema.json"])

    def test_defs_are_wire_names(self):
        defs = self.schema["$defs"]
        self.assertIn("note", defs)
        self.assertIn("score-partwise", defs)
        self.assertIn("above-below", defs)  # kebab wire name, not a casing

    def test_enum_carries_wire_literals(self):
        self.assertEqual(
            self.schema["$defs"]["step"]["enum"],
            ["A", "B", "C", "D", "E", "F", "G"],
        )
        # Space-separated and empty literals survive verbatim.
        self.assertIn("backward hook", self.schema["$defs"]["beam-value"]["enum"])
        self.assertIn("", self.schema["$defs"]["breath-mark-value"]["enum"])

    def test_number_bounds(self):
        midi16 = self.schema["$defs"]["midi-16"]
        self.assertEqual(midi16["minimum"], 1)
        self.assertEqual(midi16["maximum"], 16)
        divisions = self.schema["$defs"]["positive-divisions"]
        self.assertEqual(divisions["exclusiveMinimum"], 0)

    def test_open_enum_is_anyof_with_open_string(self):
        union = self.schema["$defs"]["instrument-sound"]["anyOf"]
        self.assertEqual(union[0], {"$ref": "#/$defs/sound-id"})
        self.assertEqual(union[1], {"type": "string"})

    def test_complex_properties_are_wire_names(self):
        note = self.schema["$defs"]["note"]["properties"]
        self.assertIn("default-x", note)  # attribute, kebab wire form
        self.assertIn("pitch", note)
        # A repeatable child is an array.
        self.assertEqual(note["beam"]["type"], "array")

    def test_roots(self):
        self.assertEqual(
            self.schema["oneOf"],
            [
                {"$ref": "#/$defs/score-partwise"},
                {"$ref": "#/$defs/score-timewise"},
            ],
        )


if __name__ == "__main__":
    unittest.main()
