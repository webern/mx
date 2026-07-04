---
applyTo: "data/**,src/private/mxtest/**"
---

`data/` is the MusicXML test corpus; `src/private/mxtest/` holds the test suites
(see `data/README.md`). Review for:

- Adding or removing a corpus file requires: the pinned count bump in
  `src/private/mxtest/corert/CoreRoundtripTest.cpp`, regenerated audit artifacts
  (`make audit`: the `*.features.xml` sidecar and `data/corpus.xml`), and a green
  `make test-core-dev`. Flag a corpus change missing any of these.
- A deliberately unparseable file needs a sibling `<name>.invalid` marker explaining why; a
  schema-invalid-but-lenient file needs a `<name>.fixup.xml` sidecar. New fixups must follow
  the uniform leniency policy in `data/README.md`, not invent per-file rules.
- `roundtrip-baseline.txt` (the api round-trip pass list) only grows by deliberate commits;
  flag removals or unexplained churn.
- `ApiLoadSmokeTest` only proves a file imports without crashing; a feature claim needs a
  fixture pinned in the read->write->read gate (`make test-api-roundtrip`).
- Test code follows the same C++ rules as the library (no new anonymous namespaces).
