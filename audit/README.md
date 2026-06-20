# audit/ -- MusicXML feature audit

A small Python tool (`python3 -m audit`) that inventories which MusicXML features
the test corpus uses, so we can make informed decisions about what the `mx::api`
public layer should expose.

A *feature* is an element plus the set of attribute names that appear on it.

## What it produces

1. `data/**/<name>.features.xml` -- one sidecar per corpus file, listing every
   element and attribute that file uses. Checked in. The round-trip test suites
   (corert, the Go/C ports, the api corpus walker) skip these.
2. `data/corpus.xml` -- a single aggregate: every element across the corpus,
   ordered by how many real-world ("wild") files use it, with per-attribute
   usage counts and pointers to example files. Distinguishes wild files from
   `data/synthetic/` files (which are built to cover the spec symbol by symbol,
   so they approximate "what is in the specification").

The same `<feature-audit>` document shape is what the `api-feature-audit` skill
emits for `mx::api` (as `data/api.features.xml`), so the three views -- spec
(synthetic), corpus (wild), and api -- are directly comparable.

## Usage

```
make audit                       # files + corpus (skips existing sidecars)
make audit-force                 # rewrite every sidecar (use after a format change)

python3 -m audit files           # write missing *.features.xml sidecars
python3 -m audit files --force   # overwrite existing sidecars
python3 -m audit corpus          # (re)build data/corpus.xml
python3 -m audit all [--force]   # both
```

`files` skips sidecars that already exist unless `--force` is given, so the
common case (a new corpus file was added) only writes the new sidecar. Use
`--force` when the output format itself changes.

## Classifying api round-trip failures

```
make dump-api-roundtrip          # C++: write normalized expected/actual XML pairs
make classify-api-roundtrip      # Python: classify those failures by root cause

python3 -m audit classify <dump_dir> [--data DIR] [--out FILE]
```

`classify` reads the dump directory produced by `make dump-api-roundtrip`
(`build/api/roundtrip-dump/`), diffs each expected/actual pair as an order-free
element **multiset** (`Counter(expected) - Counter(actual)`), cross-references
`data/api.features.xml`, and assigns each non-passing file a root-cause category
(drop-only, reorder-only, enum bug, missing attribute, pipeline error). It writes
`build/api/classified.json` and prints a worklist of the features blocking the
most files. The two steps are kept separate: dumping is slow (runs the C++
pipeline over the whole corpus), classifying is fast (pure Python), so the
classification logic can be iterated without re-dumping. See
`docs/ai/design/api-roundtrip-classifier.md`.

## Tests

```
make test-audit                  # python3 -m unittest discover -s audit/tests -t .
```

## Audited set

The audited files are exactly those the `corert` round-trip suite processes (see
`discover.py`): `*.xml`/`*.musicxml` under `data/`, excluding the
`expected`/`testOutput`/`generalxml`/`smufl` directories, `*.fixup.xml` sidecars,
and files with an `*.invalid` marker -- plus the tool's own outputs.

## Layout

| File | Responsibility |
|------|----------------|
| `discover.py`    | find the corpus files to audit (mirrors corert) |
| `extract.py`     | parse one file -> element/attribute surface |
| `featuresfile.py`| read/write the per-file `<feature-audit>` format |
| `summary.py`     | aggregate into `data/corpus.xml` |
| `__main__.py`    | CLI |
