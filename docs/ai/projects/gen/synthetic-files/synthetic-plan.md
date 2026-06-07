# Synthetic MusicXML test-file generation

Status: DONE. See `data/synthetic/README.md` for naming convention and toolchain docs.

## Key decisions

- Granularity ("B-middle-ground"): one file per element for the shared formatting tail;
  one file per token for structural/enumerated tokens.
- Vendored `xml.xsd` and `xlink.xsd` into `docs/`; repointed MusicXML schema imports from
  `http://www.musicxml.org/xsd/...` to local filenames for fully offline `xmllint --nonet`.
- No `<!DOCTYPE>` in synthetic files.
- Generation tool: self-contained `make_synth.py` (throw-away; deleted before PR).

## Results

388 synthetic files generated; every file XSD-valid (xmllint `--nonet`, verified twice).
Together with the committed `data/` corpus they cover 100% of element/attribute symbols across
MusicXML 3.0, 3.1, and 4.0.

| Batch           | Location                                    | Files |
|-----------------|---------------------------------------------|------:|
| baseline-3.0    | `data/synthetic/`                           |   235 |
| incremental-3.1 | `docs/ai/projects/gen/synthetic-files/3.1/` |   101 |
| final-4.0       | `docs/ai/projects/gen/synthetic-files/4.0/` |    52 |

Real uncovered symbols: **0 / 0 / 0** (3.0 / 3.1 / 4.0). The 387/392/431 "uncovered" tokens
in the naive XSD extractor were all extractor false-positives (xmllint-rejected).

Three files were hand-authored: `cancel.location.3.0.xml`, `key-accidental.3.0.xml`,
`key-accidental.smufl.3.1.xml` — these live inside `<key>` choice groups the generator
did not descend into.
