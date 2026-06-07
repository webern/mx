#!/usr/bin/env python3
"""Attribute struct naming configuration for the code generator.

Controls which attribute structs get type-based (shared) names vs element-based
names, and provides the resolution function used throughout the generator.
"""
from parse import pascal
from element_config import element_class_name

# Attribute structs that live at the core root level (not in elements/).
CORE_ROOT_ATTRS = {
    "EmptyPrintObjectStyleAlignAttributes",
}

# XSD type name aliases: when the type name for an element matches a key here,
# the aliased type is used for attribute-struct naming purposes.
ATTRS_TYPE_ALIAS = {
    "empty-print-style-align": "empty-print-object-style-align",
}

# Shared attribute structs that live in the elements/ directory but are reused
# across multiple elements (generated once, included by reference).
ELEMENTS_DIR_SHARED_ATTRS = {
    "EmptyPlacementAttributes",
    "EmptyLineAttributes",
    "EmptyTrillSoundAttributes",
    "EmptyFontAttributes",
    "EmptyPrintStyleAlignAttributes",
}


def resolve_attrs_name(elem_name: str, type_name: str, model) -> str:
    """Determine the correct attributes struct name for an element.
    Some empty-* types use the type name (shared). Others use element name."""
    aliased = ATTRS_TYPE_ALIAS.get(type_name, type_name)
    type_attrs = pascal(aliased) + "Attributes"
    if type_attrs in CORE_ROOT_ATTRS or type_attrs in ELEMENTS_DIR_SHARED_ATTRS:
        return type_attrs
    return element_class_name(elem_name) + "Attributes"
