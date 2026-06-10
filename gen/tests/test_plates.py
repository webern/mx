"""Tests for the Plates projection (gen.plates) and its config surface.

Covers the four pillars of the design (docs/ai/design/plates.md):

  - the name-convention model: tokenizer, recasing, acronyms, sanitization
    (the worked conversion table from design section 5.3 is asserted row by
    row);
  - the override system: both tiers, the addressing scheme, precedence, and
    fail-loud validation of stale keys;
  - collision detection across the projected scopes;
  - the projection itself, built against the real target configs so the
    shipped configurations stay green.

Run with: python3 -m unittest gen.tests.test_plates
"""

from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from gen import config as cfg
from gen.config import Config, ConfigError, RenameEntry
from gen.ir import model as ir
from gen.ir.build import build_ir
from gen.ir.dump import to_json
from gen.plates import PlatesError, build_plates
from gen.plates.names import NameFactory, sanitize_identifier, tokenize
from gen.xsd import parse

REPO = Path(__file__).resolve().parents[2]
GO_CONFIG = REPO / "gen" / "test" / "go" / "config.toml"
C_CONFIG = REPO / "gen" / "test" / "c" / "config.toml"
CPP_CONFIG = REPO / "gen" / "cpp" / "config.toml"


def build_for(config_path: Path):
    config = cfg.load(config_path)
    m = build_ir(parse(config.xsd), source=config.xsd.stem)
    if config.sounds_xml is not None:
        from gen.ir.sounds import patch_sounds, read_sound_ids

        patch_sounds(m, read_sound_ids(config.sounds_xml))
    return build_plates(m, config), config


class TokenizerAndConventions(unittest.TestCase):
    """The worked conversion table from design section 5.3."""

    TABLE = [
        # wire, words, pascal, camel, snake, kebab, screaming
        ("note", ("note",), "Note", "note", "note", "note", "NOTE"),
        ("default-x", ("default", "x"), "DefaultX", "defaultX", "default_x",
         "default-x", "DEFAULT_X"),
        ("clef-octave-change", ("clef", "octave", "change"), "ClefOctaveChange",
         "clefOctaveChange", "clef_octave_change", "clef-octave-change",
         "CLEF_OCTAVE_CHANGE"),
        ("midi-channel", ("midi", "channel"), "MIDIChannel", "midiChannel",
         "midi_channel", "midi-channel", "MIDI_CHANNEL"),
        ("optional-unique-id", ("optional", "unique", "id"), "OptionalUniqueID",
         "optionalUniqueID", "optional_unique_id", "optional-unique-id",
         "OPTIONAL_UNIQUE_ID"),
        ("brass.alphorn", ("brass", "alphorn"), "BrassAlphorn", "brassAlphorn",
         "brass_alphorn", "brass-alphorn", "BRASS_ALPHORN"),
        ("up down", ("up", "down"), "UpDown", "upDown", "up_down", "up-down",
         "UP_DOWN"),
        ("1024th", ("1024th",), "1024th", "1024th", "1024th", "1024th", "1024TH"),
        ("", ("empty",), "Empty", "empty", "empty", "empty", "EMPTY"),
    ]

    def test_worked_table(self):
        factory = NameFactory()
        for wire, words, pascal, camel, snake, kebab, screaming in self.TABLE:
            with self.subTest(wire=wire):
                name = factory.make(wire)
                self.assertEqual(name.words, words)
                self.assertEqual(name.pascal, pascal)
                self.assertEqual(name.camel, camel)
                self.assertEqual(name.snake, snake)
                self.assertEqual(name.kebab, kebab)
                self.assertEqual(name.screaming, screaming)

    def test_case_transition_splits(self):
        self.assertEqual(tokenize("fooBar"), ("foo", "bar"))
        self.assertEqual(tokenize("MIDIChannel"), ("midi", "channel"))
        self.assertEqual(tokenize("xml:lang"), ("xml", "lang"))
        self.assertEqual(tokenize("midi-128"), ("midi", "128"))

    def test_leading_camel_acronym_is_lowercased(self):
        # The camelCase leading word is always fully lowercased, even when it
        # is in the acronym set.
        name = NameFactory().make("midi-channel")
        self.assertEqual(name.camel, "midiChannel")

    def test_pluralize_appends_to_last_word(self):
        name = NameFactory().make("midi-channel", pluralize=True)
        self.assertEqual(name.pascal, "MIDIChannels")
        self.assertEqual(name.wire, "midi-channel")


class Sanitizer(unittest.TestCase):
    def test_reserved_word_gets_suffix(self):
        self.assertEqual(sanitize_identifier("class", frozenset({"class"})), "class_")

    def test_leading_digit_gets_prefix(self):
        self.assertEqual(sanitize_identifier("1024th", frozenset()), "_1024th")

    def test_empty_gets_prefix(self):
        self.assertEqual(sanitize_identifier("", frozenset()), "_")

    def test_non_identifier_chars_become_underscores(self):
        self.assertEqual(sanitize_identifier("a-b", frozenset()), "a_b")

    def test_clean_identifier_unchanged(self):
        self.assertEqual(sanitize_identifier("Note", frozenset()), "Note")


def tiny_ir() -> ir.Ir:
    """A minimal IR exercising renames, defaults, and collisions without
    parsing a schema."""
    up_down = ir.EnumType("up-down", "token", ["up", "down"])
    pitch = ir.ComplexType("pitch", "empty", presence_only=True, deps=[])
    note = ir.ComplexType(
        "note",
        "composite",
        attributes=[
            ir.Attr("type", ir.Ref("up-down", "value"), default="up"),
            ir.Attr("default-x", ir.Ref("decimal", "primitive")),
        ],
        content=ir.Sequence(
            [
                ir.Element("pitch", ir.Ref("pitch", "complex"), "required"),
                ir.Element("tie", ir.Ref("up-down", "value"), "vector", 0, ir.UNBOUNDED),
            ]
        ),
        deps=["pitch"],
    )
    return ir.Ir(
        source="tiny",
        builtins={},
        value_types=[up_down],
        groups=[],
        attribute_groups=[],
        complex_types=[pitch, note],
        roots=[ir.Root("note", "note")],
        dropped_dead=[],
        stats={},
    )


class Overrides(unittest.TestCase):
    def test_fundamental_rename_reexpands_every_convention(self):
        config = Config()
        config.renames.types["note"] = RenameEntry(fundamental="music-note")
        plates = build_plates(tiny_ir(), config)
        plate = plates.plate("note")
        self.assertEqual(plate.name.wire, "note")  # wire form untouched
        self.assertEqual(plate.name.pascal, "MusicNote")
        self.assertEqual(plate.name.snake, "music_note")

    def test_per_convention_override_pins_one_flavor(self):
        config = Config()
        config.renames.types["note"] = RenameEntry(cased={"pascal": "MusicNote"})
        plates = build_plates(tiny_ir(), config)
        plate = plates.plate("note")
        self.assertEqual(plate.name.pascal, "MusicNote")
        self.assertEqual(plate.name.snake, "note")  # others still auto-expand

    def test_scoped_attribute_rename_beats_global(self):
        config = Config()
        config.renames.attributes["type"] = RenameEntry(fundamental="global-kind")
        config.renames.scoped_attributes[("note", "type")] = RenameEntry(
            fundamental="note-kind"
        )
        plates = build_plates(tiny_ir(), config)
        members = {m.name.wire: m for m in plates.plate("note").members}
        self.assertEqual(members["type"].name.snake, "note_kind")

    def test_enum_value_rename_is_scoped_to_its_enum(self):
        config = Config()
        config.renames.enum_values[("up-down", "up")] = RenameEntry(fundamental="upward")
        plates = build_plates(tiny_ir(), config)
        enum = plates.plate("up-down")
        self.assertEqual(enum.variants[0].wire, "up")
        self.assertEqual(enum.variants[0].ident, "Upward")
        self.assertEqual(enum.variants[1].ident, "Down")

    def test_stale_rename_keys_fail_loud(self):
        config = Config()
        config.renames.types["no-such-type"] = RenameEntry(fundamental="x")
        config.renames.elements["no-such-element"] = RenameEntry(fundamental="x")
        config.renames.attributes["no-such-attr"] = RenameEntry(fundamental="x")
        config.renames.scoped_attributes[("note", "no-such")] = RenameEntry(fundamental="x")
        config.renames.enum_values[("up-down", "sideways")] = RenameEntry(fundamental="x")
        with self.assertRaises(PlatesError) as caught:
            build_plates(tiny_ir(), config)
        text = "\n".join(caught.exception.errors)
        for key in ("no-such-type", "no-such-element", "no-such-attr",
                    "note.no-such", "sideways"):
            self.assertIn(key, text)


class Projection(unittest.TestCase):
    def test_default_naming_an_enum_variant_resolves(self):
        plates = build_plates(tiny_ir(), Config())
        members = {m.name.wire: m for m in plates.plate("note").members}
        self.assertEqual(members["type"].default, "up")
        self.assertEqual(members["type"].default_variant, "Up")
        self.assertIsNone(members["default-x"].default_variant)

    def test_member_order_attributes_then_elements(self):
        plates = build_plates(tiny_ir(), Config())
        kinds = [m.kind for m in plates.plate("note").members]
        self.assertEqual(kinds, ["attribute", "attribute", "element", "element"])

    def test_presence_only_empty_projects_to_flag(self):
        plates = build_plates(tiny_ir(), Config())
        self.assertEqual(plates.plate("pitch").strategy, "flag")

    def test_vector_cardinality_survives(self):
        plates = build_plates(tiny_ir(), Config())
        members = {m.name.wire: m for m in plates.plate("note").members}
        self.assertEqual(members["tie"].cardinality, "vector")
        self.assertEqual(members["pitch"].cardinality, "required")

    def test_collision_detection_reports_induced_collisions(self):
        config = Config()
        # Force the enum's two variants onto one identifier.
        config.renames.enum_values[("up-down", "down")] = RenameEntry(fundamental="up")
        with self.assertRaises(PlatesError) as caught:
            build_plates(tiny_ir(), config)
        self.assertIn("variant identifier collision", caught.exception.errors[0])

    def test_unmapped_language_passes_primitives_through(self):
        plates = build_plates(tiny_ir(), Config())
        members = {m.name.wire: m for m in plates.plate("note").members}
        self.assertEqual(members["default-x"].type_ref.ident, "decimal")


class RealTargets(unittest.TestCase):
    """The shipped configs must project cleanly, deterministically, and with
    the spot-checkable facts the emitters will lean on."""

    @classmethod
    def setUpClass(cls):
        cls.go, cls.go_config = build_for(GO_CONFIG)
        cls.c, cls.c_config = build_for(C_CONFIG)
        cls.cpp, cls.cpp_config = build_for(CPP_CONFIG)

    def test_determinism(self):
        again, _ = build_for(GO_CONFIG)
        self.assertEqual(to_json(self.go), to_json(again))

    def test_go_idents_are_exported_pascal(self):
        plate = self.go.plate("midi-instrument")
        self.assertEqual(plate.ident, "MIDIInstrument")
        self.assertEqual(plate.file, "midi_instrument")

    def test_c_idents_carry_prefix(self):
        plate = self.c.plate("midi-instrument")
        self.assertEqual(plate.ident, "MxMIDIInstrument")
        self.assertEqual(plate.file, "mx_midi_instrument")

    def test_shared_base_resolves_barline_collision(self):
        for plates in (self.go, self.c, self.cpp):
            members = plates.plate("barline").members
            idents = [m.ident for m in members]
            self.assertEqual(len(idents), len(set(idents)))
            # Both the attribute and the element keep the wire name "segno";
            # only their identifiers diverge (the shared base renames the
            # attribute's fundamental root).
            kinds = {m.kind for m in members if m.name.wire == "segno"}
            self.assertEqual(kinds, {"attribute", "element"})

    def test_empty_enum_value_gets_fallback_identifier(self):
        enum = self.go.plate("breath-mark-value")
        empty = next(v for v in enum.variants if v.wire == "")
        self.assertEqual(empty.ident, "Empty")

    def test_digit_led_variant_sanitized_but_casing_kept(self):
        enum = self.go.plate("note-type-value")
        v1024 = next(v for v in enum.variants if v.wire == "1024th")
        self.assertEqual(v1024.name.pascal, "1024th")  # the ideal is recorded
        self.assertEqual(v1024.ident, "_1024th")  # the sanitized result

    def test_default_variant_resolution_in_real_schema(self):
        plate = self.go.plate("strong-accent")
        member = next(m for m in plate.members if m.name.wire == "type")
        self.assertEqual(member.default, "up")
        self.assertEqual(member.default_variant, "Up")

    def test_sound_id_fold_present_only_with_sounds(self):
        self.assertTrue(self.c.has_plate("sound-id"))
        self.assertFalse(self.go.has_plate("sound-id"))
        union = self.c.plate("instrument-sound")
        kinds = [(m.ref.category if m.ref else "literals") for m in union.members]
        self.assertEqual(kinds, ["value", "primitive"])  # enum + open string

    def test_files_unique_and_self_excluded(self):
        for plates in (self.go, self.c, self.cpp):
            stems = [f.file for f in plates.files]
            self.assertEqual(len(stems), len(set(stems)))
            for spec in plates.files:
                self.assertNotIn(spec.file, spec.includes)

    def test_derived_plates_expose_both_views(self):
        derived = [p for p in self.c.complex_types if p.shape == "derived"]
        self.assertTrue(derived)
        for plate in derived:
            self.assertEqual(plate.strategy, "flatten")  # C: inheritance = false
            self.assertIsNotNone(plate.base)
            self.assertIsNotNone(plate.all_members)
            self.assertGreaterEqual(len(plate.all_members), len(plate.members))
        go_derived = [p for p in self.go.complex_types if p.shape == "derived"]
        self.assertTrue(all(p.strategy == "inherit" for p in go_derived))

    def test_neutral_core_keeps_wire_facts(self):
        # What a JSON Schema target would read: wire names, enum literals,
        # the resolved content tree -- all present regardless of binding.
        enum = self.go.plate("step")
        self.assertIn("A", [v.wire for v in enum.variants])
        note = self.go.plate("note")
        self.assertIsNotNone(note.content)
        self.assertEqual(note.name.wire, "note")


class ConfigParsing(unittest.TestCase):
    def _load(self, text: str) -> Config:
        with tempfile.TemporaryDirectory() as d:
            path = Path(d) / "config.toml"
            path.write_text(text)
            return cfg.load(path)

    def test_rename_tables_parse(self):
        config = self._load(
            """
            [rename.type.attributes]
            fundamental = "properties"
            [rename.type.note]
            pascal = "MusicNote"
            [rename.attribute]
            default-x = "origin-x"
            [rename.attribute.note]
            type = "kind"
            [rename.enum-value.up-down]
            "up" = "upward"
            "" = "none"
            """
        )
        self.assertEqual(config.renames.types["attributes"].fundamental, "properties")
        self.assertEqual(config.renames.types["note"].cased, {"pascal": "MusicNote"})
        self.assertEqual(config.renames.attributes["default-x"].fundamental, "origin-x")
        self.assertEqual(
            config.renames.scoped_attributes[("note", "type")].fundamental, "kind"
        )
        self.assertEqual(
            config.renames.enum_values[("up-down", "")].fundamental, "none"
        )

    def test_unknown_keys_fail(self):
        with self.assertRaises(ConfigError):
            self._load("[naming]\nconventions = ['pascal']\n")
        with self.assertRaises(ConfigError):
            self._load("[rename.kind.x]\nfundamental = 'y'\n")
        with self.assertRaises(ConfigError):
            self._load("[rename.type.note]\npasta = 'MusicNote'\n")

    def test_unsupported_values_fail(self):
        with self.assertRaises(ConfigError):
            self._load("[layout]\npartition = 'sharded'\n")
        with self.assertRaises(ConfigError):
            self._load("[reserved]\npolicy = 'rename'\n")
        with self.assertRaises(ConfigError):
            self._load("[naming]\ntype-convention = 'dot'\n")

    def test_extends_merges_with_target_precedence(self):
        with tempfile.TemporaryDirectory() as d:
            base = Path(d) / "base.toml"
            base.write_text(
                """
                [naming]
                acronyms = ["midi"]
                [rename.type.note]
                fundamental = "base-note"
                [rename.type.pitch]
                fundamental = "base-pitch"
                [rename.enum-value.up-down]
                "up" = "base-up"
                "down" = "base-down"
                """
            )
            target = Path(d) / "config.toml"
            target.write_text(
                """
                [naming]
                extends = "base.toml"
                [rename.type.note]
                fundamental = "target-note"
                [rename.enum-value.up-down]
                "up" = "target-up"
                """
            )
            config = cfg.load(target)
        self.assertEqual(config.naming.acronyms, ("midi",))
        self.assertEqual(config.renames.types["note"].fundamental, "target-note")
        self.assertEqual(config.renames.types["pitch"].fundamental, "base-pitch")
        self.assertEqual(
            config.renames.enum_values[("up-down", "up")].fundamental, "target-up"
        )
        self.assertEqual(
            config.renames.enum_values[("up-down", "down")].fundamental, "base-down"
        )


if __name__ == "__main__":
    unittest.main()
