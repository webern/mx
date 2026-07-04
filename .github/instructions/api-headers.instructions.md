---
applyTo: "src/include/mx/api/**"
---

These are the public `mx::api` headers: plain-old-data structs that re-shape MusicXML into a
simpler model (see `docs/ai/design/api-design-principles.md`). Review for:

- No `mx::core` type may appear in a public header.
- New absent-able fields must be `std::optional<T>`. New absent-able enums must instead have an
  `unspecified` first enumerator (including uses of the ternary `Bool`). Reject new `-1`
  sentinels and new `bool is...Specified` / `has...` flags; do not ask for migration of existing
  ones (tracked in issue #249).
- Every new field needs a default value and a matching `MXAPI_EQUALS_MEMBER(field)` line
  (`MXAPI_DOUBLES_EQUALS_MEMBER` for doubles) in the type's equality block. A missing line
  silently exempts the field from equality and round-trip checks -- flag it as a bug.
- New data must not mirror MusicXML's raw shape when the element is stateful, flat,
  duplicated, or id-linked; check the change against the principles doc.
- A new positioned-in-a-measure type needs `int tickTimePosition`; durations are in ticks.
- Comments should be helpful to humans and coding agents. Crisp, not verbose, explain how the
  code works, especially anything surprising.
