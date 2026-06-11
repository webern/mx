"""Build render contexts from the plates.

The press is pure Mustache, so everything a template branches on or prints
must arrive as data. This module converts the plates into plain dicts with
three mechanical enrichments -- none of which makes a decision, language or
otherwise:

  1. Discriminant expansion: every closed enumerated field (`kind`,
     `category`, `cardinality`, `strategy`, `shape`, `node`, ...) gets a
     boolean companion per vocabulary value (`kind: "enum"` -> `is_enum:
     True`, `is_number: False`, ...). All flags are materialized so the
     engine's strict mode never trips on a legitimate branch.
  2. Quoted companions: every string field gets `<field>_q`, a double-quoted
     backslash-escaped literal (JSON repertoire, non-ASCII as \\uXXXX --
     valid verbatim in C, C++, Go, Java, JavaScript, and Rust).
  3. Loop metadata: every list item gets `is_first` / `is_last` / `index0`;
     items that are bare strings are lifted to `{value, value_q, ...}` so the
     metadata has somewhere to live.

Plus the pre-split member views templates iterate (attributes / elements /
value, own and merged), a `type` self-reference so inner scopes can reach
plate-level fields, and the generated-file banner text.
"""

from __future__ import annotations

import dataclasses
import json

from gen.names import Name
from gen.plates.model import (
    ComplexPlate,
    Plates,
    attribute_members,
    element_members,
    value_member,
)
from gen.press.writer import banner

# The closed vocabularies, by field name. A value outside its field's
# vocabulary is a build bug, so it fails loud here.
_DISCRIMINANTS: dict[str, tuple[str, ...]] = {
    "kind": (
        "enum", "number", "string", "union", "complex",
        "attribute", "element", "value",
        "primitive-decimal", "primitive-integer", "primitive-string",
    ),
    "category": ("complex", "value", "primitive"),
    "cardinality": ("required", "optional", "vector"),
    "strategy": (
        "enum-class", "numeric-wrapper", "string-wrapper", "tagged-variant",
        "value-class", "composite-class", "flag", "attrs-class",
        "inherit", "flatten",
    ),
    "shape": ("value", "composite", "empty", "derived"),
    "node": ("element", "sequence", "choice", "group"),
    "variant_scope": ("bare", "composed"),
}


def quoted(value: str) -> str:
    return json.dumps(value, ensure_ascii=True)


def _flag(value: str) -> str:
    return "is_" + value.replace("-", "_")


def _convert(obj):
    """Dataclasses to dicts, recursively, with the enrichments applied."""
    if isinstance(obj, Name):
        # Casings flatten onto the name so templates say {{name.snake}}.
        out = {"wire": obj.wire, "wire_q": quoted(obj.wire)}
        for convention, ident in obj.cased.items():
            out[convention] = ident
            out[convention + "_q"] = quoted(ident)
        return out
    if dataclasses.is_dataclass(obj):
        out: dict = {}
        for f in dataclasses.fields(obj):
            name = f.name
            value = getattr(obj, name)
            out[name] = _convert(value)
            if isinstance(value, str):
                out[name + "_q"] = quoted(value)
                vocab = _DISCRIMINANTS.get(name)
                if vocab is not None:
                    if value not in vocab:
                        raise ValueError(
                            f"{type(obj).__name__}.{name} = {value!r} is outside "
                            f"its vocabulary {vocab}"
                        )
                    for v in vocab:
                        out[_flag(v)] = v == value
        return out
    if isinstance(obj, (list, tuple)):
        return _listify([_convert(item) for item in obj])
    if isinstance(obj, dict):
        return {k: _convert(v) for k, v in obj.items()}
    return obj


def _listify(items: list) -> list:
    """Attach loop metadata; lift bare scalars so it has somewhere to live."""
    out = []
    last = len(items) - 1
    for i, item in enumerate(items):
        if not isinstance(item, dict):
            item = {"value": item}
            if isinstance(item["value"], str):
                item["value_q"] = quoted(item["value"])
        else:
            item = dict(item)
        item["is_first"] = i == 0
        item["is_last"] = i == last
        item["index0"] = i
        out.append(item)
    return out


def _common(plates: Plates) -> dict:
    return {
        "target": _convert(plates.target),
        "vars": dict(plates.target.vars),
        "schema_version": plates.schema_version,
        "source": plates.source,
        "generated_banner": banner(plates.source),
    }


def plate_context(plates: Plates, plate) -> dict:
    """The context a per-type template renders against: the plate's fields,
    the member views, the target facts, and a `type` self-reference so inner
    scopes (a member loop, a variant loop) can still reach plate fields that
    their own frame shadows."""
    ctx = _convert(plate)
    if isinstance(plate, ComplexPlate):
        ctx["attributes"] = _listify(
            [_convert(m) for m in attribute_members(plate.members)]
        )
        ctx["elements"] = _listify(
            [_convert(m) for m in element_members(plate.members)]
        )
        value = value_member(plate.members)
        ctx["value"] = _convert(value) if value is not None else None
        merged = plate.all_members if plate.all_members is not None else plate.members
        ctx["merged_attributes"] = _listify(
            [_convert(m) for m in attribute_members(merged)]
        )
        ctx["merged_elements"] = _listify(
            [_convert(m) for m in element_members(merged)]
        )
        merged_value = value_member(merged)
        ctx["merged_value"] = _convert(merged_value) if merged_value is not None else None
    ctx.update(_common(plates))
    ctx["type"] = ctx
    return ctx


def target_context(plates: Plates, outputs: list[str]) -> dict:
    """The context a once-per-target template renders against: every plate,
    the roots, and the full output manifest (`outputs`, plus `outputs_by_ext`
    grouped by final extension so a build manifest can list just its
    sources)."""
    ctx = _common(plates)
    ctx["value_types"] = _listify([plate_context(plates, p) for p in plates.value_types])
    ctx["complex_types"] = _listify(
        [plate_context(plates, p) for p in plates.complex_types]
    )
    ctx["roots"] = _convert(list(plates.roots))
    paths = sorted(outputs)
    ctx["outputs"] = _listify([{"path": p, "path_q": quoted(p)} for p in paths])
    by_ext: dict[str, list] = {}
    for p in paths:
        ext = p.rsplit(".", 1)[-1] if "." in p else ""
        by_ext.setdefault(ext, []).append({"path": p, "path_q": quoted(p)})
    ctx["outputs_by_ext"] = {ext: _listify(items) for ext, items in by_ext.items()}
    return ctx
