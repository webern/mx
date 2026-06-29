---
name: mx-api-feature-audit
description: >
  Use this skill to audit which MusicXML features the `mx::api` public layer supports, compared to
  the test corpus and the MusicXML specification. It finds (1) enum bugs -- values present in
  `mx::core` (and the spec) but missing from the parallel `mx::api` enum, which are silently dropped
  -- and (2) feature gaps -- elements/attributes used in real-world files that `mx::api` does not
  expose. It emits `data/api.features.xml` and a prioritized recommendation report. Invoke for
  requests like "audit mx::api coverage", "what MusicXML features are we missing", "find enum bugs",
  or "what should we add to the api next".
argument-hint: "<optional: a feature/element to focus on>"
disable-model-invocation: false
user-invocable: true
---
# `mx::api` Feature Audit

`mx::api` is a deliberate *subset* of MusicXML. This skill produces an evidence-based picture of
what that subset covers so we can decide what to add next and which silent-data-loss bugs to fix.

## The three views

The audit compares three views of the MusicXML feature surface. A "feature" is an element plus the
attribute names that appear on it.

1. Specification -- approximated by `data/synthetic/`, which is built to exercise every
   element/attribute/enum symbol of MusicXML 3.0/3.1/4.0. In `data/corpus.xml` these are the
   `synthetic-files`. Treat "appears in a synthetic file" as "is in the spec".
2. Corpus (wild) -- the real-world files under `data/` (everything not in `synthetic/`). In
   `data/corpus.xml` these are the `wild-files`, and their counts tell you how common a feature is
   in practice.
3. `mx::api` -- what the public layer actually exposes, which you audit from source in this skill
   and record in `data/api.features.xml`.

`data/corpus.xml` and the per-file `data/**/*.features.xml` sidecars are produced by the
`python3 -m audit` tool (see `audit/README.md`). They give you views 1 and 2 for free. Your job is
view 3 and the comparison.

## Two kinds of finding

Keep these strictly separate -- they have different severity:

- BUG (silent data loss): a value exists in a `mx::core` enum but the parallel `mx::api` enum lacks
  it, so the `mx::impl` reader maps it to a fallback (`unspecified`/`otherX`) and the original value
  is lost on round-trip. This is almost always introduced when a newer MusicXML version adds an enum
  member to `mx::core` (generated from the XSD) and the hand-written `mx::api` enum is not updated to
  match. See Step 2.
- GAP (unsupported feature): an element or attribute that `mx::api` simply does not model. This is
  by design for obscure features, but a gap in a *heavily-used wild* feature is a candidate to add.
  See Step 3.

A worked BUG example you can confirm today: `core::DynamicsChoice::Kind`
(`src/private/mx/core/generated/DynamicsChoice.h`) contains `n`, `pf`, and `sfzp` (added in MusicXML
3.1), but `api::MarkType` (`src/include/mx/api/MarkData.h`) stops at `fz`, and the `dynamicsMap` at
`src/private/mx/impl/Converter.cpp:170` has no entries for them -- so `findApiItem` returns
`api::MarkType::unspecified` and those dynamics are dropped. That is the exact failure mode Step 2
hunts for.

## Procedure

Run the steps in order. Each writes its output to a known location so the next can consume it.

1. Step 1 -- refresh the corpus views. See `./steps/step-1-refresh-corpus.md`
2. Step 2 -- hunt enum bugs (`mx::core` vs `mx::api`). See `./steps/step-2-audit-enums.md`
3. Step 3 -- audit element/attribute support, write `data/api.features.xml`. See
   `./steps/step-3-audit-elements.md`
4. Step 4 -- write the prioritized recommendation report. See `./steps/step-4-recommend.md`

The `data/api.features.xml` output format (a superset of the per-file `<feature-audit>` shape) is
specified in `./resources/api-features-format.md`.

Read `AGENTS.md` (repo root) for the layer map, and the `add-mx-api-feature` skill for how to
actually implement anything this audit recommends.
