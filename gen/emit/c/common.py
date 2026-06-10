"""C grammar helpers shared by the C templates.

Language spelling only: string literal quoting, block doc comments, the
header/impl file frames, include guards, and the composition rules C uses
for scoped names (functions are prefix_snake_verb, enum constants are
PREFIX_TYPE_VARIANT). Identifiers and casings arrive resolved on the plates.
"""

from __future__ import annotations

from gen.emit.writer import banner
from gen.plates.model import Name, Plates

_ESCAPES = {
    "\\": "\\\\",
    '"': '\\"',
    "\n": "\\n",
    "\t": "\\t",
    "\r": "\\r",
}


def c_string(s: str) -> str:
    out = ['"']
    for ch in s:
        if ch in _ESCAPES:
            out.append(_ESCAPES[ch])
        elif ord(ch) < 0x20:
            out.append(f"\\{ord(ch):03o}")
        else:
            out.append(ch)  # generated files are UTF-8; pass non-ASCII through
    out.append('"')
    return "".join(out)


def doc_comment(text: str | None, wrap: int) -> list[str]:
    """Wrap schema documentation into a block comment. Comment-closing
    sequences in the text are defused."""
    if not text:
        return []
    words = text.replace("*/", "*\\/").split()
    if not words:
        return []
    lines = ["/*"]
    current = " *"
    for word in words:
        if len(current) + 1 + len(word) > wrap and current != " *":
            lines.append(current)
            current = " *"
        current += " " + word
    lines.append(current)
    lines.append(" */")
    return lines


def fn_prefix(plates: Plates) -> str:
    """The function-name prefix: the symbol prefix lowered (Mx -> mx_)."""
    return plates.target.prefix.lower() + "_"


def const_prefix(plates: Plates) -> str:
    """The constant-name prefix: the symbol prefix screamed (Mx -> MX_)."""
    return plates.target.prefix.upper() + "_"


def fn_name(plates: Plates, type_name: Name, verb: str) -> str:
    return f"{fn_prefix(plates)}{type_name.snake}_{verb}"


def guard(stem: str) -> str:
    # The _INCLUDED suffix keeps guards out of the constant namespace the
    # plates' collision gate certifies (MX_STEP_H could be a real variant).
    return stem.upper() + "_H_INCLUDED"


def header_file(plates: Plates, stem: str, body: list[str], includes: list[str]) -> str:
    lines = [f"/* {banner(plates.source)} */", ""]
    lines += [f"#ifndef {guard(stem)}", f"#define {guard(stem)}", ""]
    for inc in includes:
        lines.append(f"#include {inc}")
    if includes:
        lines.append("")
    lines += body
    lines += ["", f"#endif /* {guard(stem)} */"]
    return "\n".join(lines).rstrip("\n") + "\n"


def impl_file(plates: Plates, stem: str, body: list[str], includes: list[str]) -> str:
    lines = [f"/* {banner(plates.source)} */", ""]
    lines += [f'#include "{stem}.h"', ""]
    for inc in includes:
        lines.append(f"#include {inc}")
    if includes:
        lines.append("")
    lines += body
    return "\n".join(lines).rstrip("\n") + "\n"
