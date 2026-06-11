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
    UnionPlate,
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
    "family": ("decimal", "integer"),
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
                        # Vocabularies overlap across fields of one object
                        # (PlateRef has category "value" and kind "enum";
                        # the kind vocabulary also contains "value").
                        # Earlier fields win: category's is_value/is_complex
                        # must not be clobbered by the kind expansion, and
                        # the two always agree where they overlap.
                        out.setdefault(_flag(v), v == value)
            elif isinstance(value, (list, tuple)):
                # Iterating a section already gates emptiness; has_<field>
                # serves the non-iterating tests (wrap-once framing).
                out["has_" + name] = bool(value)
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
    if isinstance(plate, UnionPlate):
        # The flattened case view: one entry per ref member and per literal,
        # in schema order, each carrying its discriminator constant as
        # `tag_ident` -- so loop metadata (ordinals, commas, first-member
        # handling) works on the granularity the kind enum actually has.
        kind_flags = [_flag(v) for v in _DISCRIMINANTS["kind"]]
        cases = []
        for m in plate.members:
            if m.ref is not None:
                ref = _convert(m.ref)
                case = {
                    "is_literal": False,
                    "tag_ident": m.tag.ident,
                    "ref": ref,
                    "name": _convert(m.name),
                    "clamp": _convert(m.clamp),
                    "has_clamp": bool(m.clamp),
                    "wire": None,
                    "wire_q": None,
                }
                # The referenced kind's flags, flattened onto the case so
                # templates branch without reaching through `ref`.
                for flag in kind_flags:
                    case[flag] = ref[flag]
                cases.append(case)
            else:
                for variant in m.literals or []:
                    case = {
                        "is_literal": True,
                        "tag_ident": variant.ident,
                        "ref": None,
                        "name": None,
                        "clamp": [],
                        "has_clamp": False,
                        "wire": variant.wire,
                        "wire_q": quoted(variant.wire),
                    }
                    for flag in kind_flags:
                        case[flag] = False
                    cases.append(case)
        ctx["cases"] = _listify(cases)
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
        for key in ("attributes", "elements", "merged_attributes", "merged_elements"):
            ctx["has_" + key] = bool(ctx[key])
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
