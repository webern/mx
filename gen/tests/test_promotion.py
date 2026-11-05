"""Tests for [target] promote-primitives in the Plates projection.

A promoted primitive's direct uses (attributes, elements, content fields,
value bodies) resolve to a synthesized, facet-free number type named after
the primitive, so they carry the primitive's implied clamp. Named
restrictions and union members are left alone, and a target that does not
opt in sees no change.

Run with: python3 -m unittest gen.tests.test_promotion
"""

from __future__ import annotations

import unittest

from gen.config import Config
from gen.ir import model as ir
from gen.plates import PlatesError, build_plates
from gen.tests.test_plates import tiny_ir


def promotion_ir() -> ir.Ir:
    """tiny_ir plus every place a bare integer primitive can appear: an
    attribute, element members, a value body, a named restriction, and a
    union member."""
    m = tiny_ir()
    pos = ir.Ref("positive_integer", "primitive")
    non_neg = ir.Ref("non_negative_integer", "primitive")
    m.value_types += [
        ir.NumberType("staff-number", "positive_integer"),
        ir.UnionType("positive-or-empty", [ir.UnionMember(pos), ir.UnionMember(literals=[""])]),
    ]
    m.complex_types += [
        ir.ComplexType(
            "frame",
            "composite",
            attributes=[ir.Attr("size", pos)],
            content=ir.Sequence(
                [
                    ir.Element("strings", pos, "required"),
                    ir.Element("capo", non_neg, "optional", 0, 1),
                ]
            ),
        ),
        ir.ComplexType("fret", "value", value_type=non_neg),
    ]
    return m


def promoting_config() -> Config:
    """A config that spells both primitives `int` and promotes them."""
    config = Config(types={"positive_integer": "int", "non_negative_integer": "int"})
    config.target.promote_primitives = ("positive_integer", "non_negative_integer")
    return config


def clamp_of(plate) -> list[tuple[str, str, str]]:
    """A number plate's or union member's clamp steps as plain tuples."""
    return [(s.op, s.bound, s.replacement) for s in plate.clamp]


class PrimitivePromotion(unittest.TestCase):
    """The projection of promoted primitives and its fail-loud validation."""

    def test_synthesized_number_types_carry_the_implied_clamp(self):
        """Each promoted primitive becomes a number plate with its implied
        bound and the primitive's [types] spelling."""
        plates = build_plates(promotion_ir(), promoting_config())
        positive = plates.plate("positive-integer")
        self.assertEqual(positive.kind, "number")
        self.assertEqual(positive.base, "positive_integer")
        self.assertEqual(positive.target_type, "int")
        self.assertEqual(clamp_of(positive), [("<", "1", "1")])
        self.assertEqual(clamp_of(plates.plate("non-negative-integer")), [("<", "0", "0")])

    def test_direct_uses_reference_the_synthesized_types(self):
        """Attributes, elements, content fields and value bodies all resolve
        to the synthesized type, which then shows up as a dependency."""
        plates = build_plates(promotion_ir(), promoting_config())
        frame = plates.plate("frame")
        refs = {
            "attribute": frame.member("size", kind="attribute").type_ref,
            "element": frame.member("strings", kind="element").type_ref,
            "optional element": frame.member("capo", kind="element").type_ref,
            "value body": next(
                m.type_ref for m in plates.plate("fret").members if m.kind == "value"
            ),
        }
        refs.update({f"field {f.name.wire}": f.type_ref for f in frame.fields})
        for where, ref in refs.items():
            with self.subTest(where):
                self.assertEqual(ref.category, "value")
                self.assertEqual(ref.kind, "number")
        self.assertEqual(refs["attribute"].ident, "PositiveInteger")
        self.assertEqual(refs["value body"].wire, "non-negative-integer")
        self.assertEqual(
            [d.wire for d in frame.structural_deps], ["non-negative-integer", "positive-integer"]
        )

    def test_named_restrictions_and_union_members_are_unchanged(self):
        """A derived number type still wraps the primitive's own spelling,
        and a union member stays primitive with its own implied clamp."""
        plates = build_plates(promotion_ir(), promoting_config())
        self.assertEqual(plates.plate("staff-number").target_type, "int")
        member = plates.plate("positive-or-empty").members[0]
        self.assertEqual(member.ref.category, "primitive")
        self.assertEqual(clamp_of(member), [("<", "1", "1")])

    def test_without_the_key_primitives_stay_primitive(self):
        """Promotion is opt-in: the default config adds no types."""
        plates = build_plates(promotion_ir(), Config())
        self.assertFalse(plates.has_plate("positive-integer"))
        ref = plates.plate("frame").member("size", kind="attribute").type_ref
        self.assertEqual(ref.category, "primitive")

    def test_non_numeric_entry_fails_loud(self):
        """Only a numeric primitive can become a number type."""
        config = promoting_config()
        config.target.promote_primitives = ("string",)
        with self.assertRaises(PlatesError) as caught:
            build_plates(promotion_ir(), config)
        self.assertIn("not a numeric IR primitive", caught.exception.errors[0])

    def test_synthesized_name_taken_by_the_schema_fails_loud(self):
        """The synthesized type must not shadow a type the schema declares."""
        m = promotion_ir()
        m.value_types.append(ir.NumberType("positive-integer", "integer"))
        with self.assertRaises(PlatesError) as caught:
            build_plates(m, promoting_config())
        self.assertIn("already a type in the IR", caught.exception.errors[0])


if __name__ == "__main__":
    unittest.main()
