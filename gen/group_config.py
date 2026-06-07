#!/usr/bin/env python3
"""Group structural configuration for the code generator.

Owns the mutable sets that the XSD parser populates during parsing (passed by
reference via ParseConfig), the static dicts that control group synthesis, and
the group_class_name helper. Static config lives in cpp/config.toml under
[groups]; mutable sets remain here (they must be Python objects passed by ref).
"""
import os
import tomllib

from parse import pascal

_CPP_DIR = os.path.join(os.path.dirname(__file__), "cpp")
with open(os.path.join(_CPP_DIR, "config.toml"), "rb") as _f:
    _CFG = tomllib.load(_f)

_GRP = _CFG["groups"]

# ---------------------------------------------------------------------------
# Mutable sets populated by XsdModel during parsing
# ---------------------------------------------------------------------------

SYNTHETIC_OPTIONAL_GROUPS: set = set()
SYNTHETIC_UNBOUNDED_GROUPS: set = set()
SUPPRESS_GROUP_SUFFIX: set = set()

# ---------------------------------------------------------------------------
# Static group configuration (loaded from TOML)
# ---------------------------------------------------------------------------

NESTED_OPTIONAL_SEQUENCE_AS_GROUP: dict = _GRP["nested_optional_as_group"]
EXTENSION_OPTIONAL_GROUP_RENAME: dict = _GRP["extension_rename"]
UNBOUNDED_SEQUENCE_AS_GROUP: dict = _GRP["unbounded_as_group"]
SYNTHETIC_UNBOUNDED_GROUP_IMPORT_GROUP_AFTER = set(
    _GRP["unbounded_import_group_after"]
)
GENERATE_GROUPS = set(_GRP["generate"])
WRAPPING_STREAMCONTENTS = set(_GRP["wrapping_streamcontents"])
GROUPS_WITH_REAL_FROM_X_ELEMENT = set(_GRP["real_from_x"])

# ---------------------------------------------------------------------------
# Group class name resolution
# ---------------------------------------------------------------------------


def group_class_name(group_name: str) -> str:
    if group_name in SUPPRESS_GROUP_SUFFIX:
        return pascal(group_name)
    return pascal(group_name) + "Group"
