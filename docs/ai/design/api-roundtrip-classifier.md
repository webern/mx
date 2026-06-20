# api round-trip failure classifier — diff design

Design notes for the `audit/classify.py` classifier (issue #211, Phase 2 of #208).
This document explains *how the classifier diffs the expected/actual dump pairs*
and why. It supersedes the original "walk both trees in parallel until the first
mismatch" sketch in the #211 issue body, which has a structural defect described
below.

## Update (#212): measurement-only classification

The classifier was reworked to be grounded **only** in what the round-trip
measurably did to each file. The earlier version cross-referenced
`data/api.features.xml` to label drops as "by-design" (category B) vs. bug
(category G) and to recognize enum/attribute gaps (D/E). That `support` field is
hand-authored opinion — a *prediction* of round-trip behavior, not a measurement
— and it can be wrong (part-group was marked `full` yet dropped data; see #224).
Keying classification on a fallible prediction made the worklist untrustworthy.

What changed:

- **No `api.features.xml`.** The support cross-reference (old Layer 3 and the
  B/D/E/G category logic) is removed. Whether a drop is intended is a present-day
  human decision (#214), not a property the classifier asserts.
- **Categories → signatures.** Every difference is reduced to a signature:
  `drop:<tag>`, `add:<tag>`, `value:<tag>`, `value:<tag>@<attr>`,
  `attr:<tag>@<attr>`, `reorder:<parent>`. A file's **distance** to passing is
  its count of *unique* signatures (a tag dropped a thousand times is one
  `drop:` signature, distance 1).
- **Statuses** are just `PASS` / `FAIL` / `CRASH` (the three pipeline codes fold
  into `CRASH`).
- **Candidates and the worklist.** A FAIL with no `reorder:` signature is a
  *candidate* (reorders are expected `mx::api` behavior, to be absorbed in test
  normalization later, #214). The worklist ranks each signature by the number of
  candidate files it is the *sole* blocker of (fixing it flips them green now),
  then by total candidate files blocked.

Layers 1 (multiset for drops/adds) and 4 (alignment, now also used for
value/attr, not just reorder) below are unchanged and still describe the engine.
Layer 3, "Category G", and the old JSON schema are retained only as historical
rationale; the current schema is at the end of this section.

## The defect in the naive design

Both `compareElements` (`src/private/mxtest/corert/Compare.cpp`) and the original
#211 step 3b do the same thing: a **positional, paired, depth-first walk** of the
two trees that stops at the **first** divergence. That is correct as a pass/fail
gate (corert only needs to know *whether* the documents differ), but it is wrong
as an *analysis* tool.

`mx::api` is a deliberate subset of MusicXML: it drops whole subtrees and reorders
children **by design**. The dominant failure signal is therefore a *deletion* —
an element present in `expected` that never appears in `actual`. A positional walk
cannot survive a deletion:

```
expected children:  [credit] [credit] [part-list] [part] ...
actual   children:           [part-list] [part]    [???]  ...   <- credit dropped
```

The moment `actual` is missing one child, the two child-lists shift out of
alignment, and every subsequent sibling is compared against the wrong partner.
You can only trust the **first** reported divergence; everything after it is noise.
A single drop poisons the rest of the file.

This is a well-documented property of diffing: a naive index-by-index comparison
breaks because one insert/delete shifts every later index, so the differ keeps
matching content at positions that no longer correspond. The principled fixes are
**alignment** (LCS / Myers) or, when position is irrelevant, an **order-free
multiset** comparison. See the research summary at the end of this document.

## What we actually need

The classifier's job is *not* to produce a minimal edit script. It is to answer:

1. **Which element classes (tag names) are present in `expected` but missing from
   `actual`?** — the complete set, not just the first.
2. **How many *distinct* element classes are missing per file?** — the headline
   ranking metric.
3. **Which files are missing exactly one distinct class** (once or many times)? —
   the low-hanging fruit: a single api addition flips them to PASS.
4. A cheap **"distance to passing"** scalar for secondary tie-breaking.

None of these require positions or a minimal edit script. They require a *complete
inventory of deletions*, which is exactly what a multiset difference gives in
linear time, fully invariant to reordering.

Exact order-insensitive structural diffing (unordered tree edit distance) is
NP-complete, and ordered tree edit distance (Zhang-Shasha / RTED / APTED) is
O(n³)/O(n²) — both far more than this problem needs. We deliberately do **not**
use them.

## The layered design

All layers operate on the two normalized dump documents (`*.expected.xml`,
`*.actual.xml`) using only the Python standard library
(`xml.etree.ElementTree`, `collections.Counter`, `difflib`). No `lxml`, `xmldiff`,
`zss`, or `apted` dependency is introduced — the multiset primitive covers the need
in O(n).

### Layer 1 — multiset tag diff (primary signal)

Recurse both trees, building a `Counter` of element tag names:

```python
from collections import Counter

def tag_multiset(root):
    return Counter(el.tag for el in root.iter())

missing = tag_multiset(expected) - tag_multiset(actual)   # in expected, not in actual
added   = tag_multiset(actual)   - tag_multiset(expected)  # spurious (rare; a real bug)
```

`Counter` subtraction keeps only positive counts, so `missing` is precisely the
multiset of element classes the round-trip dropped. From it:

| field | meaning |
|---|---|
| `missing_elements` | `sorted(missing)` — the distinct dropped tag names |
| `missing_element_counts` | `dict(missing)` — how many of each were dropped |
| `distinct_missing_count` | `len(missing)` — **the headline ranking metric** |
| `added_elements` | `sorted(added)` — spurious tags the api invented (bug signal) |

This is O(n), reorder-invariant, and enumerates **every** dropped class — it does
not stop at the first one. It is the direct fix for the cascade defect.

### Layer 2 — path-qualified multiset (attribution)

Bare tag names are ambiguous (`<beam>` under `<note>` vs. elsewhere). For
`blocking_features` attribution and to line up with `first_divergence_path`, build
a second multiset keyed by `parent/tag` (or the full root-to-node path). The
headline `distinct_missing_count` stays on bare tags; path qualification is used
when mapping a drop back to an `api.features.xml` feature.

### Layer 3 — cross-reference `api.features.xml` *(REMOVED in #212; historical)*

This layer was removed: classification no longer consults `api.features.xml`. The
text below is retained only to explain what the categories used to mean. For each
missing tag, the old version looked up `support` in `api.features.xml`:

- **Category B (drop-only)** becomes *provable across the whole file*: it holds iff
  **every** tag in `missing` has `support="none"`. The naive walk could never
  establish this — it only saw the first drop. This is the single biggest
  correctness win of the redesign.
- A missing tag with `support="full"` or `"partial"` is a genuine correctness bug
  or a partial-drop — surfaced explicitly instead of being hidden behind whatever
  the first divergence happened to be.

### Layer 4 — sequence alignment per parent (reorder + paired value/attr)

`difflib.SequenceMatcher` over the two child-tag sequences earns its keep twice:

- **Reorder** (`reorder:<parent>`): a parent whose child *multiset* is equal but
  whose child *sequence* differs is a pure permutation.
- **Paired value/attr** (`value:`/`attr:`): recursing only the matcher's `equal`
  blocks pairs surviving elements across drops without desyncing, so text and
  attribute differences are compared between genuinely corresponding nodes (never
  across mismatched tags). This is how value/attr diffs survive a sibling drop.

It is **not** used for the deletion inventory (Layer 1's multiset already has
that). Stdlib, zero dependency.

### Layer 5 — edit-distance scalar (secondary)

Use `sum(missing.values())` (total dropped instances) as a cheap "distance to
passing." It is a *secondary* tie-breaker only. We do **not** key ranking on it:
a scalar conflates "one big dropped subtree" with "many tiny diffs" and names no
feature. Full tree edit distance would also pull in a heavyweight dependency for
no added decision value here.

## Ranking output (the worklist)

The low-hanging fruit is the set of files with `distinct_missing_count == 1`.
Group those by their single missing tag; the resulting histogram is the
prioritized api-addition worklist.

Two refinements from the regression-triage literature (e.g. BuildSheriff, ICSE'22:
failures cluster around a few root causes; fixing one unblocks a cluster):

1. **Rank by files *unblocked*, not occurrence count.** A tag appearing 10,000×
   in one file is lower yield than a tag that is the *sole* blocker of 200 files.
   The y-axis is "count of files whose single blocking class is tag X."
2. **Histogram on the categorical root cause (missing tag), not the edit-distance
   scalar.** Keep the scalar as a within-cluster severity tie-breaker.

This belongs to Phase 3 (#212) ranking, but the per-file fields it needs
(`distinct_missing_count`, `missing_elements`, single-blocker flag) are emitted
here in Phase 2.

## JSON schema (current, #212)

Each file gets one record; every field is present on every entry:

```jsonc
"file": "wild/foo.xml",
"status": "FAIL",                 // PASS | FAIL | CRASH
"crash_kind": null,              // LOADFAIL | GETDATAFAIL | CREATEFAIL | null
"signatures": ["drop:credit", "value:step"],   // sorted unique signatures
"sample_paths": {"drop:credit": "/score-partwise/part/measure/credit"},
"signature_counts": {"drop": 1, "value": 1},   // by signature type
"distance": 2,                    // len(signatures); the headline metric
"has_reorder": false,            // any reorder: signature
"is_candidate": true             // FAIL and not has_reorder
```

The report's top level adds `summary` (status counts, `candidates`,
`reorder_blocked`, `distance_histogram`), `worklist` (signatures ranked by
`sole_blocker` then `files_blocked`), and `near_misses` (candidate files bucketed
by distance 1..3).

### Pipeline errors and the `.status` sidecar

A flat dump filename (`a__b.xml.expected.xml`) cannot, on its own, distinguish the
three pipeline-error statuses (`LOADFAIL` / `GETDATAFAIL` / `CREATEFAIL`): all
three produce only an `.expected.xml` (the api pipeline never reached a written
document). So the dump step (#210) writes a tiny `<flat>.status` sidecar next to
the expected file recording the exact code. The classifier reads it into
`crash_kind` (with `status = "CRASH"`); when the sidecar is absent, `status` is
still `"CRASH"` and `crash_kind` is `null`. The presence/absence of `.actual.xml`
remains the FAIL-vs-CRASH signal; the `.status` sidecar only refines *which*
crash. The sidecar lives in the gitignored dump dir and is never checked in.

### Category G — `support="full"`/`"partial"` drops *(REMOVED in #212)*

Category G (and the B/D/E categories) existed only because classification used to
read `api.features.xml`. With that cross-reference gone, every dropped tag is just
a `drop:<tag>` signature regardless of any prior belief about whether it was
"supported"; the supported-vs-by-design judgment is a present-day human call
(#214), not the classifier's. `part-group` (marked `support="full"` yet dropped;
#224) was the case that proved the `support` field could not be trusted as a
classification input — which is what motivated removing it. The drop is still
reported, now as `drop:part-group`, ranked purely by how many files it blocks.

## Dependency decision

Stdlib only: `xml.etree.ElementTree` + `collections.Counter` + `difflib`. Rationale:
the need is "enumerate missing distinct tag names + counts + single-blocker files,"
which multiset diff answers exactly in O(n). Structure-aware edit-script libraries
(`xmldiff`, which needs the `lxml` C extension; `apted`) deliver minimal edit
scripts and move detection we don't need; `zss` returns only a scalar distance.

---

## Research appendix (problem-space survey)

Findings backing the design above, with confidence and sources. UNVERIFIED marks a
claim from training knowledge that the fetched sources did not directly confirm
(usually because a primary PDF was unreachable).

### Alignment vs. positional walk
- A naive index-by-index walk cascades after one insert/delete; Myers models diff
  as shortest-path search on an edit graph where diagonal (match) moves are free, so
  shared content anchors the alignment. *(High — jcoglan.com/2017/02/12/the-myers-diff-algorithm-part-1/; xmailserver.org/diff2.pdf)*
- Microsoft XML Diff documents three child-matching modes — By Index (naive), LCS
  (survives ins/del), Unordered (set-like). *(Med-high — documentation.help/Microsoft-XML-Diff)*

### Tree edit distance (decided against)
- Zhang-Shasha O(n⁴)/O(nm); Klein O(n³ log n); Demaine'07 O(n³); RTED O(n³)/O(n²);
  APTED O(n³)/O(n²) with an edit *mapping* (unmapped source nodes = deletions).
  *(High — erikdemaine.org/papers/TreeEdit_ICALP2007/paper.pdf; arxiv 1201.0230;
  dl.acm.org/doi/10.1145/2699485; github.com/DatabaseGroup/apted)*
- **Unordered** tree edit distance is NP-complete / MAX-SNP-hard even for binary
  trees over a 2-symbol alphabet. *(High — Bille survey, sciencedirect
  S0304397505000174; arxiv 2108.00953)*

### XML-specific algorithms
- Chawathe et al. 1996 (basis of Python `xmldiff`): match phase + edit-script phase,
  ordered, emits INSERT/UPDATE/MOVE/DELETE — enumerates deletions and reports a
  reorder as one MOVE. *(High on ops; exact complexity bound UNVERIFIED — primary
  PDF 503'd. sigmodrecord.org/1996/06/24/...; github.com/cfpb/xtdiff)*
- X-Diff: unordered model (reorder is a non-change), bottom-up signature hashing +
  min-cost sibling matching; harder to compute, no MOVE op. *(Ordered/unordered:
  high — pages.cs.wisc.edu/~yuanwang/xdiff.html; "no MOVE": UNVERIFIED)*

### Sequence alignment & multiset
- Myers O(ND) powers git/GNU diff; per-parent LCS over child sequences is an
  established lightweight alternative to TED (XMLMind). *(High — xmailserver.org/diff2.pdf;
  xmlmind.com/.../xmlDiff_algorithm.html)*
- `collections.Counter` is a multiset; `a - b` keeps only positive counts =
  elements in `a` not covered by `b`, directly enumerable; building is O(n),
  set-ops O(n+m). *(High on semantics — docs.python.org/3/library/collections.html;
  the explicit O(n) is standard reasoning, not doc-quoted → UNVERIFIED as a doc claim)*

### Python libraries
- `xmldiff` 3.0 (2026-06-11, MIT, active): Chawathe; enumerable `DeleteNode`/
  `MoveNode`/… actions; **requires lxml (C ext)**.
- `zss` (2018, BSD): Zhang-Shasha; **scalar distance only, no edit script**.
- `apted` (2017, MIT, pure Python): edit mapping (deletions = unmapped source
  nodes); needs bracket-notation input.
- `lxml`: no semantic diff, only C14N canonicalization.
- stdlib `difflib.SequenceMatcher` (opcodes) and `collections.Counter` (multiset):
  zero-dependency, both enumerate deletions. *(All high — verified against live
  PyPI/GitHub/ReadTheDocs)*

### Yield-based ranking
- BuildSheriff (ICSE'22, 163K builds): failures cluster around a few root causes;
  triaging by shared cause saves cost — validates "many files, few blockers."
  *(Med-high — chenbihuan.github.io/paper/icse22-zhang-buildsheriff.pdf)*
- Rank by **files unblocked**, not occurrence count; histogram on categorical root
  cause, with edit distance only as a secondary severity scalar. *(Synthesized from
  the triage literature above + frequency×impact practice)*
