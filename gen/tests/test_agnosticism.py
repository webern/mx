"""The cardinal rule, enforced structurally: the generator is language
agnostic. Adding a new language target must not require edits to the
generator's Python files -- so the generator's Python is a CLOSED set of
packages, none named after a language, and no target directory contains
Python at all. Language knowledge lives in each target's config.toml and
templates/ directory.

If this test fails because you added a module, ask: could this line be
wrong for a language we have not heard of? If yes, it belongs in a target's
directory as config or template text (docs/ai/design/generator-agnosticism.md).

Run with: python3 -m unittest gen.tests.test_agnosticism
"""

from __future__ import annotations

import unittest
from pathlib import Path

GEN = Path(__file__).resolve().parents[1]

# The closed set: every directory under gen/ that may contain Python.
ALLOWED_PYTHON_DIRS = {
    GEN,  # __main__.py, config.py, names.py, __init__.py
    GEN / "xsd",
    GEN / "ir",
    GEN / "plates",
    GEN / "press",
    GEN / "tests",
}


class CardinalRule(unittest.TestCase):
    def test_generator_python_is_a_closed_set(self):
        strays = [
            str(p.relative_to(GEN))
            for p in GEN.rglob("*.py")
            if p.parent not in ALLOWED_PYTHON_DIRS
        ]
        self.assertEqual(
            strays, [],
            "Python outside the generator's closed set -- language knowledge "
            "belongs in a target's config.toml and templates/, not in code",
        )

    def test_no_module_is_named_after_a_language(self):
        # The old shape of the violation: gen/emit/go, gen/emit/c,
        # gen/plates/languages.py. Nothing under the closed set may bring
        # it back under any spelling.
        forbidden = {"go", "c", "cpp", "languages", "backends"}
        offenders = [
            str(p.relative_to(GEN))
            for d in ALLOWED_PYTHON_DIRS
            for p in d.glob("*.py")
            if p.stem in forbidden
        ]
        self.assertEqual(offenders, [])

    def test_targets_are_config_and_templates_only(self):
        # A target directory is identified by its config.toml. Its generator
        # inputs are that config and its templates; the only Python near it
        # is consumer code (test harnesses), never under templates/.
        for config in GEN.rglob("config.toml"):
            templates = config.parent / "templates"
            if templates.is_dir():
                stray = list(templates.rglob("*.py"))
                self.assertEqual(stray, [], f"Python inside {templates}")


if __name__ == "__main__":
    unittest.main()
