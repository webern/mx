# Step 1: Refresh the corpus views

The corpus views are checked in, but regenerate them so the audit reflects the current corpus and
the current feature-audit format.

```
make audit          # writes any missing *.features.xml sidecars, rebuilds data/corpus.xml
# or, after a format change:
make audit-force    # rewrites every sidecar
```

(`make audit` == `python3 -m audit all`; see `audit/README.md`.)

## What you now have

- `data/corpus.xml` -- the aggregate. Its `<elements>` are ordered by `wild-files` descending, so
  the top of the file is the most common real-world MusicXML. Each `<element>` carries:
  - `wild-files` / `synthetic-files` / `total-files` counts,
  - `min-synthetic-version` (the earliest MusicXML version a synthetic file exercises it in -- a
    hint that a feature is newer),
  - per-`<attribute>` `wild-files`/`synthetic-files` counts,
  - `<wild-files>`/`<synthetic-files>` example file pointers (wild list is capped; the `count`
    attribute is exact).
- `data/**/*.features.xml` -- one sidecar per corpus file, listing exactly the elements and
  attributes that file uses. Use these to find a file that exercises a given feature, or to check
  what a single file needs.

## Read the summary first

Open the `<summary>` block at the top of `data/corpus.xml`:

```
<summary>
  <files total="..." wild="..." synthetic="..."/>
  <elements total="..." used-in-wild="..." synthetic-only="..."/>
</summary>
```

`synthetic-only` is the count of elements that appear in the spec coverage but never in a wild file.
Those are real but rare in practice -- low priority to add unless specifically requested.

## Orientation queries

Useful greps over `data/corpus.xml` before you dive in:

- The most-used wild elements: they are simply the first `<element>` entries in the file.
- Elements with `wild-files="0"` (synthetic-only): spec features unused in the wild.
- A specific element: search for `name="<element>"` to read its attribute usage and example files.

Carry the corpus picture into Step 2 and Step 3: a bug or gap in a high-`wild-files` feature matters
far more than one in a synthetic-only feature.
