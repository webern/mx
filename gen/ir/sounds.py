"""Companion patch: fold sounds.xml into the IR as an open sound enum.

The MusicXML XSD types the instrument-sound element as a bare xs:string. The
standard timbre identifiers it expects ("brass.alphorn", ...) live only in the
separately versioned sounds.xml companion file, not the schema. This patch
reads that file and rewrites the IR so instrument-sound resolves to a sound-id
enumeration unioned with an open string: the standard identifiers become typed
values, while any other string stays valid exactly as the schema allows.

This is the one place the IR depends on an input beyond the XSD, and it runs
only when a target's config names a sounds file (see gen.config). The result
introduces no new IR shape -- it is an ordinary enum plus an ordinary union,
the same shape as font-size (the css-font-size enum unioned with decimal).
"""

from __future__ import annotations

import xml.etree.ElementTree as ET
from pathlib import Path

from gen.ir import model as ir

# The element keeps its name; its new type takes the element's name (the
# MusicXML convention, e.g. element note has type note), and the enumeration of
# identifiers gets a sub-name -- mirroring font-size over css-font-size.
ELEMENT = "instrument-sound"
UNION = "instrument-sound"
ENUM = "sound-id"


def read_sound_ids(path) -> list[str]:
    """The id of every <sound> in a sounds.xml companion file, in document
    order. The file's DOCTYPE points at an external DTD; ElementTree ignores it,
    so this stays offline."""
    root = ET.parse(Path(path)).getroot()
    return [s.get("id") for s in root.findall("sound") if s.get("id")]


def patch_sounds(m: ir.Ir, sound_ids: list[str]) -> int:
    """Fold sound_ids into m in place: add the sound-id enum and instrument-sound
    union, then retype every instrument-sound element from string to that union.
    Returns the number of element occurrences retyped, which must be >= 1."""
    enum = ir.EnumType(
        name=ENUM,
        base="token",
        values=list(sound_ids),
        doc=(
            "Standard MusicXML instrument sound identifiers. The XSD types "
            "instrument-sound as xs:string and lists these values only in the "
            "sounds.xml companion file; the generator injects them here."
        ),
    )
    union = ir.UnionType(
        name=UNION,
        members=[
            ir.UnionMember(ref=ir.Ref(ENUM, "value")),
            ir.UnionMember(ref=ir.Ref("string", "primitive")),
        ],
        doc=(
            "The instrument-sound value: one of the standard sound-id "
            "identifiers, or any other string. The schema leaves the content "
            "open (xs:string), so the string member is intrinsic, not a fallback."
        ),
    )
    # Deps-first invariant: the enum (no value deps) precedes the union that
    # references it, and nothing already in the list references either, so
    # appending the pair keeps value_types topologically ordered.
    m.value_types.append(enum)
    m.value_types.append(union)

    # instrument-sound is declared inside the virtual-instrument-data group, not
    # a complex type's content, so retype across groups as well as complex types.
    new_type = ir.Ref(UNION, "value")
    retyped = sum(
        _retype(ct.content, new_type) for ct in m.complex_types if ct.content is not None
    )
    retyped += sum(_retype(g.content, new_type) for g in m.groups)
    if retyped == 0:
        raise ValueError(f"no {ELEMENT!r} element found to patch; schema changed?")

    _bump_stats(m.stats, len(sound_ids))
    return retyped


def _retype(particle: ir.Particle, new_type: ir.Ref) -> int:
    """Retype every ELEMENT occurrence reachable in particle. GroupRef leaves are
    left alone -- their target group is retyped where it is defined."""
    if isinstance(particle, (ir.Sequence, ir.Choice)):
        return sum(_retype(i, new_type) for i in particle.items)
    if isinstance(particle, ir.Element) and particle.name == ELEMENT:
        particle.type = new_type
        return 1
    return 0


def _bump_stats(stats: dict, n_ids: int) -> None:
    stats["value_types"] = stats.get("value_types", 0) + 2
    kinds = stats.setdefault("value_kinds", {})
    kinds["enum"] = kinds.get("enum", 0) + 1
    kinds["union"] = kinds.get("union", 0) + 1
    stats["companion_sound_ids"] = n_ids
