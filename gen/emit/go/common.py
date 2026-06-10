"""Go grammar helpers shared by the Go templates.

Everything here is language spelling: string literal quoting, doc comments,
the file frame, and the composition rules Go uses for scoped names (enum
constants are package-scoped, so variant constants are composed as
TypeIdent + VariantCasing). No naming decisions happen here -- identifiers
and casings arrive resolved on the plates.
"""

from __future__ import annotations

import json

from gen.emit.writer import banner
from gen.plates.model import Name, Plates


def go_string(s: str) -> str:
    """A Go interpreted string literal. JSON escaping is a strict subset of
    Go's escape syntax (\\uXXXX, \\n, \\\"), so this is always valid Go."""
    return json.dumps(s, ensure_ascii=True)


def doc_comment(text: str | None, wrap: int, prefix: str = "") -> list[str]:
    """Wrap schema documentation into // comment lines. The text arrives raw
    on the plate (neutral core); the comment syntax is applied here."""
    if not text:
        return []
    words = text.split()
    if not words:
        return []
    lines: list[str] = []
    current = f"{prefix}//"
    for word in words:
        if len(current) + 1 + len(word) > wrap and current != f"{prefix}//":
            lines.append(current)
            current = f"{prefix}//"
        current += " " + word
    lines.append(current)
    return lines


def file_frame(plates: Plates, body: list[str], imports: list[str] | None = None) -> str:
    """The standard generated-file frame: marker, package clause, imports."""
    lines = [f"// {banner(plates.source)}", "", f"package {plates.target.namespace}"]
    if imports:
        lines += ["", "import ("]
        lines += [f"\t{go_string(imp)}" for imp in sorted(imports)]
        lines += [")"]
    lines += [""]
    lines += body
    return "\n".join(lines).rstrip("\n") + "\n"


def member_field(plates: Plates, name: Name) -> str:
    """A struct field or kind-name component in the target's field casing."""
    return name.cased[plates.target.field_convention]


def unexported(name: Name) -> str:
    """An unexported helper identifier derived from a type's name (its camel
    casing): used for package-private lookup tables."""
    return name.camel
