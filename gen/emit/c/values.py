"""C templates for the four value shapes (the leaf node types).

One template per shape, mirroring the Go backend's surface in C idiom. Each
type renders as a header/impl pair (the documented one-FileId-to-two-files
mapping for C):

    bool  mx_<t>_try_parse(const char *s, MxT *out)   strict membership
    MxT   mx_<t>_parse(const char *s)                 lenient (fixup policies)
    to_string                                         the wire spelling:
        enums return static storage (const char *, do not free);
        numbers and unions return malloc'd strings the caller frees;
        string types ARE char* values (parse returns a malloc'd copy).

Leniency policies match the Go backend: unknown enum literal -> first
variant; unparseable number -> 0; every number clamps into its declared
range with primitive-implied lower bounds and exclusive bounds clamping to
the nearest representable value (decimal: +/- 1e-6).
"""

from __future__ import annotations

from gen.emit.c.common import c_string, doc_comment, fn_name, fn_prefix, header_file, impl_file
from gen.plates.model import (
    EnumPlate,
    NumberPlate,
    Plates,
    StringPlate,
    UnionPlate,
)

# IR primitive -> (C type, strict-try helper suffix, format helper suffix).
# A string-family primitive has no strict form: any string belongs to it.
_PRIM = {
    "decimal": ("double", "try_parse_decimal", "format_decimal"),
    "integer": ("long", "try_parse_int", "format_int"),
    "positive_integer": ("long", "try_parse_int", "format_int"),
    "non_negative_integer": ("long", "try_parse_int", "format_int"),
    "string": ("char *", None, None),
    "token": ("char *", None, None),
    "nmtoken": ("char *", None, None),
    "date": ("char *", None, None),
}

_IMPLIED_MIN = {"positive_integer": 1, "non_negative_integer": 0}


def value_files(plates: Plates, plate, includes: list[str], rt: str) -> tuple[str, str]:
    """Render one value plate into its (header, impl) pair. `includes` is the
    plate's dependency stems (from Plates.files); `rt` the runtime stem."""
    if isinstance(plate, EnumPlate):
        header, impl = _enum(plates, plate, rt)
    elif isinstance(plate, NumberPlate):
        header, impl = _number(plates, plate, rt)
    elif isinstance(plate, StringPlate):
        header, impl = _string(plates, plate, rt)
    elif isinstance(plate, UnionPlate):
        header, impl = _union(plates, plate, includes, rt)
    else:
        raise TypeError(f"not a value plate: {plate!r}")
    return header, impl


def _fn(plates: Plates, plate, verb: str) -> str:
    return fn_name(plates, plate.name, verb)


# --------------------------------------------------------------------------- #
# enum-class
# --------------------------------------------------------------------------- #


def _enum(plates: Plates, plate: EnumPlate, rt: str):
    ident = plate.ident
    wrap = plates.target.doc_style.wrap
    try_parse = _fn(plates, plate, "try_parse")
    parse = _fn(plates, plate, "parse")
    to_string = _fn(plates, plate, "to_string")
    values = fn_name(plates, plate.name, "values")

    decl = doc_comment(plate.doc, wrap)
    decl += ["typedef enum {"]
    decl += [f"    {v.ident} = {i}," for i, v in enumerate(plate.variants[:-1])]
    decl += [f"    {plate.variants[-1].ident} = {len(plate.variants) - 1}"]
    decl += [f"}} {ident};", ""]
    decl += [
        f"bool {try_parse}(const char *s, {ident} *out);",
        "/* Lenient: unknown input falls back to the first variant. */",
        f"{ident} {parse}(const char *s);",
        "/* Returns the wire literal (static storage; do not free). */",
        f"const char *{to_string}({ident} v);",
    ]

    body = [f"static const char *const {values}[] = {{"]
    body += [f"    {c_string(v.wire)}," for v in plate.variants]
    body += ["};", ""]
    body += [
        f"bool {try_parse}(const char *s, {ident} *out) {{",
        f"    for (size_t i = 0; i < sizeof({values}) / sizeof({values}[0]); i++) {{",
        f"        if (strcmp(s, {values}[i]) == 0) {{",
        f"            *out = ({ident})i;",
        "            return true;",
        "        }",
        "    }",
        f"    *out = ({ident})0;",
        "    return false;",
        "}",
        "",
        f"{ident} {parse}(const char *s) {{",
        f"    {ident} v;",
        f"    {try_parse}(s, &v);",
        "    return v;",
        "}",
        "",
        f"const char *{to_string}({ident} v) {{",
        f"    if ((size_t)v >= sizeof({values}) / sizeof({values}[0]))",
        f"        return {values}[0];",
        f"    return {values}[v];",
        "}",
    ]
    return (
        header_file(plates, plate.file, decl, ["<stdbool.h>"]),
        impl_file(plates, plate.file, body, ["<string.h>"]),
    )


# --------------------------------------------------------------------------- #
# numeric-wrapper
# --------------------------------------------------------------------------- #


def _c_int(literal: str) -> str:
    return str(int(float(literal)))


def _c_float(literal: str) -> str:
    return repr(float(literal))


def _clamp_steps(plate: NumberPlate) -> list[tuple[str, str, str]]:
    """(comparison op, bound literal, replacement literal) per active bound,
    mirroring the Go backend's policy exactly."""
    is_int = plate.target_type != "double"
    b = plate.bounds
    steps: list[tuple[str, str, str]] = []

    lows: list[tuple[float, bool, str]] = []
    if b.min_inclusive is not None:
        lows.append((float(b.min_inclusive), False, b.min_inclusive))
    if b.min_exclusive is not None:
        lows.append((float(b.min_exclusive), True, b.min_exclusive))
    implied = _IMPLIED_MIN.get(plate.base)
    if implied is not None:
        lows.append((float(implied), False, str(implied)))
    if lows:
        value, exclusive, literal = max(lows, key=lambda t: (t[0], t[1]))
        if is_int:
            bound = _c_int(literal)
            steps.append(("<=" if exclusive else "<",
                          bound,
                          str(int(float(literal)) + 1) if exclusive else bound))
        else:
            bound = _c_float(literal)
            steps.append(("<=" if exclusive else "<",
                          bound,
                          repr(float(literal) + 1e-6) if exclusive else bound))

    highs: list[tuple[float, bool, str]] = []
    if b.max_inclusive is not None:
        highs.append((float(b.max_inclusive), False, b.max_inclusive))
    if b.max_exclusive is not None:
        highs.append((float(b.max_exclusive), True, b.max_exclusive))
    if highs:
        value, exclusive, literal = min(highs, key=lambda t: (t[0], -t[1]))
        if is_int:
            bound = _c_int(literal)
            steps.append((">=" if exclusive else ">",
                          bound,
                          str(int(float(literal)) - 1) if exclusive else bound))
        else:
            bound = _c_float(literal)
            steps.append((">=" if exclusive else ">",
                          bound,
                          repr(float(literal) - 1e-6) if exclusive else bound))
    return steps


def _number(plates: Plates, plate: NumberPlate, rt: str):
    ident = plate.ident
    wrap = plates.target.doc_style.wrap
    c_type, try_helper, fmt_helper = _PRIM[plate.base]
    prefix = fn_prefix(plates)
    try_parse = _fn(plates, plate, "try_parse")
    parse = _fn(plates, plate, "parse")
    to_string = _fn(plates, plate, "to_string")
    clamp = _fn(plates, plate, "clamp")
    steps = _clamp_steps(plate)
    runtime_try = prefix + try_helper
    runtime_fmt = prefix + fmt_helper
    runtime_parse = prefix + ("parse_decimal" if c_type == "double" else "parse_int")

    decl = doc_comment(plate.doc, wrap)
    decl += [f"typedef {c_type} {ident};", ""]
    decl += [
        "/* Strict parse, then clamps into the declared range. */",
        f"bool {try_parse}(const char *s, {ident} *out);",
        "/* Lenient: unparseable input becomes 0, then clamps. */",
        f"{ident} {parse}(const char *s);",
        "/* Malloc'd; caller frees. */",
        f"char *{to_string}({ident} v);",
    ]

    body = []
    if steps:
        body += [f"static {ident} {clamp}({c_type} v) {{"]
        for op, bound, repl in steps:
            body += [f"    if (v {op} {bound})", f"        v = {repl};"]
        body += [f"    return ({ident})v;", "}", ""]
        convert = f"{clamp}(v)"
    else:
        convert = f"({ident})v"
    body += [
        f"bool {try_parse}(const char *s, {ident} *out) {{",
        f"    {c_type} v;",
        f"    if (!{runtime_try}(s, &v)) {{",
        f"        *out = ({ident})0;",
        "        return false;",
        "    }",
        f"    *out = {convert};",
        "    return true;",
        "}",
        "",
        f"{ident} {parse}(const char *s) {{",
        f"    {c_type} v = {runtime_parse}(s);",
        f"    return {convert};",
        "}",
        "",
        f"char *{to_string}({ident} v) {{",
        f"    return {runtime_fmt}(({c_type})v);",
        "}",
    ]
    return (
        header_file(plates, plate.file, decl, ["<stdbool.h>"]),
        impl_file(plates, plate.file, body, [f'"{rt}.h"']),
    )


# --------------------------------------------------------------------------- #
# string-wrapper
# --------------------------------------------------------------------------- #


def _string(plates: Plates, plate: StringPlate, rt: str):
    ident = plate.ident
    wrap = plates.target.doc_style.wrap
    parse = _fn(plates, plate, "parse")
    prefix = fn_prefix(plates)

    doc = plate.doc or ""
    if plate.patterns:
        doc = (doc + " Pattern (not enforced): " + " | ".join(plate.patterns)).strip()
    decl = doc_comment(doc, wrap)
    decl += [
        f"typedef char *{ident};",
        "",
        "/* Malloc'd copy of the wire string; the value IS its spelling. */",
        f"{ident} {parse}(const char *s);",
    ]
    body = [
        f"{ident} {parse}(const char *s) {{",
        f"    return {prefix}strdup(s);",
        "}",
    ]
    return (
        header_file(plates, plate.file, decl, []),
        impl_file(plates, plate.file, body, [f'"{rt}.h"']),
    )


# --------------------------------------------------------------------------- #
# tagged-variant
# --------------------------------------------------------------------------- #


def _union_cases(plates: Plates, plate: UnionPlate):
    """Flatten union members into kind cases, mirroring the Go backend:
    (kind const, field name or None, C type, try spelling or None for
    any-string, lenient-parse spelling, to-string spelling, owns-memory)."""
    fconv = plates.target.field_convention
    prefix = plates.target.prefix.upper() + "_" if plates.target.prefix else ""
    kind_base = f"{prefix}{plate.name.screaming}_KIND_"
    fn_pre = fn_prefix(plates)
    cases = []
    for m in plate.members:
        if m.ref is not None:
            field = m.name.cased[fconv]
            kind = kind_base + m.name.screaming
            if m.ref.category == "value":
                member_fns = lambda verb, n=m.name: fn_name(plates, n, verb)
                target_plate = plates.plate(m.ref.wire)
                if target_plate.kind == "enum":
                    to_s = f"{fn_pre}strdup({member_fns('to_string')}(v.{field}))"
                elif target_plate.kind == "string":
                    to_s = f"{fn_pre}strdup(v.{field})"
                else:
                    to_s = f"{member_fns('to_string')}(v.{field})"
                cases.append({
                    "kind": kind, "field": field, "c_type": m.ref.ident,
                    "try": f"{member_fns('try_parse')}(s, &v.{field})"
                    if target_plate.kind != "string" else None,
                    "parse": f"{member_fns('parse')}(s)",
                    "to_string": to_s,
                    "owns": target_plate.kind == "string",
                })
            else:  # primitive
                c_type, try_helper, fmt_helper = _PRIM[m.ref.wire]
                if try_helper is None:  # open string member: any input matches
                    cases.append({
                        "kind": kind, "field": field, "c_type": "char *",
                        "try": None,
                        "parse": f"{fn_pre}strdup(s)",
                        "to_string": f"{fn_pre}strdup(v.{field})",
                        "owns": True,
                    })
                else:
                    cases.append({
                        "kind": kind, "field": field, "c_type": c_type,
                        "try": f"{fn_pre}{try_helper}(s, &v.{field})",
                        "parse": f"{fn_pre}{('parse_decimal' if c_type == 'double' else 'parse_int')}(s)",
                        "to_string": f"{fn_pre}{fmt_helper}(v.{field})",
                        "owns": False,
                    })
        else:
            for variant in m.literals or []:
                cases.append({
                    "kind": kind_base + variant.name.screaming,
                    "field": None, "c_type": None,
                    "try": f"strcmp(s, {c_string(variant.wire)}) == 0",
                    "parse": None,
                    "to_string": f"{fn_pre}strdup({c_string(variant.wire)})",
                    "owns": False,
                })
    return cases


def _union(plates: Plates, plate: UnionPlate, includes: list[str], rt: str):
    ident = plate.ident
    wrap = plates.target.doc_style.wrap
    try_parse = _fn(plates, plate, "try_parse")
    parse = _fn(plates, plate, "parse")
    to_string = _fn(plates, plate, "to_string")
    free = _fn(plates, plate, "free")
    cases = _union_cases(plates, plate)

    decl = doc_comment(plate.doc, wrap)
    decl += ["typedef enum {"]
    decl += [f"    {c['kind']} = {i}," for i, c in enumerate(cases[:-1])]
    decl += [f"    {cases[-1]['kind']} = {len(cases) - 1}"]
    decl += [f"}} {ident}Kind;", ""]
    decl += ["typedef struct {", f"    {ident}Kind kind;"]
    for c in cases:
        if c["field"] is not None:
            sep = "" if c["c_type"].endswith("*") else " "
            decl += [f"    {c['c_type']}{sep}{c['field']};"]
    decl += [f"}} {ident};", ""]
    decl += [
        "/* Tries each union member in schema order. */",
        f"bool {try_parse}(const char *s, {ident} *out);",
        "/* Lenient: an unmatched input is absorbed by the first member. */",
        f"{ident} {parse}(const char *s);",
        "/* The wire spelling of whichever member is held. Malloc'd. */",
        f"char *{to_string}({ident} v);",
        f"void {free}({ident} *v);",
    ]

    body = [
        f"bool {try_parse}(const char *s, {ident} *out) {{",
        f"    {ident} v;",
        "    memset(&v, 0, sizeof(v));",
    ]
    open_ended = False
    for c in cases:
        if c["try"] is None:  # open string member: always matches
            body += [
                f"    v.kind = {c['kind']};",
                f"    v.{c['field']} = {c['parse']};",
                "    *out = v;",
                "    return true;",
            ]
            open_ended = True
            break
        if c["field"] is None:
            body += [
                f"    if ({c['try']}) {{",
                f"        v.kind = {c['kind']};",
                "        *out = v;",
                "        return true;",
                "    }",
            ]
        else:
            body += [
                f"    if ({c['try']}) {{",
                f"        v.kind = {c['kind']};",
                "        *out = v;",
                "        return true;",
                "    }",
            ]
    if not open_ended:
        body += ["    *out = v;", "    return false;"]
    body += ["}", ""]

    first = cases[0]
    body += [
        f"{ident} {parse}(const char *s) {{",
        f"    {ident} v;",
        f"    if ({try_parse}(s, &v))",
        "        return v;",
        "    memset(&v, 0, sizeof(v));",
        f"    v.kind = {first['kind']};",
    ]
    if first["field"] is not None:
        body += [f"    v.{first['field']} = {first['parse']};"]
    body += ["    return v;", "}", ""]

    body += [f"char *{to_string}({ident} v) {{", "    switch (v.kind) {"]
    for c in cases[1:]:
        body += [f"    case {c['kind']}:", f"        return {c['to_string']};"]
    body += [
        "    default:",
        f"        return {first['to_string']};",
        "    }",
        "}",
        "",
        f"void {free}({ident} *v) {{",
        "    if (!v)",
        "        return;",
    ]
    owning = [c for c in cases if c["owns"]]
    for c in owning:
        body += [
            f"    if (v->kind == {c['kind']}) {{",
            f"        free(v->{c['field']});",
            f"        v->{c['field']} = NULL;",
            "    }",
        ]
    body += ["}"]

    header_includes = ["<stdbool.h>"] + [f'"{inc}.h"' for inc in includes]
    return (
        header_file(plates, plate.file, decl, header_includes),
        impl_file(plates, plate.file, body, [f'"{rt}.h"', "<stdlib.h>", "<string.h>"]),
    )
