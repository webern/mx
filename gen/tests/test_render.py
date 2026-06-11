"""Tests for the press's context builder and manifest-driven rendering.

The context builder's enrichments are mechanical (discriminant expansion,
quoted companions, loop metadata, member splits) -- each is pinned here. The
render path is exercised end to end against a tiny in-memory IR and real
template files in a temporary target directory, including the fail-loud
manifest checks (coverage, collisions, unknown strategies/placeholders) and
the generic post-render format hook.

Run with: python3 -m unittest gen.tests.test_render
"""

from __future__ import annotations

import sys
import tempfile
import unittest
from pathlib import Path

from gen.config import Config, RenderEntry, RenderSection
from gen.ir import model as ir
from gen.plates import build_plates
from gen.press.context import plate_context, target_context
from gen.press.render import RenderError, render_files, render_target


def tiny_ir() -> ir.Ir:
    up_down = ir.EnumType("up-down", "token", ["up", "down"], doc="Up or down.")
    note = ir.ComplexType(
        "note",
        "composite",
        attributes=[ir.Attr("type", ir.Ref("up-down", "value"), default="up")],
        content=ir.Sequence(
            [ir.Element("tie", ir.Ref("up-down", "value"), "vector", 0, ir.UNBOUNDED)]
        ),
    )
    return ir.Ir(
        source="musicxml-9.9-test",
        builtins={},
        value_types=[up_down],
        groups=[],
        attribute_groups=[],
        complex_types=[note],
        roots=[ir.Root("note", "note")],
        dropped_dead=[],
        stats={},
    )


def tiny_plates():
    return build_plates(tiny_ir(), Config())


class ContextBuilder(unittest.TestCase):
    def setUp(self):
        self.plates = tiny_plates()

    def test_discriminant_expansion_materializes_all_flags(self):
        enum = plate_context(self.plates, self.plates.plate("up-down"))
        self.assertTrue(enum["is_enum"])
        self.assertFalse(enum["is_number"])
        self.assertFalse(enum["is_complex"])
        member = plate_context(self.plates, self.plates.plate("note"))["attributes"][0]
        self.assertTrue(member["is_attribute"])
        self.assertFalse(member["is_element"])
        self.assertTrue(member["type_ref"]["is_enum"])
        self.assertTrue(member["is_optional"])

    def test_quoted_companions(self):
        ctx = plate_context(self.plates, self.plates.plate("up-down"))
        variant = ctx["variants"][0]
        self.assertEqual(variant["wire"], "up")
        self.assertEqual(variant["wire_q"], '"up"')
        self.assertEqual(ctx["name"]["kebab_q"], '"up-down"')

    def test_loop_metadata(self):
        ctx = plate_context(self.plates, self.plates.plate("up-down"))
        first, last = ctx["variants"]
        self.assertTrue(first["is_first"])
        self.assertFalse(first["is_last"])
        self.assertTrue(last["is_last"])
        self.assertEqual(last["index0"], 1)

    def test_member_splits_and_type_self_reference(self):
        ctx = plate_context(self.plates, self.plates.plate("note"))
        self.assertEqual(len(ctx["attributes"]), 1)
        self.assertEqual(len(ctx["elements"]), 1)
        self.assertIsNone(ctx["value"])
        self.assertIs(ctx["type"], ctx)  # inner scopes reach plate fields

    def test_scalar_lists_are_lifted(self):
        m = tiny_ir()
        m.value_types.append(
            ir.StringType("color", "token", patterns=["#[0-9A-F]{6}"])
        )
        plates = build_plates(m, Config())
        ctx = plate_context(plates, plates.plate("color"))
        self.assertEqual(ctx["patterns"][0]["value"], "#[0-9A-F]{6}")
        self.assertTrue(ctx["patterns"][0]["is_first"])

    def test_target_context_outputs_by_extension(self):
        ctx = target_context(self.plates, ["b.c", "a.c", "a.h"])
        self.assertEqual([o["path"] for o in ctx["outputs"]], ["a.c", "a.h", "b.c"])
        self.assertEqual(
            [o["path"] for o in ctx["outputs_by_ext"]["c"]], ["a.c", "b.c"]
        )
        self.assertIn("generated_banner", ctx)
        self.assertEqual(ctx["schema_version"], "9.9")


def _target_dir(templates: dict[str, str]) -> tempfile.TemporaryDirectory:
    holder = tempfile.TemporaryDirectory()
    root = Path(holder.name)
    (root / "templates").mkdir()
    for name, text in templates.items():
        (root / "templates" / name).write_text(text)
    return holder


_BANNER = "// {{generated_banner}}\n"


class ManifestRendering(unittest.TestCase):
    def setUp(self):
        self.plates = tiny_plates()

    def _config(self, holder, types=(), once=(), format_cmd=()):
        root = Path(holder.name)
        config = Config(output_dir=root / "out")
        config.render = RenderSection(
            dir=root / "templates",
            format=tuple(format_cmd),
            types=list(types),
            once=list(once),
        )
        return config

    def test_renders_per_type_and_once(self):
        holder = _target_dir(
            {
                "enum.tmpl": _BANNER + "enum {{ident}}\n{{#variants}}{{ident}} = {{wire_q}}\n{{/variants}}",
                "complex.tmpl": _BANNER + "class {{ident}} {{> fields}}",
                "fields.tmpl": "{{#attributes}}[{{ident}}]{{/attributes}}\n",
                "manifest.tmpl": _BANNER + "{{#outputs}}{{path}}\n{{/outputs}}",
            }
        )
        self.addCleanup(holder.cleanup)
        config = self._config(
            holder,
            types=[
                RenderEntry("enum.tmpl", "{snake}.txt", ("enum-class",)),
                RenderEntry("complex.tmpl", "{snake}.txt", ("composite-class",)),
            ],
            once=[RenderEntry("manifest.tmpl", "all.txt")],
        )
        files = render_files(self.plates, config)
        self.assertEqual(
            sorted(files), ["all.txt", "note.txt", "up_down.txt"]
        )
        self.assertIn("enum UpDown", files["up_down.txt"])
        self.assertIn('Up = "up"', files["up_down.txt"])
        self.assertIn("class Note [type]", files["note.txt"])
        self.assertIn("note.txt", files["all.txt"])

        result = render_target(self.plates, config)
        self.assertEqual(len(result.written), 3)

    def test_type_row_overrides_strategy_row(self):
        # Bespoke handling: a `types` row claims a plate by wire name and the
        # strategy rows no longer render it -- custom code for one type is a
        # config-and-template change, never a generator change.
        holder = _target_dir(
            {
                "enum.tmpl": _BANNER + "stock {{ident}}",
                "complex.tmpl": _BANNER + "class {{ident}}",
                "bespoke.tmpl": _BANNER + "bespoke {{ident}}",
            }
        )
        self.addCleanup(holder.cleanup)
        config = self._config(
            holder,
            types=[
                RenderEntry("enum.tmpl", "{snake}.txt", ("enum-class",)),
                RenderEntry("complex.tmpl", "{snake}.txt", ("composite-class",)),
                RenderEntry("bespoke.tmpl", "{snake}.txt", types=("up-down",)),
            ],
        )
        files = render_files(self.plates, config)
        self.assertIn("bespoke UpDown", files["up_down.txt"])
        self.assertNotIn("stock", files["up_down.txt"])
        self.assertIn("class Note", files["note.txt"])

    def test_config_row_requires_exactly_one_selector(self):
        from gen import config as cfg

        with self.assertRaises(cfg.ConfigError):
            cfg._render_entry(
                {"template": "x.tmpl", "output": "x.txt"}, "render.type", once=False
            )
        with self.assertRaises(cfg.ConfigError):
            cfg._render_entry(
                {
                    "template": "x.tmpl",
                    "output": "x.txt",
                    "strategies": ["enum-class"],
                    "types": ["color"],
                },
                "render.type",
                once=False,
            )

    def test_type_row_unknown_wire_name_fails_loud(self):
        holder = _target_dir(
            {
                "enum.tmpl": _BANNER,
                "complex.tmpl": _BANNER,
                "bespoke.tmpl": _BANNER,
            }
        )
        self.addCleanup(holder.cleanup)
        config = self._config(
            holder,
            types=[
                RenderEntry("enum.tmpl", "{snake}.txt", ("enum-class",)),
                RenderEntry("complex.tmpl", "{snake}.txt", ("composite-class",)),
                RenderEntry("bespoke.tmpl", "{snake}.txt", types=("no-such-type",)),
            ],
        )
        with self.assertRaises(RenderError) as caught:
            render_files(self.plates, config)
        self.assertIn("no-such-type", str(caught.exception))

    def test_unmatched_strategy_fails_loud(self):
        holder = _target_dir({"enum.tmpl": _BANNER})
        self.addCleanup(holder.cleanup)
        config = self._config(
            holder, types=[RenderEntry("enum.tmpl", "{snake}.txt", ("enum-class",))]
        )
        with self.assertRaises(RenderError) as caught:
            render_files(self.plates, config)
        self.assertIn("composite-class", str(caught.exception))

    def test_unknown_strategy_fails_loud(self):
        holder = _target_dir({"x.tmpl": _BANNER})
        self.addCleanup(holder.cleanup)
        config = self._config(
            holder, types=[RenderEntry("x.tmpl", "{snake}.txt", ("enumclass",))]
        )
        with self.assertRaises(RenderError) as caught:
            render_files(self.plates, config)
        self.assertIn("enumclass", str(caught.exception))

    def test_output_collision_fails_loud(self):
        holder = _target_dir({"x.tmpl": _BANNER})
        self.addCleanup(holder.cleanup)
        config = self._config(
            holder,
            types=[
                RenderEntry("x.tmpl", "same.txt", ("enum-class",)),
                RenderEntry("x.tmpl", "same.txt", ("composite-class",)),
            ],
        )
        with self.assertRaises(RenderError) as caught:
            render_files(self.plates, config)
        self.assertIn("collision", str(caught.exception))

    def test_unknown_placeholder_fails_loud(self):
        holder = _target_dir({"x.tmpl": _BANNER})
        self.addCleanup(holder.cleanup)
        config = self._config(
            holder,
            types=[
                RenderEntry("x.tmpl", "{nope}.txt", ("enum-class", "composite-class"))
            ],
        )
        with self.assertRaises(RenderError) as caught:
            render_files(self.plates, config)
        self.assertIn("placeholder", str(caught.exception))

    def test_unmarked_output_fails_loud(self):
        holder = _target_dir({"x.tmpl": "no marker here\n"})
        self.addCleanup(holder.cleanup)
        config = self._config(holder, once=[RenderEntry("x.tmpl", "x.txt")])
        with self.assertRaises(RenderError) as caught:
            render_files(self.plates, config)
        self.assertIn("generated_banner", str(caught.exception))

    def test_format_hook_runs_over_scratch(self):
        holder = _target_dir({"x.tmpl": _BANNER + "body\n"})
        self.addCleanup(holder.cleanup)
        script = (
            "import pathlib, sys\n"
            "for p in pathlib.Path(sys.argv[1]).rglob('*.txt'):\n"
            "    p.write_text(p.read_text().replace('body', 'formatted'))\n"
        )
        config = self._config(
            holder,
            once=[RenderEntry("x.tmpl", "x.txt")],
            format_cmd=(sys.executable, "-c", script, "{dir}"),
        )
        files = render_files(self.plates, config)
        self.assertIn("body", files["x.txt"])  # pure render: unformatted
        result = render_target(self.plates, config)
        out = (config.output_dir / "x.txt").read_text()
        self.assertIn("formatted", out)
        self.assertEqual(result.written, ["x.txt"])

    def test_format_command_failure_fails_loud(self):
        holder = _target_dir({"x.tmpl": _BANNER})
        self.addCleanup(holder.cleanup)
        config = self._config(
            holder,
            once=[RenderEntry("x.tmpl", "x.txt")],
            format_cmd=(sys.executable, "-c", "raise SystemExit(3)"),
        )
        with self.assertRaises(RenderError):
            render_target(self.plates, config)


if __name__ == "__main__":
    unittest.main()
