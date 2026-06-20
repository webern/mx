"""Classify api round-trip failures by *measured* divergence, and rank a worklist.

Reads the dump directory produced by ``make dump-api-roundtrip`` (Phase 1,
issue #210): pairs of ``<flat>.expected.xml`` / ``<flat>.actual.xml`` plus a
``<flat>.status`` sidecar for crashes. Diffs each pair structurally, records the
set of divergences that keep the file from round-tripping, and writes a
machine-readable JSON report (consumed by Phase 3 ranking, #212) with a
human-readable worklist on stdout.

What this is *not*: it does not consult ``data/api.features.xml`` or any other
record of what someone believed ``mx::api`` "supports". Whether a drop was
intended is a present-day human decision (issue #214), not a property the
classifier asserts. The classifier reports only what the round-trip actually did
to each file, so the worklist is grounded in measured behavior.

## How a file is diffed

The comparison is strict full-DOM (same rule as the api round-trip gate). A file
PASSes only when nothing diverges. Every divergence is reduced to a *signature*,
and a file's **distance** to passing is the count of *unique* signatures it has
(``<foo>`` dropped a thousand times is one ``drop:foo`` signature, distance 1):

    drop:<tag>          a tag present in expected, missing from actual
    add:<tag>           a spurious tag the api invented (present only in actual)
    reorder:<parent>    a parent whose child multiset matches but order differs
    value:<tag>         a paired element whose text value differs
    value:<tag>@<attr>  a paired element whose attribute *value* differs
    attr:<tag>@<attr>   a paired element with an attribute present on one side only

Drops/adds come from a multiset difference (``Counter(expected) - Counter(actual)``)
which is O(n) and reorder-invariant, so it enumerates *every* dropped class, not
just the first (a positional walk desyncs after the first drop). value/attr/reorder
come from an alignment walk (``difflib.SequenceMatcher`` over child-tag sequences)
that pairs surviving elements across drops. See
``docs/ai/design/api-roundtrip-classifier.md``.

## Statuses and the worklist

    PASS   expected == actual (defensive; not normally dumped)
    CRASH  no actual produced -- LOADFAIL / GETDATAFAIL / CREATEFAIL
    FAIL   produced output that diverged

A FAIL with no ``reorder:`` signature is a **candidate**: reorders are expected
``mx::api`` behavior to be absorbed in test normalization later (#214), so the
first-pass worklist targets files that need only feature additions. The worklist
ranks each signature by how many candidate files it is the *sole* blocker of
(fixing it flips those files green now), then by total candidate files it blocks.
"""

from __future__ import annotations

import difflib
import json
import sys
import xml.etree.ElementTree as ET
from collections import Counter
from dataclasses import dataclass
from datetime import datetime, timezone
from pathlib import Path

# Filename suffixes written by the dump harness (#210).
_EXPECTED_SUFFIX = ".expected.xml"
_ACTUAL_SUFFIX = ".actual.xml"
_STATUS_SUFFIX = ".status"

# Crash status codes (no actual document was produced).
_CRASH_STATUSES = frozenset({"LOADFAIL", "GETDATAFAIL", "CREATEFAIL"})

# Signature type -> human label, in worklist display order.
_SIGNATURE_TYPES = ("drop", "add", "value", "attr", "reorder")

# How many distance buckets of near-miss files to surface.
_NEAR_MISS_MAX_DISTANCE = 3


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
# XML helpers
# --------------------------------------------------------------------------- #


def _local(tag: str) -> str:
    """Strip a Clark-notation namespace ({uri}local -> local)."""
    if tag.startswith("{"):
        return tag.rsplit("}", 1)[1]
    return tag


def tag_multiset(root: ET.Element) -> Counter:
    """Count every element's local tag name in the tree (the drop/add primitive)."""
    return Counter(_local(el.tag) for el in root.iter())


def _values_equiv(left: str, right: str) -> bool:
    """Mirror corert ``isEquivalent``: numeric values compare by value."""
    if left == right:
        return True
    try:
        return abs(float(left) - float(right)) < 1e-8
    except (ValueError, OverflowError):
        return False


def _first_paths(root: ET.Element) -> dict[str, str]:
    """Map each local tag to one example root-to-node path (first occurrence)."""
    paths: dict[str, str] = {}

    def walk(el: ET.Element, path: str) -> None:
        for child in el:
            if not isinstance(child.tag, str):
                continue
            tag = _local(child.tag)
            here = f"{path}/{tag}"
            paths.setdefault(tag, here)
            walk(child, here)

    root_tag = _local(root.tag)
    paths.setdefault(root_tag, f"/{root_tag}")
    walk(root, f"/{root_tag}")
    return paths


# --------------------------------------------------------------------------- #
# Divergence collection
# --------------------------------------------------------------------------- #


def _add_signature(sigs: dict[str, str], sig: str, path: str | None) -> None:
    """Record a signature, keeping the first example path seen for it."""
    if sig not in sigs:
        sigs[sig] = path or ""


def _diff_walk(exp: ET.Element, act: ET.Element, path: str, sigs: dict[str, str]) -> None:
    """Collect value/attr/reorder signatures over the aligned, surviving structure.

    ``exp``/``act`` are a matched pair (same tag). Children are aligned by tag
    sequence so the walk survives drops without desyncing; only tag-equal pairs
    are recursed, so value/attr comparisons are never made across mismatched tags.
    Drops and adds are handled separately by the global multiset, not here.
    """
    tag = _local(exp.tag)

    et = (exp.text or "").strip()
    at = (act.text or "").strip()
    if not _values_equiv(et, at):
        _add_signature(sigs, f"value:{tag}", path)

    ea = {_local(k): v for k, v in exp.attrib.items()}
    aa = {_local(k): v for k, v in act.attrib.items()}
    for name in sorted(set(ea) & set(aa)):
        if not _values_equiv(ea[name], aa[name]):
            _add_signature(sigs, f"value:{tag}@{name}", path)
    for name in sorted(set(ea) ^ set(aa)):
        _add_signature(sigs, f"attr:{tag}@{name}", path)

    ec = [c for c in exp if isinstance(c.tag, str)]
    ac = [c for c in act if isinstance(c.tag, str)]
    exp_tags = [_local(c.tag) for c in ec]
    act_tags = [_local(c.tag) for c in ac]

    if exp_tags != act_tags and Counter(exp_tags) == Counter(act_tags):
        _add_signature(sigs, f"reorder:{tag}", path)

    matcher = difflib.SequenceMatcher(a=exp_tags, b=act_tags, autojunk=False)
    for op, i1, i2, j1, j2 in matcher.get_opcodes():
        if op == "equal":
            for k in range(i2 - i1):
                child = ec[i1 + k]
                _diff_walk(child, ac[j1 + k], f"{path}/{_local(child.tag)}", sigs)


def collect_signatures(exp_root: ET.Element, act_root: ET.Element) -> dict[str, str]:
    """Return every divergence signature (-> example path) for one expected/actual pair."""
    sigs: dict[str, str] = {}

    # Drops/adds: complete, reorder-invariant inventory via the tag multiset.
    missing = tag_multiset(exp_root) - tag_multiset(act_root)
    added = tag_multiset(act_root) - tag_multiset(exp_root)
    if missing:
        exp_paths = _first_paths(exp_root)
        for tag in missing:
            _add_signature(sigs, f"drop:{tag}", exp_paths.get(tag))
    if added:
        act_paths = _first_paths(act_root)
        for tag in added:
            _add_signature(sigs, f"add:{tag}", act_paths.get(tag))

    # value/attr/reorder over the aligned surviving structure.
    _diff_walk(exp_root, act_root, f"/{_local(exp_root.tag)}", sigs)
    return sigs


def _signature_type(sig: str) -> str:
    """The type prefix of a signature (``drop:foo`` -> ``drop``)."""
    return sig.split(":", 1)[0]


# --------------------------------------------------------------------------- #
# Per-file classification
# --------------------------------------------------------------------------- #


def _blank_record(rel: str) -> dict:
    """A per-file record with every field present (filled in by the caller)."""
    return {
        "file": rel,
        "status": None,
        "crash_kind": None,
        "signatures": [],
        "sample_paths": {},
        "signature_counts": {},
        "distance": 0,
        "has_reorder": False,
        "is_candidate": False,
    }


def classify_entry(entry: DumpEntry, warn) -> dict:
    """Classify one dump entry into a per-file JSON record."""
    rec = _blank_record(entry.rel)

    # Crash: only an expected file (and maybe a .status sidecar).
    if entry.actual is None:
        kind = entry.status_code if entry.status_code in _CRASH_STATUSES else None
        rec["status"] = "CRASH"
        rec["crash_kind"] = kind
        return rec

    if entry.expected is None:
        warn(f"{entry.rel}: actual present but no expected; skipping")
        rec["status"] = "FAIL"
        return rec

    try:
        exp_root = ET.parse(entry.expected).getroot()
        act_root = ET.parse(entry.actual).getroot()
    except ET.ParseError as exc:
        warn(f"{entry.rel}: XML parse error ({exc}); marking FAIL with no signatures")
        rec["status"] = "FAIL"
        return rec

    sigs = collect_signatures(exp_root, act_root)
    if not sigs:
        rec["status"] = "PASS"
        return rec

    signatures = sorted(sigs)
    rec["status"] = "FAIL"
    rec["signatures"] = signatures
    rec["sample_paths"] = {s: sigs[s] for s in signatures if sigs[s]}
    rec["signature_counts"] = dict(
        sorted(Counter(_signature_type(s) for s in signatures).items())
    )
    rec["distance"] = len(signatures)
    rec["has_reorder"] = any(_signature_type(s) == "reorder" for s in signatures)
    rec["is_candidate"] = not rec["has_reorder"]
    return rec


# --------------------------------------------------------------------------- #
# Aggregation, ranking, output
# --------------------------------------------------------------------------- #


def build_worklist(candidates: list[dict]) -> list[dict]:
    """Rank signatures by candidate files unblocked, sole-blocker first.

    ``files_blocked`` is the number of candidate files carrying the signature;
    ``sole_blocker`` is the number whose *only* divergence is that signature
    (distance 1) -- fixing it flips those files green immediately.
    """
    files: Counter = Counter()
    sole: Counter = Counter()
    for rec in candidates:
        for sig in rec["signatures"]:
            files[sig] += 1
        if rec["distance"] == 1:
            sole[rec["signatures"][0]] += 1
    rows = [
        {
            "signature": sig,
            "type": _signature_type(sig),
            "files_blocked": files[sig],
            "sole_blocker": sole[sig],
        }
        for sig in files
    ]
    rows.sort(key=lambda r: (-r["sole_blocker"], -r["files_blocked"], r["signature"]))
    return rows


def build_batch_plan(candidates: list[dict], max_steps: int = 15) -> list[dict]:
    """Greedy set-cover: the fix sequence that lands the most files the soonest.

    The signature-level worklist ranks fixes independently, but most candidate
    files need *several* fixes before they pass. This answers the #212 question
    directly -- "minimal changes -> most files" -- by greedily choosing, at each
    step, the signature that maximizes the number of candidate files whose
    *entire* remaining signature set is then cleared. Each row's
    ``cumulative_files`` is how many candidate files fully pass once every fix up
    to and including that row is made.
    """
    sigsets = [set(r["signatures"]) for r in candidates]
    all_sigs = {s for ss in sigsets for s in ss}
    fixed: set[str] = set()
    plan: list[dict] = []
    prev = 0
    for _ in range(max_steps):
        best_sig, best_total = None, prev
        for sig in sorted(all_sigs - fixed):  # sorted: deterministic tie-break
            trial = fixed | {sig}
            total = sum(1 for ss in sigsets if ss <= trial)
            if total > best_total:
                best_total, best_sig = total, sig
        if best_sig is None:  # no remaining fix lands another file
            break
        fixed.add(best_sig)
        plan.append({
            "fix": best_sig,
            "added_files": best_total - prev,
            "cumulative_files": best_total,
        })
        prev = best_total
    return plan


def _near_misses(candidates: list[dict]) -> dict[str, list[dict]]:
    """Group candidate files by distance (1..N) so small fix-sets are visible."""
    out: dict[str, list[dict]] = {}
    for dist in range(1, _NEAR_MISS_MAX_DISTANCE + 1):
        bucket = [
            {"file": r["file"], "signatures": r["signatures"]}
            for r in candidates
            if r["distance"] == dist
        ]
        bucket.sort(key=lambda d: d["file"])
        out[str(dist)] = bucket
    return out


def build_report(dump_dir: Path, warn) -> dict:
    """Run classification over the dump directory and return the JSON report."""
    entries = parse_dump_dir(dump_dir)
    records = [classify_entry(e, warn) for e in entries]

    candidates = [r for r in records if r["status"] == "FAIL" and r["is_candidate"]]
    by_status: Counter = Counter(r["status"] for r in records)
    distance_hist = Counter(r["distance"] for r in candidates)

    return {
        "dump_dir": str(dump_dir.resolve()),
        "generated": datetime.now(timezone.utc).isoformat(),
        "summary": {
            "total": len(records),
            "by_status": dict(sorted(by_status.items())),
            "candidates": len(candidates),
            "reorder_blocked": sum(
                1 for r in records if r["status"] == "FAIL" and r["has_reorder"]
            ),
            "distance_histogram": {str(k): distance_hist[k] for k in sorted(distance_hist)},
        },
        "worklist": build_worklist(candidates),
        "batch_plan": build_batch_plan(candidates),
        "near_misses": _near_misses(candidates),
        "files": records,
    }


def print_summary(report: dict, out_path: Path) -> None:
    """Print the human-readable worklist to stdout."""
    summary = report["summary"]
    print(f"Classified {summary['total']} files from {report['dump_dir']}\n")

    for status in ("PASS", "FAIL", "CRASH"):
        n = summary["by_status"].get(status, 0)
        print(f"  {status:<8}{n:>5}")
    print(
        f"\n  candidates (FAIL, no reorder): {summary['candidates']}"
        f"   |   reorder-blocked: {summary['reorder_blocked']}"
    )

    hist = summary["distance_histogram"]
    if hist:
        print("\nDistance to passing (candidate files; #unique fixes needed):")
        for dist, n in hist.items():
            print(f"  {dist:>3} fix(es){n:>6} files")

    worklist = report["worklist"]
    if worklist:
        print("\nWorklist (signatures ranked by candidate files unblocked):")
        print(f"  {'signature':<28}{'sole':>6}{'total':>7}")
        for row in worklist[:20]:
            print(
                f"  {row['signature']:<28}{row['sole_blocker']:>6}{row['files_blocked']:>7}"
            )

    plan = report["batch_plan"]
    if plan:
        print("\nBatch plan (greedy: fewest fixes -> most candidate files passing):")
        for i, row in enumerate(plan, 1):
            print(
                f"  {i:>2}. +{row['fix']:<28}"
                f"{row['cumulative_files']:>5} pass  (+{row['added_files']})"
            )

    print(f"\nOutput: {out_path}")


def run(dump_dir: Path, data_root: Path, out_path: Path) -> int:
    """Entry point for the ``classify`` subcommand.

    ``data_root`` is accepted for CLI compatibility but unused: classification is
    derived entirely from the dump pair, never from an external support index.
    """
    del data_root
    if not dump_dir.is_dir():
        print(f"classify: dump dir not found: {dump_dir}", file=sys.stderr)
        return 2

    def warn(msg: str) -> None:
        print(f"classify: warning: {msg}", file=sys.stderr)

    report = build_report(dump_dir, warn)

    out_path.parent.mkdir(parents=True, exist_ok=True)
    out_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")

    print_summary(report, out_path)
    return 0
