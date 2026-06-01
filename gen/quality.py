#!/usr/bin/env python3
"""quality.py - design-quality score for the gen/ generator code.

Produces a single composite score (0-100, higher = better) plus a per-axis
breakdown and a worklist of the worst offenders. Built as a regression detector
and a refactor compass for the M6 "better-gen" milestone: the score must rise as
the generator is restructured, and the offender lists tell an agent where to work
next.

This script measures; it does not gate. The pass/fail floor lives in the
Makefile (GEN_QUALITY_FLOOR). Lint is a separate concern handled by `make
gen-lint` (pylint), deliberately kept out of this rubric.

Scored code: every gen/*.py except this file. Globbing (rather than a hardcoded
list) means a new module cannot hide complexity from the score.

Rubric (composite 0-100, higher = better):
    structure   50%   monoliths: LOC-weighted function size + file size
    cyclomatic  25%   independent paths per function (radon)
    cognitive   25%   nesting / readability per function (cognitive_complexity)

Every axis uses the same smooth, bounded, ungameable transform:
    per-item score s = target / max(target, value)        # 1.0 at/under target
    axis score     = 100 * sum(loc_i * s_i) / sum(loc_i)  # LOC-weighted mean
So a partial win (CC 208 -> 30) registers even before it crosses the target,
and a stub-spam can't flatter the score because tiny items carry little weight.

Setup: requires radon and cognitive_complexity importable by the interpreter
running this file. In the mx-sdk container that is /opt/quality-venv/bin/python.

Usage:
    python3 gen/quality.py                # score, write report, print summary
    python3 gen/quality.py --floor 41.0   # also show delta vs the floor
"""

import argparse
import ast
import json
import sys
from pathlib import Path

from radon.complexity import cc_visit
from cognitive_complexity.api import get_cognitive_complexity

REPO = Path(__file__).resolve().parent.parent
GEN = REPO / "gen"
OUT_DIR = REPO / "data" / "testOutput" / "gen-quality"

# This file is the measurer; it never scores itself.
SELF = Path(__file__).resolve().name

# Targets: at or below these a function/file is "good" and scores 1.0; above,
# the score decays smoothly. CC 10 and cognitive 7 are the tools' conventional
# thresholds; the size targets are conventional "small unit" sizes.
TARGET_CC = 10
TARGET_COGNITIVE = 7
TARGET_FN_LOC = 40
TARGET_FILE_LOC = 400

WEIGHTS = {"structure": 0.50, "cyclomatic": 0.25, "cognitive": 0.25}
OFFENDERS_PER_AXIS = 30


def smooth(value, target):
    """Bounded decay: 1.0 at or under target, target/value above it."""
    return target / max(target, value)


def loc_weighted(items, score_of):
    """LOC-weighted mean of per-item scores, x100. items: (loc, value) pairs."""
    total = sum(loc for loc, _ in items)
    if total == 0:
        return 100.0
    return 100.0 * sum(loc * score_of(value) for loc, value in items) / total


def scored_files():
    return sorted(p for p in GEN.glob("*.py") if p.resolve().name != SELF)


def flatten_radon(blocks):
    """Flatten radon cc_visit output to every function/method, keyed by lineno."""
    out = {}
    for b in blocks:
        methods = getattr(b, "methods", None)
        if methods is not None:          # Class
            out.update(flatten_radon(methods))
        else:                            # Function
            out[b.lineno] = b.complexity
            out.update(flatten_radon(getattr(b, "closures", [])))
    return out


def collect():
    """Return (functions, files). functions: list of per-function metric dicts."""
    functions = []
    files = []
    for path in scored_files():
        src = path.read_text()
        rel = path.relative_to(REPO).as_posix()
        files.append({"ref": rel, "loc": len(src.splitlines())})

        tree = ast.parse(src)
        cc_by_line = flatten_radon(cc_visit(src))
        for node in ast.walk(tree):
            if not isinstance(node, (ast.FunctionDef, ast.AsyncFunctionDef)):
                continue
            loc = (node.end_lineno or node.lineno) - node.lineno + 1
            try:
                cognitive = get_cognitive_complexity(node)
            except Exception:            # pragma: no cover - defensive
                cognitive = 0
            functions.append({
                "ref": f"{path.relative_to(REPO).as_posix()}:{node.lineno}",
                "name": node.name,
                "loc": loc,
                "cc": cc_by_line.get(node.lineno, 0),
                "cognitive": cognitive,
            })
    return functions, files


def score(functions, files):
    fn_items = [(f["loc"], f["loc"]) for f in functions]
    file_items = [(f["loc"], f["loc"]) for f in files]
    structure_fn = loc_weighted(fn_items, lambda v: smooth(v, TARGET_FN_LOC))
    structure_file = loc_weighted(file_items, lambda v: smooth(v, TARGET_FILE_LOC))
    components = {
        "structure": (structure_fn + structure_file) / 2,
        "cyclomatic": loc_weighted(
            [(f["loc"], f["cc"]) for f in functions], lambda v: smooth(v, TARGET_CC)),
        "cognitive": loc_weighted(
            [(f["loc"], f["cognitive"]) for f in functions],
            lambda v: smooth(v, TARGET_COGNITIVE)),
    }
    composite = sum(WEIGHTS[k] * v for k, v in components.items())
    return composite, components, structure_fn, structure_file


def top(functions, key, fields, n=OFFENDERS_PER_AXIS):
    ranked = sorted(functions, key=lambda f: f[key], reverse=True)[:n]
    return [{k: f[k] for k in ("ref", "name", *fields)} for f in ranked]


def build_result(floor):
    functions, files = collect()
    composite, components, structure_fn, structure_file = score(functions, files)
    total_loc = sum(f["loc"] for f in files)
    result = {
        "composite": round(composite, 1),
        "components": {k: round(v, 1) for k, v in components.items()},
        "structure_detail": {
            "function": round(structure_fn, 1),
            "file": round(structure_file, 1),
        },
        "raw": {
            "files": len(files),
            "functions": len(functions),
            "total_loc": total_loc,
            "max_cc": max((f["cc"] for f in functions), default=0),
            "max_cognitive": max((f["cognitive"] for f in functions), default=0),
            "max_fn_loc": max((f["loc"] for f in functions), default=0),
            "largest_file": max(files, key=lambda f: f["loc"], default={"loc": 0})["loc"],
        },
        "offenders": {
            "cyclomatic": top(functions, "cc", ("cc", "loc")),
            "cognitive": top(functions, "cognitive", ("cognitive", "loc")),
            "size": top(functions, "loc", ("loc", "cc", "cognitive")),
        },
    }
    if floor is not None:
        result["floor"] = floor
        result["delta"] = round(composite - floor, 1)
    return result


def render(result):
    """Human-readable report (used for stdout and report.md)."""
    c = result["composite"]
    lines = []
    head = f"gen-quality: {c:.1f} / 100"
    if "floor" in result:
        d = result["delta"]
        head += f"   (floor {result['floor']:.1f}, {d:+.1f})"
    lines += [head, ""]
    for axis, val in result["components"].items():
        w = WEIGHTS[axis]
        extra = ""
        if axis == "structure":
            sd = result["structure_detail"]
            extra = f"   [fn {sd['function']:.1f} / file {sd['file']:.1f}]"
        lines.append(f"  {axis:<11} {val:>6.1f}  x{w:.2f}{extra}")
    raw = result["raw"]
    lines += [
        "",
        f"  {raw['functions']} functions across {raw['files']} files, "
        f"{raw['total_loc']} lines (largest file {raw['largest_file']})",
        f"  max cc {raw['max_cc']}  max cognitive {raw['max_cognitive']}  "
        f"max fn loc {raw['max_fn_loc']}",
        "",
        "Worst offenders (top 5 per axis; full lists in score.json):",
    ]
    for axis, valkey in (("cyclomatic", "cc"), ("cognitive", "cognitive"), ("size", "loc")):
        for o in result["offenders"][axis][:5]:
            lines.append(f"  {axis:<10} {o['ref']:<26} {o['name']:<34} {o[valkey]}")
    return "\n".join(lines) + "\n"


def main():
    ap = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--floor", type=float, default=None,
                    help="floor value for delta display (gating lives in the Makefile)")
    args = ap.parse_args()

    result = build_result(args.floor)

    OUT_DIR.mkdir(parents=True, exist_ok=True)
    (OUT_DIR / "score.json").write_text(json.dumps(result, indent=2) + "\n")
    report = render(result)
    (OUT_DIR / "report.md").write_text("```\n" + report + "```\n")
    sys.stdout.write(report)
    return result


if __name__ == "__main__":
    main()
