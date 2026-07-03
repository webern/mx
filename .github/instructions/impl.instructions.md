---
applyTo: "src/private/mx/impl/**"
---

`mx::impl` translates between `mx::core` (generated MusicXML model) and `mx::api` (public
structs). Review for:

- Read/write symmetry: a feature read in a `*Reader` must also be written by the matching
  `*Writer`, and vice versa. A one-sided change silently drops data on round-trip -- flag it.
- Silent data loss: an empty `case`/`break`, a value read but never stored, or a field never
  consulted on write is only acceptable with a comment saying the drop is intentional.
- Sentinel contracts must hold on both sides (e.g. time-modification 1/1 means "absent";
  `-1` means "unspecified"). A writer emitting a sentinel or a reader failing to produce one
  breaks round-trip.
- `MX_DEBUG_THROW` is a no-op in release builds; it must not be the only guard on a path that
  would otherwise write invalid or truncated MusicXML.
- Tick time: positions are absolute (`tickTimePosition` against `ScoreData::ticksPerQuarter`);
  writers must synthesize `<backup>`/`<forward>`/`<divisions>`, never expect callers to.
- No new anonymous namespaces (unity-build rule; use named helpers / `k`-prefixed constants).
