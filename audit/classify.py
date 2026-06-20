"""Classify api round-trip failures by root cause.

Reads the dump directory produced by ``make dump-api-roundtrip`` (Phase 1,
issue #210): pairs of ``<flat>.expected.xml`` / ``<flat>.actual.xml`` plus a
``<flat>.status`` sidecar for pipeline errors. Cross-references
``data/api.features.xml`` and the per-file ``*.features.xml`` sidecars, assigns
each non-passing file one primary root-cause category plus any secondaries, and
writes a machine-readable JSON report (consumed by Phase 3 ranking) with a
human-readable summary on stdout.

The diff is **multiset-based**, not a positional walk. ``mx::api`` drops and
reorders subtrees by design, so the dominant signal is a *deletion*; a naive
index-by-index walk desynchronizes after the first drop and only its first
divergence is trustworthy. The multiset difference
``Counter(expected) - Counter(actual)`` enumerates **every** dropped element
class in O(n), fully reorder-invariant. See
``docs/ai/design/api-roundtrip-classifier.md`` for the full rationale.

Categories:

    A  already-passing  expected/actual identical (defensive; not normally dumped)
    B  drop-only        every missing element class has support="none"
    C  reorder-only     same tag multiset, but a parent's child order differs
    D  enum-bug         text/attr value is a known missing enum member
    E  missing-attribute  a partial feature's attribute was dropped
    F  pipeline-error   LOADFAIL / GETDATAFAIL / CREATEFAIL (no actual produced)
    unknown            a FAIL that matched none of the above
"""

from __future__ import annotations

import difflib
import json
import sys
import xml.etree.ElementTree as ET
from collections import Counter
from dataclasses import dataclass, field
from datetime import datetime, timezone
from pathlib import Path

from . import FEATURES_SUFFIX, featuresfile

# Filename suffixes written by the dump harness (#210).
_EXPECTED_SUFFIX = ".expected.xml"
_ACTUAL_SUFFIX = ".actual.xml"
_STATUS_SUFFIX = ".status"

# Pipeline-error status codes (no actual document was produced).
_PIPELINE_STATUSES = frozenset({"LOADFAIL", "GETDATAFAIL", "CREATEFAIL"})

# Human-readable category labels for the stdout summary.
_CATEGORY_LABELS = {
    "A": "already passing",
    "B": "drop-only divergence",
    "C": "reorder-only divergence",
    "D": "enum bug",
    "E": "missing attribute/element",
    "F": "pipeline error",
    "unknown": "unknown",
}

# Categories that are actionable feature gaps (ranked in the worklist).
_ACTIONABLE = frozenset({"B", "D", "E"})


# --------------------------------------------------------------------------- #
# api.features.xml support index
# --------------------------------------------------------------------------- #


@dataclass
class ApiFeature:
    """One ``<feature>`` from ``data/api.features.xml`` with support detail."""

    name: str
    support: str  # "full" / "partial" / "none" / "" (unknown)
    attributes: dict[str, str] = field(default_factory=dict)  # attr name -> support
    enum_missing: set[str] = field(default_factory=set)  # normalized missing members


def _normalize_enum(value: str | None) -> str:
    """Bridge XML kebab-case values and api/core camelCase enum symbols.

    ``sharp-sharp`` (XML text) and ``sharpSharp`` (enum member) both normalize to
    ``sharpsharp`` so they compare equal.
    """
    if not value:
        return ""
    return value.strip().lower().replace("-", "").replace("_", "")


def load_api_features(path: Path) -> dict[str, ApiFeature]:
    """Parse ``api.features.xml`` into a name -> ApiFeature map."""
    features: dict[str, ApiFeature] = {}
    root = ET.parse(path).getroot()
    for feature in root.findall("./features/feature"):
        name = feature.get("name", "")
        if not name:
            continue
        feat = ApiFeature(name=name, support=feature.get("support", ""))
        for attr in feature.findall("./attributes/attribute"):
            aname = attr.get("name", "")
            if aname:
                feat.attributes[aname] = attr.get("support", "")
        for enum in feature.findall("./enums/enum"):
            for missing in enum.findall("./missing"):
                feat.enum_missing.add(_normalize_enum(missing.text))
        features[name] = feat
    return features


# --------------------------------------------------------------------------- #
# Dump directory discovery
# --------------------------------------------------------------------------- #


@dataclass
class DumpEntry:
    """One corpus file's presence in the dump directory."""

    rel: str  # corpus-relative path, forward slashes (e.g. "lysuite/Saltarello.xml")
    expected: Path | None = None
    actual: Path | None = None
    status_code: str | None = None  # from the .status sidecar, if any


def _flat_to_rel(flat: str) -> str:
    """Reverse the dump harness's path flattening: ``a__b.xml`` -> ``a/b.xml``."""
    return flat.replace("__", "/")


def parse_dump_dir(dump_dir: Path) -> list[DumpEntry]:
    """Group the dump directory's files into one DumpEntry per corpus file."""
    entries: dict[str, DumpEntry] = {}

    def _entry(flat: str) -> DumpEntry:
        return entries.setdefault(flat, DumpEntry(rel=_flat_to_rel(flat)))

    for p in sorted(dump_dir.iterdir()):
        if not p.is_file():
            continue
        name = p.name
        if name.endswith(_EXPECTED_SUFFIX):
            _entry(name[: -len(_EXPECTED_SUFFIX)]).expected = p
        elif name.endswith(_ACTUAL_SUFFIX):
            _entry(name[: -len(_ACTUAL_SUFFIX)]).actual = p
        elif name.endswith(_STATUS_SUFFIX):
            _entry(name[: -len(_STATUS_SUFFIX)]).status_code = (
                p.read_text(encoding="utf-8").strip() or None
            )
    return [entries[k] for k in sorted(entries)]


# --------------------------------------------------------------------------- #
# XML helpers and the positional first-divergence walk
# --------------------------------------------------------------------------- #


def _local(tag: str) -> str:
    """Strip a Clark-notation namespace ({uri}local -> local)."""
    if tag.startswith("{"):
        return tag.rsplit("}", 1)[1]
    return tag


def tag_multiset(root: ET.Element) -> Counter:
    """Count every element's local tag name in the tree (the multiset primitive)."""
    return Counter(_local(el.tag) for el in root.iter())


def _values_equiv(left: str, right: str) -> bool:
    """Mirror corert ``isEquivalent``: numeric values compare by value."""
    if left == right:
        return True
    try:
        return abs(float(left) - float(right)) < 1e-8
    except (ValueError, OverflowError):
        return False


@dataclass
class Divergence:
    """The first positional divergence, retained for continuity (not completeness)."""

    mismatch_type: str  # element-name / text / attribute / attribute-count / child-count
    path: str
    element: str  # leaf local name at the divergence
    expected_value: str | None = None
    actual_value: str | None = None
    attr_name: str | None = None
    missing_attrs: list[str] = field(default_factory=list)
    parent_expected_children: list[str] = field(default_factory=list)
    parent_actual_children: list[str] = field(default_factory=list)


def first_divergence(exp: ET.Element, act: ET.Element, path: str) -> Divergence | None:
    """Find the first positional divergence, mirroring ``compareElements``.

    ``path`` includes ``exp`` itself. Tags of ``exp``/``act`` are assumed equal
    (the caller pairs them); the root pair is the only entry point.
    """
    et = (exp.text or "").strip()
    at = (act.text or "").strip()
    if not _values_equiv(et, at):
        return Divergence("text", path, _local(exp.tag), expected_value=et, actual_value=at)

    ea = {_local(k): v for k, v in exp.attrib.items()}
    aa = {_local(k): v for k, v in act.attrib.items()}
    for aname in sorted(set(ea) & set(aa)):
        if not _values_equiv(ea[aname], aa[aname]):
            return Divergence(
                "attribute", path, _local(exp.tag),
                expected_value=ea[aname], actual_value=aa[aname], attr_name=aname,
            )
    missing_attrs = sorted(set(ea) - set(aa))
    extra_attrs = sorted(set(aa) - set(ea))
    if missing_attrs or extra_attrs:
        return Divergence(
            "attribute-count", path, _local(exp.tag),
            attr_name=missing_attrs[0] if missing_attrs else None,
            missing_attrs=missing_attrs,
        )

    ec = [c for c in exp if isinstance(c.tag, str)]
    ac = [c for c in act if isinstance(c.tag, str)]
    exp_children = [_local(c.tag) for c in ec]
    act_children = [_local(c.tag) for c in ac]
    for i in range(min(len(ec), len(ac))):
        if exp_children[i] != act_children[i]:
            return Divergence(
                "element-name", f"{path}/{exp_children[i]}", exp_children[i],
                parent_expected_children=exp_children, parent_actual_children=act_children,
            )
        deeper = first_divergence(ec[i], ac[i], f"{path}/{exp_children[i]}")
        if deeper is not None:
            return deeper
    if len(ec) != len(ac):
        return Divergence(
            "child-count", path, _local(exp.tag),
            parent_expected_children=exp_children, parent_actual_children=act_children,
        )
    return None


def _is_permutation(a: list[str], b: list[str]) -> bool:
    """True if the two child-tag sequences are a pure reorder (same multiset)."""
    if Counter(a) != Counter(b) or a == b:
        return False
    # Confirm via difflib opcodes: a permutation has no shared-multiset 'replace'
    # that adds/removes a tag (handled by the Counter check above); this just
    # asserts the sequences genuinely differ in order.
    return any(tag != "equal" for tag, *_ in difflib.SequenceMatcher(a=a, b=b).get_opcodes())


# --------------------------------------------------------------------------- #
# Per-file classification
# --------------------------------------------------------------------------- #


def _blank_record(rel: str) -> dict:
    """A per-file record with every field present (filled in by the caller)."""
    return {
        "file": rel,
        "status": None,
        "primary_category": None,
        "secondary_categories": [],
        "first_divergence_element": None,
        "first_divergence_path": None,
        "mismatch_type": None,
        "missing_elements": [],
        "missing_element_counts": {},
        "distinct_missing_count": 0,
        "added_elements": [],
        "is_single_blocker": False,
        "total_missing_instances": 0,
        "blocking_features": [],
        "pipeline_error_kind": None,
    }


def classify_entry(
    entry: DumpEntry,
    api_features: dict[str, ApiFeature],
    data_root: Path,
    warn,
) -> dict:
    """Classify one dump entry into a per-file JSON record."""
    rec = _blank_record(entry.rel)

    # Pipeline error: only an expected file (and maybe a .status sidecar).
    if entry.actual is None:
        kind = entry.status_code if entry.status_code in _PIPELINE_STATUSES else None
        rec["status"] = kind or "PIPELINE_ERROR"
        rec["primary_category"] = "F"
        rec["pipeline_error_kind"] = kind
        return rec

    if entry.expected is None:
        warn(f"{entry.rel}: actual present but no expected; skipping")
        rec["status"] = "FAIL"
        rec["primary_category"] = "unknown"
        return rec

    rec["status"] = "FAIL"
    try:
        exp_root = ET.parse(entry.expected).getroot()
        act_root = ET.parse(entry.actual).getroot()
    except ET.ParseError as exc:
        warn(f"{entry.rel}: XML parse error ({exc}); marking unknown")
        rec["primary_category"] = "unknown"
        return rec

    # Layer 1: multiset tag diff -- the complete, reorder-invariant inventory.
    missing = tag_multiset(exp_root) - tag_multiset(act_root)
    added = tag_multiset(act_root) - tag_multiset(exp_root)
    rec["missing_elements"] = sorted(missing)
    rec["missing_element_counts"] = {k: missing[k] for k in sorted(missing)}
    rec["distinct_missing_count"] = len(missing)
    rec["added_elements"] = sorted(added)
    rec["total_missing_instances"] = sum(missing.values())
    rec["is_single_blocker"] = len(missing) == 1

    # First positional divergence (continuity only; the harness reports the same).
    div = first_divergence(exp_root, act_root, f"/{_local(exp_root.tag)}")
    if div is not None:
        rec["mismatch_type"] = div.mismatch_type
        rec["first_divergence_element"] = div.element
        rec["first_divergence_path"] = div.path

    # No divergence and nothing dropped/added -> the file actually round-trips.
    if div is None and not missing and not added:
        rec["status"] = "PASS"
        rec["primary_category"] = "A"
        return rec

    def support_of(tag: str) -> str | None:
        feat = api_features.get(tag)
        return feat.support if feat else None

    cats: list[str] = []

    # B -- drop-only: provable across the whole file via the multiset.
    if missing and not added and all(support_of(t) == "none" for t in missing):
        cats.append("B")

    # C -- reorder-only: same global multiset, a parent's child order differs.
    if (
        not missing
        and not added
        and div is not None
        and div.mismatch_type in ("element-name", "child-count")
        and _is_permutation(div.parent_expected_children, div.parent_actual_children)
    ):
        cats.append("C")

    # D -- enum bug: a value mismatch that is a known missing enum member.
    if div is not None and div.mismatch_type in ("text", "attribute"):
        feat = api_features.get(div.element)
        if (
            feat is not None
            and feat.support == "partial"
            and feat.enum_missing
            and _normalize_enum(div.expected_value) in feat.enum_missing
        ):
            cats.append("D")

    # E -- missing attribute: a partial feature's modeled attribute was dropped.
    if div is not None and div.mismatch_type in ("attribute", "attribute-count"):
        feat = api_features.get(div.element)
        if (
            feat is not None
            and feat.support == "partial"
            and div.attr_name is not None
            and div.attr_name in feat.attributes
        ):
            cats.append("E")

    # Primary = first match in priority order; the rest are secondary.
    primary = next((c for c in ("B", "C", "D", "E") if c in cats), None)
    if primary is None:
        warn(f"{entry.rel}: unclassified FAIL (missing={rec['missing_elements']}, "
             f"mismatch={rec['mismatch_type']})")
        rec["primary_category"] = "unknown"
    else:
        rec["primary_category"] = primary
        rec["secondary_categories"] = [c for c in cats if c != primary]

    # Blocking features: what, if fully supported, would unblock this file.
    if primary == "B":
        rec["blocking_features"] = sorted(missing)
    elif primary in ("D", "E") and div is not None and div.element:
        rec["blocking_features"] = [div.element]

    # Cross-reference the per-file sidecar: a missing or unreadable sidecar is a
    # warning, never an abort. When present, sanity-check that the divergence
    # element really is part of the file's surface (catches namespace/local-name
    # drift between the dump XML and the audited surface).
    sidecar = (data_root / entry.rel).with_suffix("")
    sidecar = sidecar.with_name(sidecar.name + FEATURES_SUFFIX)
    if not sidecar.exists():
        warn(f"{entry.rel}: no feature sidecar at {sidecar}; cross-reference skipped")
    else:
        try:
            surface = featuresfile.read(sidecar)
            if div is not None and div.element and div.element not in surface.elements:
                warn(f"{entry.rel}: divergence element '{div.element}' not in file surface")
        except ET.ParseError as exc:
            warn(f"{entry.rel}: unreadable feature sidecar ({exc}); cross-reference skipped")

    return rec


# --------------------------------------------------------------------------- #
# Aggregation, ranking, output
# --------------------------------------------------------------------------- #


def _rank_blocking_features(records: list[dict]) -> list[tuple[str, int, int]]:
    """Rank actionable (B/D/E) blocking features by files unblocked.

    Returns (feature, files_unblocked, single_blocker_count) sorted descending by
    files unblocked, then by single-blocker count, then name.
    """
    files = Counter()
    single = Counter()
    for rec in records:
        if rec["primary_category"] not in _ACTIONABLE:
            continue
        blockers = rec["blocking_features"]
        for feat in set(blockers):
            files[feat] += 1
        if rec["is_single_blocker"] and len(blockers) == 1:
            single[blockers[0]] += 1
    ranked = [(f, files[f], single[f]) for f in files]
    ranked.sort(key=lambda t: (-t[1], -t[2], t[0]))
    return ranked


def build_report(
    dump_dir: Path, data_root: Path, api_features_path: Path, warn
) -> dict:
    """Run classification over the dump directory and return the JSON report."""
    api_features = load_api_features(api_features_path)
    entries = parse_dump_dir(dump_dir)
    records = [classify_entry(e, api_features, data_root, warn) for e in entries]

    by_category: Counter = Counter(r["primary_category"] for r in records)
    return {
        "dump_dir": str(dump_dir.resolve()),
        "data_root": str(data_root.resolve()),
        "generated": datetime.now(timezone.utc).isoformat(),
        "summary": {
            "total": len(records),
            "by_category": dict(sorted(by_category.items())),
        },
        "files": records,
    }


def print_summary(report: dict, out_path: Path) -> None:
    """Print the human-readable summary to stdout."""
    records = report["files"]
    counts = report["summary"]["by_category"]
    total = report["summary"]["total"]
    print(f"Classified {total} files from {report['dump_dir']}\n")

    for cat in ("A", "B", "C", "D", "E", "F", "unknown"):
        n = counts.get(cat, 0)
        if n == 0 and cat == "A":
            continue
        marker = "?" if cat == "unknown" else cat
        print(f"  {marker}  {_CATEGORY_LABELS[cat]:<28}{n:>4}")

    ranked = _rank_blocking_features(records)
    if ranked:
        print("\nTop blocking features (ranked by files unblocked; B+D+E):")
        for feat, files, single in ranked[:15]:
            print(f"  {feat:<24}{files:>4} files   ({single} single-blocker)")

    print(f"\nOutput: {out_path}")


def run(dump_dir: Path, data_root: Path, out_path: Path) -> int:
    """Entry point for the ``classify`` subcommand."""
    if not dump_dir.is_dir():
        print(f"classify: dump dir not found: {dump_dir}", file=sys.stderr)
        return 2
    api_features_path = data_root / "api.features.xml"
    if not api_features_path.exists():
        print(f"classify: api.features.xml not found: {api_features_path}", file=sys.stderr)
        return 2

    def warn(msg: str) -> None:
        print(f"classify: warning: {msg}", file=sys.stderr)

    report = build_report(dump_dir, data_root, api_features_path, warn)

    out_path.parent.mkdir(parents=True, exist_ok=True)
    out_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")

    print_summary(report, out_path)
    return 0
