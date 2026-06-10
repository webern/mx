"""Go templates for the four value shapes (the leaf node types).

One template per shape, per the design: enum-class, numeric-wrapper,
string-wrapper, tagged-variant. Every type exposes the same surface so the
complex-type templates can call them uniformly:

    TryParse<T>(s string) (T, bool)   lexically strict: the input must be a
                                      well-formed value of the type's family
                                      (numbers then clamp into range; an
                                      enum literal must match exactly)
    Parse<T>(s string) T              lenient: malformed input degrades
                                      deterministically
    (T) String() string               the wire spelling

The leniency policy itself (clamp bounds, implied minimums, exclusive-bound
epsilon) is DATA on the plates (NumberPlate.clamp / UnionPlateMember.clamp,
documented in data/README.md); this module only spells it in Go.
"""

from __future__ import annotations

from gen.emit.go.common import (
    doc_comment,
    file_frame,
    go_string,
    member_field,
    unexported,
)
from gen.plates.model import (
    EnumPlate,
    NumberPlate,
    Plates,
    StringPlate,
    UnionPlate,
    UnionPlateMember,
)

# IR primitive -> the strict-try helper and serialize spelling in runtime.go.
# A string-family primitive has no strict form: any string belongs to it.
_PRIM_TRY = {
    "decimal": "tryParseDecimal",
    "integer": "tryParseInt",
    "positive_integer": "tryParseInt",
    "non_negative_integer": "tryParseInt",
}
_PRIM_PARSE = {
    "decimal": "parseDecimal",
    "integer": "parseInt",
    "positive_integer": "parseInt",
    "non_negative_integer": "parseInt",
}
_PRIM_FORMAT = {
    "decimal": "formatDecimal({0})",
    "integer": "formatInt({0})",
    "positive_integer": "formatInt({0})",
    "non_negative_integer": "formatInt({0})",
}

def value_file(plates: Plates, plate) -> str:
    if isinstance(plate, EnumPlate):
        body = _enum_body(plates, plate)
    elif isinstance(plate, NumberPlate):
        body = _number_body(plates, plate)
    elif isinstance(plate, StringPlate):
        body = _string_body(plates, plate)
    elif isinstance(plate, UnionPlate):
        body = _union_body(plates, plate)
    else:
        raise TypeError(f"not a value plate: {plate!r}")
    return file_frame(plates, body)


# --------------------------------------------------------------------------- #
# enum-class
# --------------------------------------------------------------------------- #


def _enum_body(plates: Plates, plate: EnumPlate) -> list[str]:
    ident = plate.ident
    wrap = plates.target.doc_style.wrap
    table = unexported(plate.name)
    consts = [v.ident for v in plate.variants]  # final, composed by the plates

    lines = doc_comment(plate.doc, wrap)
    lines += [f"type {ident} int", ""]
    lines += ["const ("]
    lines += [f"\t{consts[0]} {ident} = iota"]
    lines += [f"\t{c}" for c in consts[1:]]
    lines += [")", ""]

    lines += [f"// {table}Values lists the wire literals by variant ordinal."]
    lines += [f"var {table}Values = []string{{"]
    lines += [f"\t{go_string(v.wire)}," for v in plate.variants]
    lines += ["}", ""]

    lines += [f"var {table}Index = map[string]{ident}{{"]
    lines += [f"\t{go_string(v.wire)}: {c}," for v, c in zip(plate.variants, consts)]
    lines += ["}", ""]

    lines += [
        f"// TryParse{ident} matches s against the wire literals.",
        f"func TryParse{ident}(s string) ({ident}, bool) {{",
        f"\tv, ok := {table}Index[s]",
        "\treturn v, ok",
        "}",
        "",
        f"// Parse{ident} is lenient: unknown input falls back to the first variant.",
        f"func Parse{ident}(s string) {ident} {{",
        f"\tif v, ok := {table}Index[s]; ok {{",
        "\t\treturn v",
        "\t}",
        f"\treturn {consts[0]}",
        "}",
        "",
        "// String returns the wire literal.",
        f"func (v {ident}) String() string {{",
        f"\tif int(v) < 0 || int(v) >= len({table}Values) {{",
        f"\t\treturn {table}Values[0]",
        "\t}",
        f"\treturn {table}Values[v]",
        "}",
    ]
    return lines


# --------------------------------------------------------------------------- #
# numeric-wrapper
# --------------------------------------------------------------------------- #


def _number_body(plates: Plates, plate: NumberPlate) -> list[str]:
    ident = plate.ident
    wrap = plates.target.doc_style.wrap
    go_type = plate.target_type
    try_fn = _PRIM_TRY[plate.base]
    parse_fn = _PRIM_PARSE[plate.base]
    fmt = _PRIM_FORMAT[plate.base].format(f"{go_type}(v)")
    steps = plate.clamp

    lines = doc_comment(plate.doc, wrap)
    lines += [f"type {ident} {go_type}", ""]

    if steps:
        convert = f"clamp{ident}(v)"
        clamps = ", then clamps into the declared range"
    else:
        convert = f"{ident}(v)"
        clamps = ""

    lines += [
        f"// TryParse{ident} parses s as a lexically well-formed value{clamps}.",
        f"func TryParse{ident}(s string) ({ident}, bool) {{",
        f"\tv, ok := {try_fn}(s)",
        "\tif !ok {",
        f"\t\treturn {ident}(0), false",
        "\t}",
        f"\treturn {convert}, true",
        "}",
        "",
        f"// Parse{ident} is lenient: unparseable input becomes 0{clamps}.",
        f"func Parse{ident}(s string) {ident} {{",
        f"\tv := {parse_fn}(s)",
        f"\treturn {convert}",
        "}",
        "",
    ]
    if steps:
        lines += [f"func clamp{ident}(v {go_type}) {ident} {{"]
        for step in steps:
            lines += [f"\tif v {step.op} {step.bound} {{", f"\t\tv = {step.replacement}", "\t}"]
        lines += [f"\treturn {ident}(v)", "}", ""]
    lines += [
        "// String returns the wire spelling.",
        f"func (v {ident}) String() string {{",
        f"\treturn {fmt}",
        "}",
    ]
    return lines


# --------------------------------------------------------------------------- #
# string-wrapper
# --------------------------------------------------------------------------- #


def _string_body(plates: Plates, plate: StringPlate) -> list[str]:
    ident = plate.ident
    wrap = plates.target.doc_style.wrap
    doc = plate.doc or ""
    if plate.patterns:
        # Patterns are documented, not enforced: round-trip fidelity wants the
        # input back out, valid or not.
        note = "Pattern (not enforced): " + " | ".join(plate.patterns)
        doc = f"{doc} {note}".strip()

    lines = doc_comment(doc, wrap)
    lines += [
        f"type {ident} string",
        "",
        f"// TryParse{ident} accepts any string: the wire form is the value.",
        f"func TryParse{ident}(s string) ({ident}, bool) {{",
        f"\treturn {ident}(s), true",
        "}",
        "",
        f"func Parse{ident}(s string) {ident} {{",
        f"\treturn {ident}(s)",
        "}",
        "",
        "// String returns the wire spelling.",
        f"func (v {ident}) String() string {{",
        "\treturn string(v)",
        "}",
    ]
    return lines


# --------------------------------------------------------------------------- #
# tagged-variant
# --------------------------------------------------------------------------- #


def _member_cases(plates: Plates, plate: UnionPlate):
    """Flatten union members into kind cases: one per ref member, one per
    literal. The kind constants come final from the plates (member tags and
    literal variants share the enum-variant scoping and collision gate).
    Each case: (kind const, payload field or None, payload Go type,
    strict-try spelling or None for an open string member, lenient-parse
    spelling, to-string spelling, clamp steps)."""
    cases = []
    for m in plate.members:
        if m.ref is not None:
            field = member_field(plates, m.name)
            kind = m.tag.ident
            if m.ref.category == "value":
                cases.append(
                    (kind, field, m.ref.ident,
                     f"TryParse{m.ref.ident}(s)",
                     f"Parse{m.ref.ident}(s)",
                     f"v.{field}.String()", [])
                )
            else:  # primitive
                try_fn = _PRIM_TRY.get(m.ref.wire)
                cases.append(
                    (kind, field, m.ref.ident,
                     f"{try_fn}(s)" if try_fn else None,  # None: any string matches
                     f"{_PRIM_PARSE[m.ref.wire]}(s)" if m.ref.wire in _PRIM_PARSE else "s",
                     _PRIM_FORMAT.get(m.ref.wire, "{0}").format(f"v.{field}"),
                     m.clamp)
                )
        else:
            for variant in m.literals or []:
                cases.append(
                    (variant.ident, None, None, f"s == {go_string(variant.wire)}", None,
                     go_string(variant.wire), [])
                )
    return cases


def _clamp_lines(steps, indent: str) -> list[str]:
    lines = []
    for step in steps:
        lines += [f"{indent}if v {step.op} {step.bound} {{",
                  f"{indent}\tv = {step.replacement}",
                  f"{indent}}}"]
    return lines


def _union_body(plates: Plates, plate: UnionPlate) -> list[str]:
    ident = plate.ident
    wrap = plates.target.doc_style.wrap
    cases = _member_cases(plates, plate)

    lines = doc_comment(plate.doc, wrap)
    lines += [f"type {ident} struct {{", f"\tKind {ident}Kind"]
    for kind, field, go_type, *_ in cases:
        if field is not None:
            lines += [f"\t{field} {go_type}"]
    lines += ["}", ""]

    lines += [f"type {ident}Kind int", "", "const ("]
    lines += [f"\t{cases[0][0]} {ident}Kind = iota"]
    lines += [f"\t{kind}" for kind, *_ in cases[1:]]
    lines += [")", ""]

    lines += [
        f"// TryParse{ident} tries each union member in schema order.",
        f"func TryParse{ident}(s string) ({ident}, bool) {{",
    ]
    open_ended = False
    for i, (kind, field, _go_type, try_expr, _parse, _str, clamp) in enumerate(cases):
        if field is not None and try_expr is None:
            # An open string member matches anything: it must be last, or the
            # members after it could never be reached.
            if i != len(cases) - 1:
                raise ValueError(
                    f"{plate.ident}: union member after an open string member "
                    f"is unreachable"
                )
            lines += [f"\treturn {ident}{{Kind: {kind}, {field}: s}}, true"]
            open_ended = True
            break
        if field is None:
            lines += [
                f"\tif {try_expr} {{",
                f"\t\treturn {ident}{{Kind: {kind}}}, true",
                "\t}",
            ]
        else:
            lines += [f"\tif v, ok := {try_expr}; ok {{"]
            lines += _clamp_lines(clamp, "\t\t")
            lines += [
                f"\t\treturn {ident}{{Kind: {kind}, {field}: v}}, true",
                "\t}",
            ]
    if not open_ended:
        lines += [f"\treturn {ident}{{}}, false"]
    lines += ["}", ""]

    first_kind, first_field, _gt, _try, first_parse, _s, first_clamp = cases[0]
    lines += [
        f"// Parse{ident} is lenient: when no member matches, the first member",
        "// absorbs the input under its own leniency rules.",
        f"func Parse{ident}(s string) {ident} {{",
        f"\tif v, ok := TryParse{ident}(s); ok {{",
        "\t\treturn v",
        "\t}",
    ]
    if first_field is None:
        lines += [f"\treturn {ident}{{Kind: {first_kind}}}"]
    elif first_clamp:
        lines += [f"\tv := {first_parse}"]
        lines += _clamp_lines(first_clamp, "\t")
        lines += [f"\treturn {ident}{{Kind: {first_kind}, {first_field}: v}}"]
    else:
        lines += [f"\treturn {ident}{{Kind: {first_kind}, {first_field}: {first_parse}}}"]
    lines += ["}", ""]

    lines += [
        "// String returns the wire spelling of whichever member is held.",
        f"func (v {ident}) String() string {{",
        "\tswitch v.Kind {",
    ]
    for kind, _field, _gt, _try, _parse, to_str, _clamp in cases[1:]:
        lines += [f"\tcase {kind}:", f"\t\treturn {to_str}"]
    lines += ["\t}", f"\treturn {cases[0][5]}", "}"]
    return lines
