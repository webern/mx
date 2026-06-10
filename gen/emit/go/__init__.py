"""Go backend: render the Plates into the Go test target package.

Dumb renderers per the design: identifiers, casings, type mappings, file
stems, and structure all arrive resolved on the plates; this package owns
only Go grammar (declarations, parse/serialize bodies, comment syntax) and
the runtime support file.

Currently rendered: the four value shapes (the leaf node types) plus the
runtime. Complex types and the document entry points are later phases; until
they land, the emitted package is a compilable library of value types and
the corert harness keeps using its stub.
"""

from __future__ import annotations

import shutil
import subprocess
import tempfile
from pathlib import Path

from gen.emit.go.runtime import runtime_file
from gen.emit.go.values import value_file
from gen.plates.model import Plates


def render(plates: Plates) -> dict[str, str]:
    files: dict[str, str] = {"runtime.go": runtime_file(plates)}
    for plate in plates.value_types:
        files[plate.file + ".go"] = value_file(plates, plate)
    return _gofmt(files)


def _gofmt(files: dict[str, str]) -> dict[str, str]:
    """Run the rendered files through gofmt, once, in a scratch directory.
    The templates emit straightforward go syntax; gofmt owns the alignment
    rules (struct fields, map literals) that are its tabwriter's business,
    exactly as Go codegen convention expects. Requiring the formatter keeps
    the emitted bytes canonical instead of approximating its heuristics."""
    if shutil.which("gofmt") is None:
        raise RuntimeError("the Go backend requires gofmt on PATH")
    with tempfile.TemporaryDirectory() as scratch:
        root = Path(scratch)
        for rel, content in files.items():
            (root / rel).write_text(content, encoding="utf-8")
        subprocess.run(["gofmt", "-w", scratch], check=True)
        return {rel: (root / rel).read_text(encoding="utf-8") for rel in files}
