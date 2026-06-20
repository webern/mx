---
name: explain-api-roundtrip
description: >
  Use this skill to explain, in plain language, what is wrong with the `mx::api` round-trip and what
  it needs next. It drives the failure classifier (dump -> classify) over the corpus, then reads
  build/api/classified.json and turns it into a prioritized, human-readable explanation grouped by
  failure mode (hard crash, dropped supported elements, reorder, by-design drop, audit blind spot).
  Invoke for requests like "what's broken about mx::api", "explain the round-trip failures",
  "what does the api need next", or "triage the api round-trip".
argument-hint: "<optional: a category or element to focus on>"
disable-model-invocation: false
user-invocable: true
---
# Explain the `mx::api` round-trip

`mx::api` is a deliberate subset of MusicXML, so some round-trip data loss is by design. This skill
separates the by-design losses from the real defects and produces a plain-English answer to two
questions: what is broken, and what should we fix next.

It is the read-out layer on top of the failure classifier (issue #211): the classifier produces a
machine-readable `build/api/classified.json`; this skill interprets it for a human.

## How it works

The pipeline has two steps, kept separate on purpose (see `audit/README.md`):

1. `make dump-api-roundtrip` — runs the api pipeline over every corpus file and writes the
   normalized expected/actual XML pairs to `build/api/roundtrip-dump/`. Slow: it builds the C++
   harness and runs ~800 files. Re-run only when api/impl code changed.
2. `make classify-api-roundtrip` — pure Python, fast. Diffs each pair as an element multiset
   (`Counter(expected) - Counter(actual)`), cross-references `data/api.features.xml`, and writes
   `build/api/classified.json` plus a stdout summary.

The categories in the JSON (`primary_category`):

| id | meaning |
|----|---------|
| B | drop-only: every missing element is `support="none"` — a by-design subset drop |
| C | reorder-only: same elements, different order |
| D | enum bug: a value maps to a known-missing enum member |
| E | missing attribute: a `partial` feature dropped one attribute |
| F | pipeline error: LOADFAIL/GETDATAFAIL/CREATEFAIL — no output produced (a crash) |
| unknown | a FAIL that matched none of the above — usually a `support="full"` element that was dropped (a real bug) or an element not tracked in `api.features.xml` |

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

Read the stdout summary it prints — that is the top-level shape (counts per category + the worklist
of features blocking the most files).

### Step 2 — mine `build/api/classified.json`

Run these read-only analyses (they join the classifier output against the support index). Adjust the
path if `--out` was overridden.

Top dropped elements, with their audited support level (the key signal — `support="full"` drops are
bugs, `support="none"` drops are by design):

```
python3 - <<'PY'
import json, re
from collections import Counter
d = json.load(open("build/api/classified.json"))
support = {m.group(1): m.group(2) for m in
          re.finditer(r'name="([^"]+)" support="([a-z]+)"', open("data/api.features.xml").read())}
miss = Counter()
for r in d["files"]:
    for tag in r["missing_element_counts"]:
        miss[tag] += 1  # files affected
for tag, n in miss.most_common(25):
    print(f"{n:>4} files  {tag:<18} support={support.get(tag, 'NOT-IN-INDEX')}")
PY
```

Pipeline-error (crash) cluster — group by file/feature to find the common root:

```
python3 - <<'PY'
import json
d = json.load(open("build/api/classified.json"))
for r in d["files"]:
    if r["primary_category"] == "F":
        print(r["pipeline_error_kind"], r["file"])
PY
```

Reorder cluster — where in the tree the order diverges:

```
python3 - <<'PY'
import json
from collections import Counter
d = json.load(open("build/api/classified.json"))
paths = Counter(r["first_divergence_path"] for r in d["files"] if r["primary_category"] == "C")
for path, n in paths.most_common(10):
    print(f"{n:>4}  {path}")
PY
```

What is driving the `unknown` bucket (the warnings on stderr from Step 1 also list this; this is the
programmatic view):

```
python3 - <<'PY'
import json, re
from collections import Counter
d = json.load(open("build/api/classified.json"))
support = {m.group(1): m.group(2) for m in
          re.finditer(r'name="([^"]+)" support="([a-z]+)"', open("data/api.features.xml").read())}
full_drop, untracked = Counter(), Counter()
for r in d["files"]:
    if r["primary_category"] != "unknown":
        continue
    for tag in r["missing_elements"]:
        s = support.get(tag)
        if s in ("full", "partial"):
            full_drop[tag] += 1   # claimed supported but dropped -> bug
        elif s is None:
            untracked[tag] += 1   # not in api.features.xml -> audit gap
print("supported-but-dropped:", full_drop.most_common(10))
print("untracked:", untracked.most_common(10))
PY
```

To drill into one file, look at its record (`missing_elements`, `mismatch_type`,
`first_divergence_path`) and diff the pair directly:
`diff build/api/roundtrip-dump/<flat>.expected.xml build/api/roundtrip-dump/<flat>.actual.xml`
where `<flat>` is the corpus path with `/` replaced by `__`.

### Step 3 — write the explanation

Synthesize the findings into plain language grouped by failure mode, ordered by severity. Use this
structure (fill the numbers and element names from Step 2; do not invent them):

1. Frame it: `mx::api` is a subset, so some loss is by design — separate that from the real defects.
2. Hard crashes (category F). Highest severity: no output at all. Name the cluster (the crash
   analysis usually points at one feature). These are bugs.
3. Dropped supported elements (the `support="full"`/`partial` rows from the top-dropped table and the
   `supported-but-dropped` view). Either an impl round-trip bug or `api.features.xml` overstates
   support — say which needs checking, per element.
4. Reorder (category C). Lower severity: content intact, order wrong. Name the divergence path.
5. By-design drops (category B): mention briefly — these are expected subset behavior, not bugs.
6. Audit blind spots: the `untracked` view — elements dropped but not in `api.features.xml`, so they
   can't be categorized. Recommend running `api-feature-audit` to close the gap.

Then give a prioritized "what it needs" list. Be honest about caveats: the comparison is strict
full-DOM, and if the pinned baseline (`roundtrip-baseline.txt`) is ungrown, almost the whole corpus
shows as failing — these are the raw landscape, not a regression.

## Hand-off Fixes (if requested)

- To fix a dropped/under-supported element or a crash: use the `add-mx-api-feature` skill.
- To correct or extend support levels in `data/api.features.xml`: use the `api-feature-audit` skill.
- The findings belong under the tracking issue #208; file specifics with the `open-mx-issue` skill.
