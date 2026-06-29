---
name: mx-api-classify-roundtrip-failures
description: >
  Use this skill to explain, in plain language, what is wrong with the `mx::api` round-trip and what
  it needs next. It drives the failure classifier (dump -> classify) over the corpus, then reads
  build/api/classified.json and turns it into a prioritized, human-readable worklist grouped by
  failure shape (crashes, instant wins, small fix-sets, reorder-blocked, high-frequency drops).
  Invoke for requests like "what's broken about mx::api", "explain the round-trip failures",
  "what does the api need next", or "triage the api round-trip".
argument-hint: "<optional: a signature or element to focus on>"
disable-model-invocation: false
user-invocable: true
---
# Explain the `mx::api` round-trip

`mx::api` is a deliberate subset of MusicXML, and the comparison is strict full-DOM, so most corpus
files diverge somewhere. This skill turns the measured divergences into a plain-English answer to two
questions: what is broken, and what should we fix next to land the most files into the round-trip
corpus with the fewest software changes.

It is the read-out layer on top of the failure classifier (issue #211/#212): the classifier produces
a machine-readable `build/api/classified.json`; this skill interprets it for a human.

## What the classifier reports (and what it does not)

Classification is purely **measured** from each expected/actual pair. It does **not** consult
`data/api.features.xml` or any record of what `mx::api` was believed to "support" — whether a drop is
intended is a present-day human decision (#214), not something the tool asserts. So do not describe
any drop as "by-design" on the classifier's authority; report what was dropped and let the human
decide.

Each difference is a **signature**, and a file's **distance** to passing is its count of unique
signatures:

| signature | meaning |
|-----------|---------|
| `drop:<tag>` | a tag in expected, missing from actual |
| `add:<tag>` | a spurious tag only in actual (a bug) |
| `value:<tag>` | a paired element whose text value differs |
| `value:<tag>@<attr>` | a paired element whose attribute value differs |
| `attr:<tag>@<attr>` | an attribute present on only one side |
| `reorder:<parent>` | a parent with matching children in a different order |

Per-file `status` is `PASS` / `FAIL` / `CRASH`. A `FAIL` with no `reorder:` signature is a
**candidate**: the first-pass worklist targets these, since reorders are expected `mx::api` behavior
to be absorbed in test normalization later (#214).

## Procedure

### Step 1 — produce the data

If `build/api/roundtrip-dump/` is empty or stale (api/impl changed since it was written), run:

```
make dump-api-roundtrip
```

Then always run:

```
make classify-api-roundtrip
```

Read the stdout summary it prints — status counts, the distance histogram, and the ranked worklist.
That worklist *is* the headline answer to "what should we fix next."

### Step 2 — mine `build/api/classified.json`

These read-only analyses expand on the stdout summary. Adjust the path if `--out` was overridden.

The worklist — signatures ranked by candidate files unblocked (`sole_blocker` = files this fix flips
green on its own; `files_blocked` = candidate files that include it):

```
python3 - <<'PY'
import json
d = json.load(open("build/api/classified.json"))
for row in d["worklist"][:25]:
    print(f"{row['sole_blocker']:>4} sole  {row['files_blocked']:>5} total  {row['signature']}")
PY
```

Instant wins — candidate files one fix away from passing (distance 1):

```
python3 - <<'PY'
import json
d = json.load(open("build/api/classified.json"))
for f in d["near_misses"]["1"]:
    print(f["signatures"][0], f["file"])
PY
```

Small fix-sets — files that pass once a handful of features land (distance 2–3); the union of their
signatures is a high-yield batch:

```
python3 - <<'PY'
import json
from collections import Counter
d = json.load(open("build/api/classified.json"))
for dist in ("2", "3"):
    sigs = Counter(s for f in d["near_misses"][dist] for s in f["signatures"])
    print(f"distance {dist}: {len(d['near_misses'][dist])} files; signatures {sigs.most_common(10)}")
PY
```

Crash cluster (highest severity — no output at all) — group by kind to find the common root:

```
python3 - <<'PY'
import json
from collections import Counter
d = json.load(open("build/api/classified.json"))
crashes = [(r["crash_kind"], r["file"]) for r in d["files"] if r["status"] == "CRASH"]
print(Counter(k for k, _ in crashes))
for kind, f in crashes[:20]:
    print(kind, f)
PY
```

Reorder-blocked files (deferred to #214) — how many, and at which parents:

```
python3 - <<'PY'
import json
from collections import Counter
d = json.load(open("build/api/classified.json"))
reorders = Counter(s for r in d["files"] if r["has_reorder"]
                   for s in r["signatures"] if s.startswith("reorder:"))
print("reorder-blocked files:", sum(1 for r in d["files"] if r["has_reorder"]))
print(reorders.most_common(10))
PY
```

To drill into one file, look at its record (`signatures`, `sample_paths`, `distance`) and diff the
pair directly:
`diff build/api/roundtrip-dump/<flat>.expected.xml build/api/roundtrip-dump/<flat>.actual.xml`
where `<flat>` is the corpus path with `/` replaced by `__`.

### Step 3 — write the explanation

Synthesize the findings into plain language, ordered by what grows the corpus fastest. Use this
structure (fill the numbers and signatures from Step 2; do not invent them):

1. Frame it: strict full-DOM, so a file passes only when *every* signature is resolved. The goal is
   to land files with the fewest software changes.
2. Crashes (`status="CRASH"`). Highest severity: no output at all. Name the cluster (the crash kind
   usually points at one feature). These are bugs.
3. Instant wins: the distance-1 candidates and the top `sole_blocker` signatures — one fix each flips
   a file green now. This is the front of the worklist.
4. Small fix-sets: the distance-2/3 candidates and the union of signatures that would unblock a batch
   — "add these N features → these M files pass."
5. Reorder-blocked: count them, name the top `reorder:` parents, and note they are deferred to test
   normalization (#214), not part of the first-pass worklist.
6. High-frequency drops that are *not* sole blockers: large `files_blocked` with low `sole_blocker`
   means the fix helps many files but flips none alone — flag as enabling, lower immediate yield.

Be honest about caveats: the comparison is strict full-DOM, and if the pinned baseline
(`roundtrip-baseline.txt`) is ungrown, almost the whole corpus shows as failing — these are the raw
landscape, not a regression.

## Hand-off Fixes (if requested)

- To fix a dropped/under-supported element or a crash: use the `add-mx-api-feature` skill.
- The findings belong under the tracking issue #208; file specifics with the `open-mx-issue` skill.
