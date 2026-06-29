# Step 3: Audit element/attribute support, write `data/api.features.xml`

Now determine, element by element, what `mx::api` actually exposes, and record it in the format from
`./resources/api-features-format.md`.

## Work corpus-first, most-used-first

Drive the audit from `data/corpus.xml`, top to bottom. It is already ordered by `wild-files`
descending, so you spend effort where it matters. You do not need to cover synthetic-only elements
(`wild-files="0"`) unless the user asked for exhaustive coverage -- note them as `support="none"`
in bulk if so, but do not over-invest.

For each element you audit:

1. Is the element modeled in `mx::api`? The public data model is `src/include/mx/api/*Data.h`
   (`ScoreData`, `PartData`, `MeasureData`, `NoteData`, `DirectionData`, ...). Search there for a
   field or type that carries the element. Then confirm the translation exists in
   `src/private/mx/impl/` (a `*Reader` populates it from core, a `*Writer` emits it). Element with a
   field AND a reader/writer => exposed.
2. Which of its attributes survive? For the element's `<attribute>` entries in `data/corpus.xml`
   (focus on non-zero `wild-files`), check whether `mx::api` carries each one. Common position/font/
   color attributes are often handled by shared helpers (`PositionData`, `FontData`, `ColorData`,
   `getPositionData`/`getFontData` in `mx::impl`) -- a single helper may cover many attributes at
   once, so check those before declaring an attribute unsupported.
3. Any enums on the element? Fold in the Step 2 result as an `<enum>` entry. This is where bugs get
   recorded inline with the feature.

Classify the element `support` as `full` / `partial` / `none` per the format vocabulary.

## Verify, do not guess

A field named like the element is not proof. Confirm both directions:

- read: a `*Reader` in `mx::impl` sets the api field from the core value.
- write: a `*Writer` emits it back. If only one side exists, it is `partial` and worth a note.

When unsure whether an attribute round-trips, find a wild file that uses it (via the element's
`<wild-files>` pointers in `corpus.xml`, or the per-file sidecars) and trace it, or note the
uncertainty rather than asserting support.

## Write the file

Emit `data/api.features.xml` per `./resources/api-features-format.md`: one `<feature>` per audited
element, sorted by name, with `<attributes>`, `<enums>`, and `<notes>` as applicable. This file is
the durable record of view 3 and the direct input to the Step 4 comparison.
