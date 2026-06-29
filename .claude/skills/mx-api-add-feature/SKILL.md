---
name: mx-api-add-feature
description: >
  Use this skill when the user wants to add a MusicXML feature to the `mx::api` layer. This skill
  provides a step-by-step guide to add something to mx::api and wire it up through mx::impl. It may
  also help answer the question "how do I use {{xyz}} feature of MusicXML". If the feature is not
  yet included in the api layer then it may be necessary to open a PR adding support. This skill is
  also useful when implementing PRs with names like "Add support for segno" or "Repeats are
  inaccessible".
argument-hint: "<prompt>"
disable-model-invocation: false
user-invocable: true
---
# Add `mx::api` Feature

The `mx::api` namespace is the public interface of the `mx` library: a deliberate *subset* of
MusicXML expressed in a simple object model that is easier to work with than the MusicXML DOM.
Because it is a subset, a frequent user request is "add X".

## Architecture: three layers

A feature flows through three layers. Know which one you are touching:

- `mx::core` (`src/private/mx/core/generated/`) -- the strongly-typed MusicXML XSD model,
  *generated* from the schema by `gen/`. The element/attribute you are adding almost always ALREADY
  EXISTS here, fully parseable. Do NOT regenerate core or edit `gen/` for a normal feature.
- `mx::impl` (`src/private/mx/impl/`) -- the translation layer. `*Reader` classes turn core into
  api; `*Writer` classes turn api into core. See Step 3.
- `mx::api` (`src/include/mx/api/`) -- the public data model (`ScoreData`, `PartData`,
  `MeasureData`, `NoteData`, `DirectionData`, ...). This is the surface you expose the feature on.
  See Step 2.

So a typical feature is two-sided work in `mx::api` + `mx::impl` only; `mx::core` already has it.

Read `AGENTS.md` (repo root) for the layout and the "Key files to understand" table before you
start.

## Understand the Feature

Do not blindly add every element or attribute. Keep the api lean and easy to use -- obscure things
(for example, per-note color) just bloat the surface. In a targeted way, add what the user actually
needs and nothing extra. Before writing any code:

1. Identify the exact MusicXML element and/or attributes in play. Confirm they are NOT already
   exposed in `src/include/mx/api/` (grep the `*Data.h` headers).
2. Confirm `mx::core` already models them (grep `src/private/mx/core/generated/`). It almost always
   does. If it genuinely does not, stop and tell the user -- that is a `gen/`/core change, out of
   scope for this skill.
3. Decide the minimal api surface and which existing aggregate the feature belongs in (for example,
   a direction-like marking joins the measure's directions; a note-attached marking joins
   `NoteData`). Step 2 covers these patterns.

## Step 1: Test Strategy

First, determine how it will be tested. See ./steps/step-1-test-strategy.md

## Step 2: `mx::api` Data Model

Add (or update) the enums and plain-old-data types in `mx::api`, matching existing patterns. See
./steps/step-2-mx-api-data-model.md

## Step 3: `mx::impl` Algorithms

Wire the new data types to the DOM through the `mx::impl` reader/writer layer, keeping the read and
write paths symmetric. See ./steps/step-3-mx-impl.md

## Step 4: Run tests

Run `make test` for the unit suites and `make test-api-roundtrip` for the corpus roundtrip gate
(Step 1 explains both, along with `make discover-api-roundtrip`). Verify everything passes. Then run
`make fmt` and `make check` so the CI quality gates will pass.

## (optional) Step 5: Open a PR

If the user asks you to open a PR, see ./resources/pr-template.md
