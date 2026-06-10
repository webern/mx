"""Post-projection collision detection (design section 7).

After tokenizing, recasing, renames, and reserved-word/validity mangling, two
distinct wire names can collapse to one identifier. The IR's "no element-name
collisions" invariant guarantees nothing here, because these collisions are
induced by the projection. Each scope is checked in the convention the target
actually uses (the identifiers were already produced in it); every report
names the scope, the colliding wire names, and the shared identifier --
enough to write a targeted rename to resolve it.
"""

from __future__ import annotations

from gen.plates.model import ComplexPlate, EnumPlate, Plates, UnionPlate


def run_checks(plates: Plates) -> list[str]:
    errors: list[str] = []
    _check_type_idents(plates, errors)
    _check_variants(plates, errors)
    _check_members(plates, errors)
    _check_file_stems(plates, errors)
    return errors


def _collisions(pairs: list[tuple[str, str]]) -> list[tuple[str, list[str]]]:
    """Group (identifier, wire) pairs; return identifiers claimed by more
    than one distinct wire name, with their claimants."""
    by_ident: dict[str, list[str]] = {}
    for ident, wire in pairs:
        by_ident.setdefault(ident, []).append(wire)
    return [
        (ident, wires)
        for ident, wires in by_ident.items()
        if len(set(wires)) > 1
    ]


def _check_type_idents(plates: Plates, errors: list[str]) -> None:
    pairs = [
        (p.ident, p.name.wire)
        for p in list(plates.value_types) + list(plates.complex_types)
    ]
    for ident, wires in _collisions(pairs):
        errors.append(
            f"type identifier collision: {sorted(set(wires))} all project to '{ident}'"
        )


def _check_variants(plates: Plates, errors: list[str]) -> None:
    for p in plates.value_types:
        if isinstance(p, EnumPlate):
            pairs = [(v.ident, repr(v.wire)) for v in p.variants]
        elif isinstance(p, UnionPlate):
            pairs = [
                (v.ident, repr(v.wire))
                for m in p.members
                if m.literals
                for v in m.literals
            ]
        else:
            continue
        for ident, wires in _collisions(pairs):
            errors.append(
                f"variant identifier collision in '{p.name.wire}': "
                f"{sorted(set(wires))} all project to '{ident}'"
            )


def _check_members(plates: Plates, errors: list[str]) -> None:
    for p in plates.complex_types:
        for label, members in (("members", p.members), ("all_members", p.all_members)):
            if not members:
                continue
            pairs = [(m.ident, f"{m.kind} {m.name.wire!r}") for m in members]
            for ident, wires in _collisions(pairs):
                errors.append(
                    f"member identifier collision in '{p.name.wire}' ({label}): "
                    f"{sorted(set(wires))} all project to '{ident}'"
                )


def _check_file_stems(plates: Plates, errors: list[str]) -> None:
    """File stems are compared case-insensitively: `Note` and `note` are
    distinct identifiers but the same file on macOS/Windows filesystems."""
    if not plates.files:
        return
    pairs = [(spec.file.lower(), spec.file) for spec in plates.files]
    by_lower: dict[str, set[str]] = {}
    for lower, stem in pairs:
        by_lower.setdefault(lower, set()).add(stem)
    seen: dict[str, list[str]] = {}
    for spec in plates.files:
        seen.setdefault(spec.file.lower(), []).extend(spec.types)
    for lower, stems in by_lower.items():
        types = seen[lower]
        if len(types) > 1:
            errors.append(
                f"file stem collision (case-insensitive): types {sorted(types)} "
                f"all land in '{sorted(stems)[0]}'"
            )
