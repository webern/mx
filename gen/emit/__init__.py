"""The emit stage: render the Plates through a per-language backend.

A backend is a module exposing one function:

    render(plates: gen.plates.Plates) -> dict[str, str]

mapping output paths (relative to the target's [output] dir) to file
contents. Backends are the templates of the pipeline -- dumb renderers that
walk plates and print text. They make no naming decisions (identifiers,
casings, file stems all arrive resolved on the plates) and no schema
decisions (structure and cardinality arrive resolved from the IR); what they
own is the target language's grammar: declarations, parse/serialize bodies,
comment syntax, and which support files accompany the generated types.

Backends register here by language name; the language comes from the
target's config ([target] language), so `python3 -m gen <config.toml>` is
the whole user interface.
"""

from __future__ import annotations

import importlib

from gen.config import Config
from gen.emit.writer import EmitResult, write_files
from gen.plates.model import Plates

# language -> backend module path, imported lazily so a target can be
# projected (`gen plates`) without every backend being importable.
BACKENDS: dict[str, str] = {
    "go": "gen.emit.go",
}


class EmitError(Exception):
    pass


def emit(plates: Plates, config: Config) -> EmitResult:
    language = plates.target.language
    module_path = BACKENDS.get(language)
    if module_path is None:
        known = ", ".join(sorted(BACKENDS)) or "none yet"
        raise EmitError(
            f"no emit backend for language '{language}' (backends: {known})"
        )
    if config.output_dir is None:
        raise EmitError(f"config has no [output] dir: {config.path}")
    backend = importlib.import_module(module_path)
    rendered = backend.render(plates)
    return write_files(config.output_dir, rendered)
