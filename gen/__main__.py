"""mx code generator entry point.

Usage:
  python3 -m gen <config.toml>          generate code for a target (not yet implemented)
  python3 -m gen analyze [xsd]          parse the XSD and print a structural analysis
  python3 -m gen ir [--type N] [xsd]    lower the XSD to the IR and print it as JSON

Reads a MusicXML 4.0 XSD specification and generates typed document
serialization/deserialization code for the target described in the given
config file.
"""

import sys
from pathlib import Path

# The MusicXML version this generator targets, used as the default for analyze.
DEFAULT_XSD = Path(__file__).resolve().parent.parent / "docs" / "musicxml-4.0-ed15c23.xsd"


def _analyze(args: list[str]) -> int:
    from gen.xsd.analyze import report
    from gen.xsd.parser import parse

    xsd = Path(args[0]) if args else DEFAULT_XSD
    if not xsd.exists():
        print(f"error: XSD not found: {xsd}", file=sys.stderr)
        return 1
    print(report(parse(xsd)))
    return 0


def _ir(args: list[str]) -> int:
    from gen.ir.build import build_ir
    from gen.ir.dump import to_json
    from gen.xsd.parser import parse

    type_name = None
    rest = []
    i = 0
    while i < len(args):
        if args[i] == "--type" and i + 1 < len(args):
            type_name = args[i + 1]
            i += 2
        else:
            rest.append(args[i])
            i += 1

    xsd = Path(rest[0]) if rest else DEFAULT_XSD
    if not xsd.exists():
        print(f"error: XSD not found: {xsd}", file=sys.stderr)
        return 1
    ir = build_ir(parse(xsd), source=xsd.stem)

    if type_name:
        match = next((c for c in ir.complex_types if c.name == type_name), None) or next(
            (v for v in ir.value_types if v.name == type_name), None
        )
        if match is None:
            print(f"error: type not found in IR: {type_name}", file=sys.stderr)
            return 1
        print(to_json(match))
    else:
        print(to_json(ir))
    return 0


def main(argv: list[str]) -> int:
    if not argv:
        print(__doc__, file=sys.stderr)
        return 2
    if argv[0] == "analyze":
        return _analyze(argv[1:])
    if argv[0] == "ir":
        return _ir(argv[1:])
    print("error: generator not implemented", file=sys.stderr)
    return 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
