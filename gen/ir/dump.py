"""Serialize the IR to JSON for inspection."""

from __future__ import annotations

import json
from dataclasses import fields, is_dataclass

# Discriminator fields are emitted first so each object announces what it is.
_FIRST = ("kind", "node", "name", "element")


def to_jsonable(obj):
    """Convert IR dataclasses to plain JSON-able data, dropping None and empty
    collections to keep the output readable."""
    if is_dataclass(obj):
        names = [f.name for f in fields(obj)]
        order = [n for n in _FIRST if n in names] + [n for n in names if n not in _FIRST]
        result = {}
        for name in order:
            value = getattr(obj, name)
            if value is None or (isinstance(value, (list, dict)) and not value):
                continue
            result[name] = to_jsonable(value)
        return result
    if isinstance(obj, list):
        return [to_jsonable(x) for x in obj]
    if isinstance(obj, dict):
        return {k: to_jsonable(v) for k, v in obj.items()}
    return obj


def to_json(obj) -> str:
    return json.dumps(to_jsonable(obj), indent=2)


def resolved_view(resolver, ct) -> dict:
    """A complex type as an emitter consumes it: attribute groups flattened into
    one list, model-group refs spliced into the content. The collapsed form the
    Resolver computes, shaped for inspection via `ir --resolve`."""
    view: dict = {"kind": ct.kind, "name": ct.name}
    attrs = resolver.attributes(ct)
    if attrs:
        view["attributes"] = attrs
    if ct.kind == "derived":
        view["base"] = ct.base
        view["all_attributes"] = resolver.all_attributes(ct)
    if ct.value_type:
        view["value_type"] = ct.value_type
    content = resolver.content(ct)
    if content is not None:
        view["content"] = content
    if ct.presence_only:
        view["presence_only"] = True
    if ct.doc:
        view["doc"] = ct.doc
    return view
