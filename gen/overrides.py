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

# Attribute structs that preserve xmlns:* namespace declarations through
# round-trip (e.g. xmlns:xlink on score-partwise).
XMLNS_PRESERVING_ATTRS = {
    "ScorePartwiseAttributes",
    "ScoreTimewiseAttributes",
    "OpusAttributes",
    "LinkAttributes",
}

# Per-(parent-element, child-element) override for the constructor argument
# passed to make{Child}() on the parent's ctor init list.
CHILD_INIT_VALUE_OVERRIDE = {
    ("scaling", "millimeters"): "MillimetersValue(7)",
    ("scaling", "tenths"): "TenthsValue(40)",
    ("staff-details", "staff-lines"): "NonNegativeInteger(5)",
}

# Elements whose hasContents() should always return true regardless of
# what the XSD min/max-occurs analysis would produce.
ELEMENT_HAS_CONTENTS_ALWAYS_TRUE = {
    "measure-layout",
}

# Per-(element-name, child-xml-name) override for min_occurs. Used when XSD
# group inlining propagates minOccurs=0 from the enclosing group to an element
# that HEAD treats as unconditionally present.
CHILD_MIN_OCCURS_OVERRIDE = {}
