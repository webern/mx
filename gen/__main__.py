"""mx code generator entry point.

Usage:
  python3 -m gen <config.toml>              emit code for the target the config describes
  python3 -m gen analyze [xsd]              parse the XSD and print a structural analysis
  python3 -m gen ir [--type N] [--resolve] [--config C] [xsd]
                                            lower the XSD to the IR and print it as JSON;
                                            --resolve prints the collapsed (group-spliced,
                                            attribute-flattened) view of complex types;
                                            --config applies a target's companion patches
                                            (e.g. the sounds.xml fold) before dumping
  python3 -m gen plates --config C [--type N] [--check]
                                            project the IR onto the target the config
                                            describes and print the Plates as JSON;
                                            --check validates renames and detects
                                            identifier collisions, exiting non-zero on
                                            any failure (a CI gate, like analyze)
  python3 -m gen render --config C --type N
                                            render one type through the target's
                                            templates to stdout (template debugging)

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
    from gen.ir.dump import resolved_view, to_json
    from gen.ir.resolve import Resolver

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
    ir = _lower(xsd, cfg)

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


def _lower(xsd: Path, cfg):
    """Lower an XSD to the IR, applying a config's companion patches (today:
    the sounds.xml fold). One definition, shared by every command."""
    from gen.ir.build import build_ir
    from gen.xsd.parser import parse

    ir = build_ir(parse(xsd), source=xsd.stem)
    if cfg is not None and cfg.sounds_xml is not None:
        from gen.ir.sounds import patch_sounds, read_sound_ids

        patch_sounds(ir, read_sound_ids(cfg.sounds_xml))
    return ir


def _plates(args: list[str]) -> int:
    from gen.ir.dump import to_json
    from gen.plates import PlatesError

    config_path = None
    type_name = None
    check = False
    i = 0
    while i < len(args):
        if args[i] == "--config" and i + 1 < len(args):
            config_path = args[i + 1]
            i += 2
        elif args[i] == "--type" and i + 1 < len(args):
            type_name = args[i + 1]
            i += 2
        elif args[i] == "--check":
            check = True
            i += 1
        else:
            print(f"error: unexpected argument: {args[i]}", file=sys.stderr)
            return 2
    if config_path is None:
        print("error: plates requires --config <config.toml>", file=sys.stderr)
        return 2

    from gen.plates import build_for_config

    try:
        plates, _ = build_for_config(config_path)
    except PlatesError as e:
        for line in e.errors:
            print(f"error: {line}", file=sys.stderr)
        return 1

    if check:
        # Rename validation and collision detection already ran in the build;
        # reaching here means the projection is clean.
        print(
            f"plates ok: {len(plates.value_types)} value types, "
            f"{len(plates.complex_types)} complex types"
        )
        return 0

    if type_name:
        if not plates.has_plate(type_name):
            print(f"error: type not found in plates: {type_name}", file=sys.stderr)
            return 1
        print(to_json(plates.plate(type_name)))
        return 0

    print(to_json(plates))
    return 0


def _emit(config_path: str) -> int:
    from gen.config import ConfigError
    from gen.plates import PlatesError, build_for_config
    from gen.press.engine import PressError
    from gen.press.render import RenderError, render_target

    try:
        plates, cfg = build_for_config(config_path)
        result = render_target(plates, cfg)
    except PlatesError as e:
        for line in e.errors:
            print(f"error: {line}", file=sys.stderr)
        return 1
    except (
        ConfigError,
        FileNotFoundError,
        PressError,
        RenderError,
        RuntimeError,
        ValueError,
    ) as e:
        print(f"error: {e}", file=sys.stderr)
        return 1
    print(result.summary())
    return 0


def _render_debug(args: list[str]) -> int:
    from gen.plates import PlatesError, build_for_config
    from gen.press.engine import PressError
    from gen.press.render import RenderError, render_files

    config_path = None
    type_name = None
    i = 0
    while i < len(args):
        if args[i] == "--config" and i + 1 < len(args):
            config_path = args[i + 1]
            i += 2
        elif args[i] == "--type" and i + 1 < len(args):
            type_name = args[i + 1]
            i += 2
        else:
            print(f"error: unexpected argument: {args[i]}", file=sys.stderr)
            return 2
    if config_path is None or type_name is None:
        print("error: render requires --config <config.toml> --type <name>",
              file=sys.stderr)
        return 2
    try:
        plates, cfg = build_for_config(config_path)
        if cfg.render is None:
            print(f"error: config has no [render] manifest: {cfg.path}",
                  file=sys.stderr)
            return 1
        if not plates.has_plate(type_name):
            print(f"error: type not found in plates: {type_name}", file=sys.stderr)
            return 1
        plate = plates.plate(type_name)
        files = render_files(plates, cfg)
        from gen.press.render import _expand

        shown = 0
        for entry in cfg.render.types:
            if plate.strategy in entry.strategies:
                path = _expand(entry.output, plate)
                print(f"==== {path} (from {entry.template})")
                print(files[path], end="")
                shown += 1
        if not shown:
            print(f"error: no manifest entry renders strategy "
                  f"'{plate.strategy}'", file=sys.stderr)
            return 1
    except PlatesError as e:
        for line in e.errors:
            print(f"error: {line}", file=sys.stderr)
        return 1
    except (PressError, RenderError, FileNotFoundError, ValueError) as e:
        print(f"error: {e}", file=sys.stderr)
        return 1
    return 0


def main(argv: list[str]) -> int:
    if not argv:
        print(__doc__, file=sys.stderr)
        return 2
    if argv[0] == "analyze":
        return _analyze(argv[1:])
    if argv[0] == "ir":
        return _ir(argv[1:])
    if argv[0] == "plates":
        return _plates(argv[1:])
    if argv[0] == "render":
        return _render_debug(argv[1:])
    if argv[0].endswith(".toml"):
        return _emit(argv[0])
    print(f"error: unknown command: {argv[0]}", file=sys.stderr)
    return 2


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
