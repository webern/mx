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
