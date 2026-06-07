#!/usr/bin/env python3
"""Per-element and per-attribute behavioral overrides for the code generator.

These tables capture hand-applied decisions from the original codegen that
deviate from what the XSD alone would produce. They exist because mx/core
embeds historical conventions (specific default values, always-true hasContents,
xmlns preservation) that predate this generator. Making them explicit and
separate from the generation logic is a step toward a fully data-driven pipeline.
"""
import os
import tomllib

_CPP_DIR = os.path.join(os.path.dirname(__file__), "cpp")
with open(os.path.join(_CPP_DIR, "config.toml"), "rb") as _f:
    _CFG = tomllib.load(_f)

# Per-attribute default value override, keyed by (attrs struct name, camelCase
# attribute field name). The value is the literal C++ initializer expression.
# Source of truth: [overrides.attr_default] in cpp/config.toml.
ATTR_DEFAULT_OVERRIDE = {
    (struct, field): value
    for struct, fields in _CFG["overrides"]["attr_default"].items()
    for field, value in fields.items()
}

# Source of truth: [overrides] in cpp/config.toml.
XMLNS_PRESERVING_ATTRS = set(_CFG["overrides"]["xmlns_preserving_attrs"])

# Source of truth: [overrides.child_init_value] in cpp/config.toml.
CHILD_INIT_VALUE_OVERRIDE = {
    (parent, child): value
    for parent, children in _CFG["overrides"]["child_init_value"].items()
    for child, value in children.items()
}

# Source of truth: [overrides] has_contents_always_true in cpp/config.toml.
ELEMENT_HAS_CONTENTS_ALWAYS_TRUE = set(
    _CFG["overrides"]["has_contents_always_true"]
)

# Per-(element-name, child-xml-name) override for min_occurs. Used when XSD
# group inlining propagates minOccurs=0 from the enclosing group to an element
# that HEAD treats as unconditionally present.
CHILD_MIN_OCCURS_OVERRIDE = {}
