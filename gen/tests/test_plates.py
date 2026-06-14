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
from gen.names import NameFactory, sanitize_identifier, tokenize
from gen.plates import PlatesError, build_for_config, build_plates
from gen.plates.build import portable_pattern
from gen.xsd import parse

REPO = Path(__file__).resolve().parents[2]
GO_CONFIG = REPO / "gen" / "test" / "go" / "config.toml"
C_CONFIG = REPO / "gen" / "test" / "c" / "config.toml"
CPP_CONFIG = REPO / "gen" / "cpp" / "config.toml"


def build_for(config_path: Path):
    # The CLI and the tests share one pipeline so they cannot drift.
    return build_for_config(config_path)


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

    def test_element_rename_applies_to_members(self):
        config = Config()
        config.renames.elements["tie"] = RenameEntry(fundamental="tie-mark")
        plates = build_plates(tiny_ir(), config)
        member = plates.plate("note").member("tie", kind="element")
        self.assertEqual(member.name.snake, "tie_mark")
        self.assertEqual(member.name.wire, "tie")

    def test_composed_scope_builds_flat_namespace_constants(self):
        config = Config()
        config.target.variant_scope = "composed"
        config.naming.field_convention = "pascal"
        config.naming.variant_convention = "pascal"
        plates = build_plates(tiny_ir(), config)
        enum = plates.plate("up-down")
        self.assertEqual([v.ident for v in enum.variants], ["UpDownUp", "UpDownDown"])
        # A composed constant colliding with a TYPE identifier is caught: pin
        # casings so type "no" + variant "te" composes to the type ident
        # "Note" of the existing note type.
        config.renames.types["up-down"] = RenameEntry(fundamental="no")
        config.renames.enum_values[("up-down", "up")] = RenameEntry(
            cased={"pascal": "te"}
        )
        with self.assertRaises(PlatesError) as caught:
            build_plates(tiny_ir(), config)
        self.assertIn("identifier collision", caught.exception.errors[0])
        self.assertIn("Note", caught.exception.errors[0])

    def test_composed_scope_with_prefix_screams(self):
        config = Config()
        config.target.variant_scope = "composed"
        config.target.symbol_prefix = "Mx"
        config.naming.variant_convention = "screaming"
        plates = build_plates(tiny_ir(), config)
        enum = plates.plate("up-down")
        self.assertEqual(enum.variants[0].ident, "MX_UP_DOWN_UP")

    def test_unknown_types_key_fails_loud(self):
        config = Config(types={"decmial": "double"})
        with self.assertRaises(PlatesError) as caught:
            build_plates(tiny_ir(), config)
        self.assertIn("not an IR primitive", caught.exception.errors[0])

    def test_derived_all_members_built_for_inheriting_targets(self):
        base = ir.ComplexType(
            "base-type", "empty",
            attributes=[ir.Attr("color", ir.Ref("token", "primitive"))],
        )
        derived = ir.ComplexType(
            "derived-type", "derived", base="base-type",
            attributes=[ir.Attr("size", ir.Ref("token", "primitive"))],
            deps=["base-type"],
        )
        m = tiny_ir()
        m.complex_types += [base, derived]
        config = Config()  # inheritance defaults to True
        plates = build_plates(m, config)
        plate = plates.plate("derived-type")
        self.assertEqual(plate.strategy, "inherit")
        self.assertIsNotNone(plate.all_members)  # gate coverage either way
        self.assertEqual(
            [mm.name.wire for mm in plate.all_members], ["color", "size"]
        )


class ClampPolicy(unittest.TestCase):
    """The leniency policy is data on the plates (one decision, two backend
    spellings): facet bounds plus primitive-implied bounds, tightest wins,
    exclusive bounds clamping to the nearest representable in-range value."""

    def _steps(self, base, **bounds):
        from gen.plates.build import clamp_steps
        from gen.plates.model import NumberBounds

        return [
            (s.op, s.bound, s.replacement)
            for s in clamp_steps(base, NumberBounds(**bounds))
        ]

    def test_inclusive_bounds(self):
        self.assertEqual(
            self._steps("integer", min_inclusive="1", max_inclusive="16"),
            [("<", "1", "1"), (">", "16", "16")],
        )

    def test_exclusive_decimal_clamps_past_epsilon(self):
        self.assertEqual(
            self._steps("decimal", min_exclusive="0"),
            [("<=", "0.0", "1e-06")],
        )

    def test_exclusive_integer_clamps_to_next(self):
        self.assertEqual(
            self._steps("integer", min_exclusive="0", max_exclusive="10"),
            [("<=", "0", "1"), (">=", "10", "9")],
        )

    def test_exclusive_beats_inclusive_at_same_value(self):
        self.assertEqual(
            self._steps("decimal", min_inclusive="0", min_exclusive="0"),
            [("<=", "0.0", "1e-06")],
        )
        self.assertEqual(
            self._steps("decimal", max_inclusive="5", max_exclusive="5"),
            [(">=", "5.0", "4.999999")],
        )

    def test_implied_minimum_merges_with_facets(self):
        # positive_integer implies >= 1 even with a looser explicit min.
        self.assertEqual(
            self._steps("positive_integer", min_inclusive="0"),
            [("<", "1", "1")],
        )
        self.assertEqual(self._steps("non_negative_integer"), [("<", "0", "0")])
        self.assertEqual(self._steps("decimal"), [])

    def test_union_primitive_member_carries_implied_clamp(self):
        m = tiny_ir()
        m.value_types.append(
            ir.UnionType(
                "positive-or-empty",
                [
                    ir.UnionMember(ir.Ref("positive_integer", "primitive")),
                    ir.UnionMember(literals=[""]),
                ],
            )
        )
        plates = build_plates(m, Config())
        union = plates.plate("positive-or-empty")
        member = union.members[0]
        self.assertEqual(
            [(s.op, s.bound, s.replacement) for s in member.clamp],
            [("<", "1", "1")],
        )
        self.assertIsNotNone(member.tag)  # discriminator constant is final


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

    def test_c_idents_carry_prefix(self):
        plate = self.c.plate("midi-instrument")
        self.assertEqual(plate.ident, "MxMIDIInstrument")

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
        self.assertEqual(empty.ident, "BreathMarkValueEmpty")  # composed scope

    def test_variant_idents_are_final_per_scope(self):
        # Go and C compose into their flat constant namespaces; the digit-led
        # value needs no sanitizer mangling once composed.
        go_enum = self.go.plate("note-type-value")
        v1024 = next(v for v in go_enum.variants if v.wire == "1024th")
        self.assertEqual(v1024.name.pascal, "1024th")  # the ideal is recorded
        self.assertEqual(v1024.ident, "NoteTypeValue1024th")
        c_enum = self.c.plate("note-type-value")
        c1024 = next(v for v in c_enum.variants if v.wire == "1024th")
        self.assertEqual(c1024.ident, "MX_NOTE_TYPE_VALUE_1024TH")
        self.assertEqual(self.go.target.variant_scope, "composed")
        self.assertEqual(self.cpp.target.variant_scope, "bare")
        cpp_enum = self.cpp.plate("note-type-value")
        cpp1024 = next(v for v in cpp_enum.variants if v.wire == "1024th")
        self.assertEqual(cpp1024.ident, "_1024th")  # bare: sanitizer applies

    def test_default_variant_resolution_in_real_schema(self):
        plate = self.go.plate("strong-accent")
        member = next(m for m in plate.members if m.name.wire == "type")
        self.assertEqual(member.default, "up")
        self.assertEqual(member.default_variant, "UpDownUp")

    def test_flat_cardinality_in_real_schema(self):
        # metronome's beat-unit occurs on a branch spine AND inside that same
        # branch's inner choice: the occurrences co-occur, so the flat member
        # must be a vector (the corpus exercises this: beat-unit-tied).
        metronome = {m.name.wire: m for m in self.go.plate("metronome").members}
        self.assertEqual(metronome["beat-unit"].cardinality, "vector")
        # pitch's spine is exactly-once sequences: step stays required.
        pitch = {m.name.wire: m for m in self.go.plate("pitch").members}
        self.assertEqual(pitch["step"].cardinality, "required")
        # note's cue occurs in two exclusive branches of one choice.
        note = {m.name.wire: m for m in self.go.plate("note").members}
        self.assertEqual(note["cue"].cardinality, "optional")
        self.assertEqual(note["tie"].cardinality, "vector")  # maxOccurs=2

    def test_sound_id_fold_present_only_with_sounds(self):
        self.assertTrue(self.c.has_plate("sound-id"))
        self.assertFalse(self.go.has_plate("sound-id"))
        union = self.c.plate("instrument-sound")
        kinds = [(m.ref.category if m.ref else "literals") for m in union.members]
        self.assertEqual(kinds, ["value", "primitive"])  # enum + open string

    def test_deps_are_sorted_unique_and_non_primitive(self):
        for plates in (self.go, self.c, self.cpp):
            for plate in list(plates.value_types) + list(plates.complex_types):
                wires = [d.wire for d in plate.deps]
                self.assertEqual(wires, sorted(set(wires)))
                self.assertNotIn(plate.name.wire, wires)
                for d in plate.deps:
                    self.assertNotEqual(d.category, "primitive")

    def test_primitive_refs_never_become_deps(self):
        # instrument-sound (C target, sounds folded) is sound-id | string;
        # the open `string` member is a PRIMITIVE, but a complex type named
        # `string` also exists. The dependency list must not conflate them.
        union = self.c.plate("instrument-sound")
        self.assertEqual([d.wire for d in union.deps], ["sound-id"])

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

    def test_unknown_top_level_sections_fail(self):
        # The flagship fail-loud guarantee must hold at the outermost level:
        # [renames] (typo) silently dropping every rename would be the worst
        # silent misconfiguration in the system.
        with self.assertRaises(ConfigError):
            self._load("[renames.type]\nnote = 'tone'\n")
        with self.assertRaises(ConfigError):
            self._load("[targets]\nlanguage = 'go'\n")
        with self.assertRaises(ConfigError):
            self._load("[input]\nxsd_file = 'x.xsd'\n")

    def test_unsupported_values_fail(self):
        with self.assertRaises(ConfigError):
            self._load("[naming]\ntype-convention = 'dot'\n")

    def test_string_lists_reject_bare_strings(self):
        # A bare TOML string would silently explode into characters.
        with self.assertRaises(ConfigError):
            self._load("[naming]\nacronyms = 'midi'\n")
        with self.assertRaises(ConfigError):
            self._load("[reserved]\nwords = 'class'\n")

    def test_empty_rename_entry_fails(self):
        with self.assertRaises(ConfigError):
            self._load("[rename.type.note]\n")

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

    def _load_with_base(self, base_text: str, target_text: str) -> Config:
        with tempfile.TemporaryDirectory() as d:
            (Path(d) / "base.toml").write_text(base_text)
            target = Path(d) / "config.toml"
            target.write_text('[naming]\nextends = "base.toml"\n' + target_text)
            return cfg.load(target)

    def test_extends_rejects_chained_bases(self):
        with self.assertRaises(ConfigError):
            self._load_with_base("[naming]\nextends = 'grand.toml'\n", "")

    def test_extends_rejects_foreign_sections_in_base(self):
        # A [reserved] or [types] in the base would be silently dead.
        with self.assertRaises(ConfigError):
            self._load_with_base("[reserved]\nwords = ['class']\n", "")

    def test_extends_rejects_scope_shape_disagreement(self):
        # The base addresses an owner scope; the target a global entry of the
        # same name. Wholesale replacement would quietly drop the base's
        # renames, so the disagreement must be loud.
        with self.assertRaises(ConfigError):
            self._load_with_base(
                "[rename.attribute.barline]\nsegno = 'segno-sound'\n",
                "[rename.attribute]\nbarline = 'bar-line'\n",
            )


class PortablePattern(unittest.TestCase):
    """portable_pattern: XSD pattern facets re-spelled as one anchored regex
    in the portable dialect (gen.plates.build)."""

    def test_no_facets_is_none(self):
        self.assertIsNone(portable_pattern([]))

    def test_single_pattern_is_anchored(self):
        self.assertEqual(
            portable_pattern(["[^,]+(, ?[^,]+)*"]),
            "^(?:[^,]+(, ?[^,]+)*)$",
        )

    def test_name_char_escape_expands(self):
        out = portable_pattern(["acc\\c+"])
        self.assertEqual(out, "^(?:acc[-.0-9:A-Z_a-z]+)$")

    def test_name_start_escape_expands(self):
        out = portable_pattern(["\\i\\c*"])
        self.assertEqual(out, "^(?:[:A-Z_a-z][-.0-9:A-Z_a-z]*)$")

    def test_multiple_facets_or_join(self):
        # XSD: several pattern facets on one restriction step are
        # alternatives (4.0's smufl-accidental-glyph-name has two).
        out = portable_pattern(["acc\\c+", "(wiggle\\c+)"])
        self.assertTrue(out.startswith("^(?:(?:") and out.endswith(")$"))
        self.assertIn(")|(?:", out)

    def test_caret_and_dollar_are_xsd_literals(self):
        # XSD has no anchors; a bare ^ or $ must stay an ordinary character.
        self.assertEqual(portable_pattern(["a$b"]), "^(?:a\\$b)$")
        self.assertEqual(portable_pattern(["[^,]"]), "^(?:[^,])$")

    def test_ordinary_escapes_pass_through(self):
        self.assertEqual(
            portable_pattern(["#[\\dA-F]{6}([\\dA-F][\\dA-F])?"]),
            "^(?:#[\\dA-F]{6}([\\dA-F][\\dA-F])?)$",
        )

    def test_nonportable_constructs_fail_loud(self):
        for bad in ("[a-z-[aeiou]]", "\\C+", "\\I", "\\p{L}", "tail\\"):
            with self.assertRaises(ValueError, msg=bad):
                portable_pattern([bad])

    def test_every_shipped_schema_pattern_translates_and_compiles(self):
        # The translator must cover every pattern in the 3.1 and 4.0
        # schemas, and the portable form must be a valid Python regex too
        # (Python's re is the strictest of the engines the dialect targets).
        import re as regex

        for config in (GO_CONFIG, CPP_CONFIG):
            plates, _ = build_for(config)
            for p in plates.value_types:
                if getattr(p, "patterns", None):
                    self.assertIsNotNone(p.pattern, p.name.wire)
                    regex.compile(p.pattern)

    def test_color_pattern_semantics(self):
        import re as regex

        plates, _ = build_for(GO_CONFIG)
        compiled = regex.compile(plates.plate("color").pattern)
        self.assertIsNotNone(compiled.match("#FF0000"))
        self.assertIsNotNone(compiled.match("#40800080"))
        self.assertIsNone(compiled.match("#GG0000"))
        self.assertIsNone(compiled.match("#FF0000 "))  # anchoring


class ContentProjection(unittest.TestCase):
    """The grammar-preserving content projection:
    fields, group/choice plates, synthesized hoisting, and its invisibility
    to targets that consume only the flat member view."""

    @classmethod
    def setUpClass(cls):
        cls.cpp, cls.cpp_config = build_for(CPP_CONFIG)
        cls.go, cls.go_config = build_for(GO_CONFIG)

    def _fields(self, wire):
        return {f.name.wire: f for f in self.cpp.plate(wire).fields}

    def test_pure_sequence_projects_ordered_element_fields(self):
        fields = self.cpp.plate("pitch").fields
        self.assertEqual([f.name.wire for f in fields], ["step", "alter", "octave"])
        self.assertEqual(
            [f.cardinality for f in fields], ["required", "optional", "required"]
        )
        self.assertTrue(all(f.kind == "element" for f in fields))

    def test_trivial_group_splices_to_element_field(self):
        # note references the `staff` group (one element); the field view
        # splices it inline with the occurrence bounds multiplied through.
        staff = self._fields("note")["staff"]
        self.assertEqual(staff.kind, "element")
        self.assertEqual(staff.cardinality, "optional")

    def test_structural_group_becomes_shared_type(self):
        editorial = self._fields("attributes")["editorial"]
        self.assertEqual(editorial.kind, "group")
        self.assertEqual(editorial.type_ref.wire, "editorial-group")
        plate = self.cpp.plate("editorial-group")
        self.assertEqual(plate.strategy, "group-class")
        self.assertFalse(plate.synthesized)
        self.assertEqual([f.name.wire for f in plate.fields], ["footnote", "level"])
        # The dispatch set a strict parser tests the next element against.
        self.assertEqual(editorial.first, ["footnote", "level"])

    def test_choice_shaped_group_absorbs_occurrence_at_reference(self):
        # music-data is S(C[0,unbounded](...)): the group IS the choice and
        # the measure's field takes the repetition.
        md = self._fields("partwise-measure")["music-data"]
        self.assertEqual(md.kind, "choice")
        self.assertEqual(md.cardinality, "vector")
        self.assertEqual(md.type_ref.wire, "music-data-choice")
        plate = self.cpp.plate("music-data-choice")
        self.assertEqual(plate.strategy, "choice-class")
        self.assertEqual(plate.alternatives[0].tag, "note")
        self.assertFalse(plate.nullable)

    def test_note_choice_hoists_branch_sequences(self):
        # The hoisted field is named by the type's owner-stripped remainder.
        choice = self._fields("note")["choice"]
        self.assertEqual(choice.kind, "choice")
        self.assertEqual(choice.cardinality, "required")
        self.assertEqual(choice.type_ref.wire, "note-choice")
        plate = self.cpp.plate("note-choice")
        self.assertTrue(plate.synthesized)
        self.assertEqual(len(plate.alternatives), 3)
        self.assertTrue(all(a.kind == "group" for a in plate.alternatives))
        # Branch dispatch sets are disjoint (LL(1) over the corpus grammar).
        firsts = [set(a.first) for a in plate.alternatives]
        self.assertEqual(firsts[0] & firsts[1], set())
        self.assertEqual(firsts[1] & firsts[2], {"chord", "pitch", "unpitched", "rest"} & firsts[1])

    def test_full_note_presence_flag_and_inner_choice(self):
        plate = self.cpp.plate("full-note-group")
        chord = plate.fields[0]
        self.assertEqual(chord.name.wire, "chord")
        self.assertTrue(chord.presence)
        self.assertEqual(chord.cardinality, "optional")
        inner = plate.fields[1]
        self.assertEqual(inner.kind, "choice")
        self.assertEqual(inner.first, ["pitch", "unpitched", "rest"])

    def test_bounded_repeats_carry_their_cap(self):
        beam = self._fields("note")["beam"]
        self.assertEqual(beam.cardinality, "vector")
        self.assertEqual(beam.max, "8")
        tie = {
            f.name.wire: f for f in self.cpp.plate("note-choice-group-3").fields
        }["tie"]
        self.assertEqual(tie.max, "2")

    def test_min_one_repeats_are_marked(self):
        figure = self._fields("figured-bass")["figure"]
        self.assertEqual(figure.cardinality, "vector")
        self.assertTrue(figure.min1)

    def test_nullable_choice_with_vector_alternative(self):
        plate = self.cpp.plate("key-choice")
        self.assertTrue(plate.nullable)
        trad, nontrad = plate.alternatives
        self.assertEqual(trad.cardinality, "required")
        self.assertEqual(trad.kind, "group")
        self.assertEqual(nontrad.cardinality, "vector")
        self.assertFalse(nontrad.min1)

    def test_element_names_classify_misplaced_vs_unknown(self):
        names = self.cpp.plate("pitch").element_names
        self.assertEqual(names, ["step", "alter", "octave"])
        self.assertIn("pitch", self.cpp.plate("note").element_names)

    def test_structural_deps_replace_flat_deps(self):
        note = self.cpp.plate("note")
        flat = {d.wire for d in note.deps}
        structural = {d.wire for d in note.structural_deps}
        self.assertIn("pitch", flat)  # the flat view reaches elements directly
        self.assertNotIn("pitch", structural)  # the structural view goes via the choice
        self.assertIn("note-choice", structural)
        self.assertNotIn("note-choice", flat)

    def test_synthesized_wires_are_renameable(self):
        plates, config = build_for(CPP_CONFIG)
        config.renames.types["note-choice"] = RenameEntry(
            fundamental="note-variants"
        )
        from gen.ir.build import build_ir as _build
        from gen.ir.sounds import patch_sounds, read_sound_ids
        from gen.xsd import parse as _parse

        m = _build(_parse(config.xsd), source=config.xsd.stem)
        patch_sounds(m, read_sound_ids(config.sounds_xml))
        renamed = build_plates(m, config)
        self.assertEqual(renamed.plate("note-choice").ident, "NoteVariants")
        # A stale key naming no type (real or synthesized) fails loud.
        config.renames.types["note-choice-zzz"] = RenameEntry(fundamental="x")
        with self.assertRaises(PlatesError) as caught:
            build_plates(m, config)
        self.assertIn("no such type", caught.exception.errors[0])

    def test_determinism_with_groups(self):
        again, _ = build_for(CPP_CONFIG)
        self.assertEqual(to_json(self.cpp), to_json(again))

    def test_projection_is_invisible_to_flat_targets(self):
        # The Go target projects the same groups data (it is neutral), but
        # its members view is untouched by the projection's existence.
        self.assertTrue(self.go.groups)
        note = self.go.plate("note")
        self.assertTrue(all(m.kind in ("attribute", "element") for m in note.members))


if __name__ == "__main__":
    unittest.main()
