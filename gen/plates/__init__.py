"""The Plates: the template-facing, per-target projection of the IR.

See gen.plates.model for the data shape, gen.plates.build for the projection,
and docs/ai/design/plates.md for the design.
"""

from gen.plates.build import PlatesError, build_plates
from gen.plates.model import Plates

__all__ = ["Plates", "PlatesError", "build_plates"]
