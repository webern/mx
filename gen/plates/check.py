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
    if plates.target.variant_scope == "composed":
        _check_flat_namespace(plates, errors)
    else:
        _check_type_idents(plates, errors)
        _check_variants_per_type(plates, errors)
    _check_members(plates, errors)
    _check_template_reserved(plates, errors)
    _check_file_stems(plates, errors)
    return errors


def _check_template_reserved(plates: Plates, errors: list[str]) -> None:
    """Names the target's TEMPLATES synthesize cannot be gated structurally,
    so the target declares them: [reserved] members (member identifiers its
    templates claim on every struct) and [reserved] type-suffixes
    (compositions appended to type identifiers, like a Child struct). A
    schema name landing on either must fail here, not as a confusing compile
    error in committed output."""
    reserved_members = set(plates.target.reserved_members)
    if reserved_members:
        for p in plates.complex_types:
            for member_list in (p.members, p.all_members or []):
                for m in member_list:
                    if m.ident in reserved_members:
                        errors.append(
                            f"member identifier '{m.ident}' in '{p.name.wire}' is "
                            f"reserved by the target's templates ([reserved] members); "
                            f"rename it"
                        )
    suffixes = plates.target.reserved_type_suffixes
    if suffixes:
        idents = {
            p.ident: p.name.wire
            for p in list(plates.value_types) + list(plates.complex_types)
        }
        for ident, wire in idents.items():
            for suffix in suffixes:
                composed = ident + suffix
                if composed in idents:
                    errors.append(
                        f"type identifier collision: '{idents[composed]}' is named "
                        f"'{composed}', which the target's templates compose from "
                        f"'{wire}' + reserved suffix '{suffix}'"
                    )


def _variant_pairs(plate) -> list[tuple[str, str]]:
    """(ident, claimant description) for every constant a value plate emits:
    enum variants, union literal variants, and union member tags (the
    discriminator constants) alike."""
    if isinstance(plate, EnumPlate):
        return [(v.ident, f"{plate.name.wire}.{v.wire!r}") for v in plate.variants]
    if isinstance(plate, UnionPlate):
        pairs = [
            (v.ident, f"{plate.name.wire}.{v.wire!r}")
            for m in plate.members
            if m.literals
            for v in m.literals
        ]
        pairs += [
            (m.tag.ident, f"{plate.name.wire} member {m.ref.wire!r}")
            for m in plate.members
            if m.tag is not None
        ]
        return pairs
    return []


def _check_flat_namespace(plates: Plates, errors: list[str]) -> None:
    """For a composed variant scope, the target has one identifier namespace:
    type identifiers and every (already composed) enum/literal constant must
    be mutually unique -- this is the namespace the compiler actually sees."""
    pairs = [
        (p.ident, f"type {p.name.wire!r}")
        for p in list(plates.value_types) + list(plates.complex_types)
    ]
    for p in plates.value_types:
        pairs.extend(_variant_pairs(p))
    for ident, claimants in _collisions(pairs):
        errors.append(
            f"identifier collision: {sorted(set(claimants))} all project to '{ident}'"
        )


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


def _check_variants_per_type(plates: Plates, errors: list[str]) -> None:
    """For a bare variant scope, constants live inside their type: uniqueness
    is per enum (or per union's literal set)."""
    for p in plates.value_types:
        pairs = _variant_pairs(p)
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
