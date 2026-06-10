"""Go backend: render the Plates into the Go test target package.

Dumb renderers per the design: identifiers, casings, type mappings, file
stems, and structure all arrive resolved on the plates; this package owns
only Go grammar (declarations, parse/serialize bodies, comment syntax) and
the runtime support file.

Rendered: the four value shapes, the four complex shapes, the document
entry points (FromXDoc/ToXDoc), and the runtime support file.
"""

from __future__ import annotations

import shutil
import subprocess
import tempfile
from pathlib import Path

from gen.emit.go.complexes import complex_file
from gen.emit.go.document import document_file
from gen.emit.go.runtime import runtime_file
from gen.emit.go.values import value_file
from gen.plates.model import Plates


# File stems this backend reserves for its support files; a schema type
# landing on one would silently overwrite it in the manifest.
_RESERVED_STEMS = ("runtime", "document")

# Type identifiers this backend declares itself (document.go).
_RESERVED_IDENTS = ("Document", "ExtraAttr")


def _guard_identifiers(plates: Plates) -> None:
    """The plates' collision gate certifies projected identifiers; this
    backend also COMPOSES a few (the per-type Child struct, the Children
    field, its support types). A schema name landing on one of those must
    fail loud here, not as a confusing compile error in generated code.
    It also rejects the one derivation shape the inherit template cannot
    render (children spread across the chain)."""
    from gen.plates.model import element_members

    type_idents = {p.ident for p in list(plates.value_types) + list(plates.complex_types)}
    for plate in list(plates.value_types) + list(plates.complex_types):
        if plate.file in _RESERVED_STEMS:
            raise ValueError(
                f"type '{plate.name.wire}' projects to the reserved file stem "
                f"'{plate.file}'; rename it in config.toml"
            )
        if plate.ident in _RESERVED_IDENTS:
            raise ValueError(
                f"type '{plate.name.wire}' projects to '{plate.ident}', which this "
                f"backend reserves; rename it in config.toml"
            )
    for plate in plates.complex_types:
        elements = element_members(plate.members)
        if elements and f"{plate.ident}Child" in type_idents:
            raise ValueError(
                f"type '{plate.ident}Child' collides with '{plate.name.wire}'s "
                f"child struct; rename one in config.toml"
            )
        if elements and any(m.ident == "Children" for m in plate.members):
            raise ValueError(
                f"'{plate.name.wire}' has a member projecting to 'Children', "
                f"which this backend reserves; rename it in config.toml"
            )
    for plate in plates.complex_types:
        if plate.strategy != "inherit":
            continue
        # The inherit template dispatches children against ONE chain member's
        # child struct. A derivation chain where two members carry element
        # members (no MusicXML schema has one) would lose the others' children
        # -- fail loud rather than emit a parser with a blind spot.
        bearing = []
        cur = plate
        while cur is not None:
            if element_members(cur.members):
                bearing.append(cur.name.wire)
            cur = plates.plate(cur.base.wire) if cur.base is not None else None
        if len(bearing) > 1:
            raise ValueError(
                f"'{plate.name.wire}': multiple chain members carry child "
                f"elements ({', '.join(bearing)}); the inherit template "
                f"cannot dispatch them all"
            )


def render(plates: Plates) -> dict[str, str]:
    _guard_identifiers(plates)
    files: dict[str, str] = {
        "runtime.go": runtime_file(plates),
        "document.go": document_file(plates),
    }
    for plate in plates.value_types:
        files[plate.file + ".go"] = value_file(plates, plate)
    for plate in plates.complex_types:
        files[plate.file + ".go"] = complex_file(plates, plate)
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
            (root / rel).parent.mkdir(parents=True, exist_ok=True)
            (root / rel).write_text(content, encoding="utf-8")
        subprocess.run(["gofmt", "-w", scratch], check=True)
        return {rel: (root / rel).read_text(encoding="utf-8") for rel in files}
