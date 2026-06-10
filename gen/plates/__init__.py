"""The Plates: the template-facing, per-target projection of the IR.

See gen.plates.model for the data shape, gen.plates.build for the projection,
and docs/ai/design/plates.md for the design.
"""

from gen.plates.build import PlatesError, build_plates
from gen.plates.model import Plates

__all__ = ["Plates", "PlatesError", "build_plates", "build_for_config"]


def build_for_config(config_path):
    """The whole pipeline for one target, shared by the CLI and tests: load
    the config, lower its pinned XSD to the IR, apply companion patches, and
    project. Returns (plates, config)."""
    from gen.config import load as load_config
    from gen.ir.build import build_ir
    from gen.xsd.parser import parse

    cfg = load_config(config_path)
    if cfg.xsd is None:
        raise FileNotFoundError(f"config has no [input] xsd: {cfg.path}")
    m = build_ir(parse(cfg.xsd), source=cfg.xsd.stem)
    if cfg.sounds_xml is not None:
        from gen.ir.sounds import patch_sounds, read_sound_ids

        patch_sounds(m, read_sound_ids(cfg.sounds_xml))
    return build_plates(m, cfg), cfg
