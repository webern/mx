---
name: mx-api-add-feature
description: >
  Use when adding a MusicXML feature to mx::api, exposing a feature that is currently inaccessible,
  or explaining whether and how mx::api supports a feature.
argument-hint: "<feature or request>"
disable-model-invocation: false
user-invocable: true
---
# Add an `mx::api` feature

Read the `mx-api-doctrine` skill before designing, implementing, or reviewing the change.

## Investigate

Establish the requested user-facing behavior before choosing an implementation:

- Find the relevant MusicXML elements and attributes in the schema and corpus.
- Check whether `mx::api` already represents the feature under a different shape or name.
- Inspect the generated `mx::core` types, which always implement the requested features.
- Find analogous api types, reader/writer code, and tests to use as patterns.

## Design and implement

Choose an api shape using the doctrine and precedent rather than mirroring MusicXML's shape. Keep
the change focused on the requested behavior, but include whatever representation is needed to make
that behavior sound.

Feature work commonly touches:

- public data types in `src/include/mx/api/`
- core-to-api translation in `src/private/mx/impl/*Reader*` or `*Functions*`
- api-to-core translation in `src/private/mx/impl/*Writer*` or `*Functions*`
- enum conversion in `src/private/mx/impl/Converter.*`
- tests in `src/private/mxtest/api/`

These are landmarks, not a checklist. Follow the closest existing feature through both translation
directions and change only the files the design requires. Do not edit generated core files by hand.

## Verify

- Check API round-trip discovery with `make api-roundtrip-discover` to see if you have unlocked a
  file. If so, add it to the round-trip baseline `src/private/mxtest/api/roundtrip-baseline.txt`. If
  your changes did not unlock a file, still add it and note that it was discovered already passing.

If your reader and writer functions seem complex, add testing. Here are some ideas:
- an in-memory `ScoreData` round trip with `mxtest::roundTrip`. Probe the `ScoreData` values in the
  unit test.
- XML output/input checks with `mxtest::toXml` and `mxtest::fromXml`

Run the applicable gates from `AGENTS.md`; api feature work normally includes `make api-test`,
`make api-roundtrip`, and `make fmt`.

If the user asks to open a pull request, use the `mx-open-pr` skill.
