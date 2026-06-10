"""Tests for the emit stage's writer and backend dispatch.

The writer owns the contract that makes regeneration safe and repeatable:
write-if-changed, prune only generator-marked files, refuse unsafe paths and
unmarked content. Backends themselves are exercised by their own tests.

Run with: python3 -m unittest gen.tests.test_emit
"""

from __future__ import annotations

import sys
import tempfile
import types
import unittest
from pathlib import Path

from gen import emit
from gen.config import Config
from gen.emit.writer import banner, is_generated, write_files
from gen.plates.model import DocStyle, Plates, TargetInfo


def _mark(text: str) -> str:
    return f"// {banner('test')}\n{text}\n"


class Writer(unittest.TestCase):
    def setUp(self):
        self._tmp = tempfile.TemporaryDirectory()
        self.out = Path(self._tmp.name)
        self.addCleanup(self._tmp.cleanup)

    def test_writes_then_reports_unchanged(self):
        files = {"a.go": _mark("package mx"), "sub/b.go": _mark("package mx")}
        first = write_files(self.out, files)
        self.assertEqual(sorted(first.written), ["a.go", "sub/b.go"])
        second = write_files(self.out, files)
        self.assertEqual(second.written, [])
        self.assertEqual(sorted(second.unchanged), ["a.go", "sub/b.go"])

    def test_prunes_stale_generated_files_only(self):
        write_files(self.out, {"old.go": _mark("package mx")})
        (self.out / "handwritten.go").write_text("package mx // mine\n")
        result = write_files(self.out, {"new.go": _mark("package mx")})
        self.assertEqual(result.pruned, ["old.go"])
        self.assertEqual(result.foreign, ["handwritten.go"])
        self.assertTrue((self.out / "handwritten.go").exists())
        self.assertFalse((self.out / "old.go").exists())

    def test_rejects_unmarked_content(self):
        with self.assertRaises(ValueError):
            write_files(self.out, {"a.go": "package mx\n"})

    def test_rejects_unsafe_paths(self):
        for bad in ("../escape.go", "/abs.go"):
            with self.assertRaises(ValueError):
                write_files(self.out, {bad: _mark("x")})

    def test_marker_must_be_near_the_top(self):
        buried = ("//x\n" * 300) + f"// {banner('test')}\n"
        self.assertFalse(is_generated(buried))
        self.assertTrue(is_generated(_mark("body")))


class Dispatch(unittest.TestCase):
    def _plates(self, language: str) -> Plates:
        return Plates(
            source="test",
            target=TargetInfo(
                language=language,
                namespace="",
                prefix="",
                type_convention="pascal",
                field_convention="snake",
                variant_convention="pascal",
                file_convention="snake",
                inheritance=True,
                variant_scope="bare",
                doc_style=DocStyle(style="//"),
                reserved=[],
                partition="single",
            ),
        )

    def test_unknown_language_fails_loud(self):
        with self.assertRaises(emit.EmitError) as caught:
            emit.emit(self._plates("cobol"), Config(output_dir=Path(".")))
        self.assertIn("cobol", str(caught.exception))

    def test_backend_render_is_written(self):
        module = types.ModuleType("gen.tests._fake_backend")
        module.render = lambda plates: {"out.txt": _mark(plates.target.language)}
        sys.modules["gen.tests._fake_backend"] = module
        emit.BACKENDS["fake"] = "gen.tests._fake_backend"
        self.addCleanup(emit.BACKENDS.pop, "fake")
        self.addCleanup(sys.modules.pop, "gen.tests._fake_backend")

        with tempfile.TemporaryDirectory() as d:
            out = Path(d) / "generated"
            result = emit.emit(self._plates("fake"), Config(output_dir=out))
            self.assertEqual(result.written, ["out.txt"])
            self.assertIn("fake", (out / "out.txt").read_text())

    def test_missing_output_dir_fails_loud(self):
        emit.BACKENDS["fake2"] = "gen.tests._fake_backend2"
        self.addCleanup(emit.BACKENDS.pop, "fake2")
        with self.assertRaises(emit.EmitError) as caught:
            emit.emit(self._plates("fake2"), Config(output_dir=None))
        self.assertIn("[output] dir", str(caught.exception))


if __name__ == "__main__":
    unittest.main()
