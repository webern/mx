"""Load a target's config.toml into a typed Config.

A target config describes one generation run: where generated code lands and
which optional companion patches to apply before emitting. The IR itself is a
pure function of the schema inputs (see gen.ir); config selects *which* inputs
and where the output goes, never how an individual type is shaped.
"""

from __future__ import annotations

import tomllib
from dataclasses import dataclass
from pathlib import Path


@dataclass
class Config:
    path: Path  # the config file itself, resolved
    xsd: Path | None  # the MusicXML XSD this target generates from, resolved
    output_dir: Path | None  # where generated code lands, resolved
    sounds_xml: Path | None  # companion sounds file to fold in, or None when off


def load(config_path) -> Config:
    """Parse config.toml. Paths inside it are interpreted relative to the config
    file's own directory, so a target's config stays self-contained."""
    path = Path(config_path).resolve()
    if not path.exists():
        raise FileNotFoundError(f"config not found: {path}")
    with open(path, "rb") as f:
        data = tomllib.load(f)
    base = path.parent

    # Each target pins its own MusicXML version: the schema it generates from is
    # part of the target's identity, not a global default.
    xsd = None
    inp = data.get("input", {})
    if inp.get("xsd"):
        xsd = (base / inp["xsd"]).resolve()
        if not xsd.exists():
            raise FileNotFoundError(f"xsd not found: {xsd}")

    output_dir = None
    out = data.get("output", {})
    if out.get("dir"):
        output_dir = (base / out["dir"]).resolve()

    # Companion sounds patch is on iff [sounds] xml names a file (see
    # gen.ir.sounds). Resolve and existence-check it here so a bad path fails at
    # config load, not deep in the lowering.
    sounds_xml = None
    sounds = data.get("sounds", {})
    if sounds.get("xml"):
        sounds_xml = (base / sounds["xml"]).resolve()
        if not sounds_xml.exists():
            raise FileNotFoundError(f"sounds file not found: {sounds_xml}")

    return Config(path=path, xsd=xsd, output_dir=output_dir, sounds_xml=sounds_xml)
