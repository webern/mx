"""The C calling convention for value types and primitives.

The single place that knows how generated C code parses, prints, stores, and
frees a value of any plate kind or primitive -- the complex-type templates
call this at every attribute, text body, and leaf child instead of
re-deriving the ownership rules inline:

  - enum:   value type; to_string returns static storage (not owned)
  - number: value type; to_string returns malloc'd storage (owned)
  - string: the value IS an owned char*; printing it costs nothing
  - union:  struct value; to_string malloc'd; the value itself may own
            memory (mx_<t>_free releases it)
  - primitives follow their family (decimal/integer/string)
"""

from __future__ import annotations

from dataclasses import dataclass

from gen.plates.model import PlateRef, Plates
from gen.emit.c.common import fn_name, fn_prefix


@dataclass
class CValue:
    """How generated C handles one value type: expression templates take the
    value (or source string) as `{0}`."""

    c_type: str  # storage type for a by-value field
    parse: str  # lenient parse from a const char *
    to_string: str  # the wire spelling of a value
    to_string_owned: bool  # the to_string result must be freed
    free: str | None  # statement template releasing a value's OWN memory
    is_pointer_value: bool = False  # the value IS a pointer (char* family):
    # a child field stores it directly and discriminates on it, no boxing


_PRIM_FAMILY = {
    "decimal": "decimal",
    "integer": "integer",
    "positive_integer": "integer",
    "non_negative_integer": "integer",
    "string": "string",
    "token": "string",
    "nmtoken": "string",
    "date": "string",
}


def value_api(plates: Plates, ref: PlateRef) -> CValue:
    p = fn_prefix(plates)
    if ref.category == "primitive":
        family = _PRIM_FAMILY[ref.wire]
        if family == "decimal":
            return CValue("double", f"{p}parse_decimal({{0}})",
                          f"{p}format_decimal({{0}})", True, None)
        if family == "integer":
            return CValue("long", f"{p}parse_int({{0}})",
                          f"{p}format_int({{0}})", True, None)
        return CValue("char *", f"{p}strdup({{0}})", "{0}", False, "free({0});",
                      is_pointer_value=True)

    plate = plates.plate(ref.wire)
    parse = fn_name(plates, plate.name, "parse") + "({0})"
    to_string = fn_name(plates, plate.name, "to_string") + "({0})"
    if plate.kind == "enum":
        return CValue(ref.ident, parse, to_string, False, None)
    if plate.kind == "number":
        return CValue(ref.ident, parse, to_string, True, None)
    if plate.kind == "string":
        # The typedef is char*: the value is its own spelling and owns itself.
        return CValue(ref.ident, parse, "{0}", False, "free({0});",
                      is_pointer_value=True)
    if plate.kind == "union":
        free = fn_name(plates, plate.name, "free") + "(&{0});"
        return CValue(ref.ident, parse, to_string, True, free)
    raise ValueError(f"not a value reference: {ref.wire} ({plate.kind})")
