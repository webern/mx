"""Tests for the press's writer.

The writer owns the contract that makes regeneration safe and repeatable:
write-if-changed, prune only generator-marked files, refuse unsafe paths and
unmarked content.

Run with: python3 -m unittest gen.tests.test_writer
"""

from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from gen.press.writer import banner, is_generated, write_files


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



if __name__ == "__main__":
    unittest.main()
