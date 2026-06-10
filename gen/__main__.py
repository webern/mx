"""mx code generator entry point.

Usage:
  python3 -m gen <config.toml>              generate code for a target (not yet implemented)
  python3 -m gen analyze [xsd]              parse the XSD and print a structural analysis
  python3 -m gen ir [--type N] [--resolve] [--config C] [xsd]
                                            lower the XSD to the IR and print it as JSON;
                                            --resolve prints the collapsed (group-spliced,
                                            attribute-flattened) view of complex types;
                                            --config applies a target's companion patches
                                            (e.g. the sounds.xml fold) before dumping

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
    from gen.ir.dump import resolved_view, to_json
    from gen.ir.resolve import Resolver
    from gen.xsd.parser import parse

    type_name = None
    resolve = False
    config_path = None
    rest = []
    i = 0
    while i < len(args):
        if args[i] == "--type" and i + 1 < len(args):
            type_name = args[i + 1]
            i += 2
        elif args[i] == "--resolve":
            resolve = True
            i += 1
        elif args[i] == "--config" and i + 1 < len(args):
            config_path = args[i + 1]
            i += 2
        else:
            rest.append(args[i])
            i += 1

    cfg = None
    if config_path is not None:
        from gen.config import load as load_config

        cfg = load_config(config_path)

    # XSD precedence: an explicit positional argument wins, else the target
    # config's pinned version, else the 4.0 default.
    if rest:
        xsd = Path(rest[0])
    elif cfg is not None and cfg.xsd is not None:
        xsd = cfg.xsd
    else:
        xsd = DEFAULT_XSD
    if not xsd.exists():
        print(f"error: XSD not found: {xsd}", file=sys.stderr)
        return 1
    ir = build_ir(parse(xsd), source=xsd.stem)

    # A target config can fold companion data into the IR before it is consumed:
    # today, the sounds.xml patch (instrument-sound -> open sound enum).
    if cfg is not None and cfg.sounds_xml is not None:
        from gen.ir.sounds import patch_sounds, read_sound_ids

        patch_sounds(ir, read_sound_ids(cfg.sounds_xml))

    resolver = Resolver.from_ir(ir) if resolve else None

    if type_name:
        ct = next((c for c in ir.complex_types if c.name == type_name), None)
        if ct is not None:
            print(to_json(resolved_view(resolver, ct) if resolver else ct))
            return 0
        vt = next((v for v in ir.value_types if v.name == type_name), None)
        if vt is None:
            print(f"error: type not found in IR: {type_name}", file=sys.stderr)
            return 1
        print(to_json(vt))  # value types are already fully resolved
        return 0

    if resolver:
        print(to_json([resolved_view(resolver, c) for c in ir.complex_types]))
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
