#!/usr/bin/env python3
"""Score wrapper (partwise/timewise) flavor configuration.

Per-flavor knobs that capture hand-written variations between the partwise
and timewise families. Source of truth: [score_wrapper] in cpp/config.toml.
"""
import os
import tomllib

_CPP_DIR = os.path.join(os.path.dirname(__file__), "cpp")
with open(os.path.join(_CPP_DIR, "config.toml"), "rb") as _f:
    _CFG = tomllib.load(_f)

SCORE_WRAPPER_FLAVOR_CONFIG = _CFG["score_wrapper"]
