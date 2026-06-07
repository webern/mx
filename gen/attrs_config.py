#!/usr/bin/env python3
"""Attribute struct naming configuration for the code generator.

Controls which attribute structs get type-based (shared) names vs element-based
names, and provides the resolution function used throughout the generator.
Source of truth: [attrs] in cpp/config.toml.
"""
import os
import tomllib

from parse import pascal
from element_config import element_class_name

_CPP_DIR = os.path.join(os.path.dirname(__file__), "cpp")
with open(os.path.join(_CPP_DIR, "config.toml"), "rb") as _f:
    _CFG = tomllib.load(_f)

CORE_ROOT_ATTRS = set(_CFG["attrs"]["core_root"])
ELEMENTS_DIR_SHARED_ATTRS = set(_CFG["attrs"]["shared"])
ATTRS_TYPE_ALIAS = _CFG["attrs"]["type_alias"]


def resolve_attrs_name(elem_name: str, type_name: str, model) -> str:
    """Determine the correct attributes struct name for an element.
    Some empty-* types use the type name (shared). Others use element name."""
    aliased = ATTRS_TYPE_ALIAS.get(type_name, type_name)
    type_attrs = pascal(aliased) + "Attributes"
    if type_attrs in CORE_ROOT_ATTRS or type_attrs in ELEMENTS_DIR_SHARED_ATTRS:
        return type_attrs
    return element_class_name(elem_name) + "Attributes"
