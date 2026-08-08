> SUPERSEDED: This document was part of an early thinking process and is NOT part of the design.
> Its conclusions (e.g. a separate internal API identity) are rejected. The design is in
> `spanner-design.md`. AI agents must ignore this document.

> This was a prompt, written by me. Below is the surprisingly readable response from Luna.

We are working on universal spanners.

A question arises which presents a big issue I need to think about.

> 1. **What is an id?** `NoteData` has no id field today. Say where the id lives, what strings are
>    allowed, and how widely it must be unique (score or part). Say what happens when two notes
>    share an id.

Actually there are two dimensions to decide on here.

Dimension 1: Use of score ID semantics or not. In otherwords, should `mx::api` IDs be pattern
constrained and document-unique-constrained like XSD `xs:ID` prescribes? Dimension 2: Constrain
violation error mechanism. This is an unprecidented new case for both `mx::api` and `mx::core` (see
#397).

The `mx::api` doctorine states:

> ## Failure model: misuse must be boring

> Responses to wrong api usage, in order of preference:
> 
> 1. Unrepresentable: shape the type so the wrong state cannot be expressed (choice types below;
>    merged fields, principle 3).
> 2. Defined fallback: document a harmless result and return it. A wrong-kind choice accessor
>    returns a default-constructed copy; the writer drops the half of an encoding that is
>    meaningless for the note it is on (a tie on a silent cue note is written as `<tied>` notation
>    only, never as a sound-level `<tie>`). No signal to the caller.
> 3. `Result<T>` (`Result.h`): the error channel of last resort. It exists for the `DocumentManager`
>    I/O boundary, where failure is real (unreadable file, unparseable XML). Do not spread it into
>    the data model.

However with IDs we are entering into a territory where this may no longer be a viable approach. We
*could* silently accept the construction of a document with duplicate IDs and fail at some pre-write
validation step. But that is a terrible violation of the fail-fast principle.

The library design got around the fail-fast problem up to this point because it usually had some
not-too-surprising fallback (clamping a bad value, for example) that could allow a valid document
valid document to be written. But with duplicate IDs, there is no benign fallback.

Help me thing through this problem. Please study the code and reflect on the decision I have before
me.

---

> Luna's analysis: consider this overview of all the issues that IDs and u_spans introduce.
> Normally it feels like bad form to copy so much LLM writing verbatim but this is really well done.
> I wouldn't say that

# IDs and the Fail-Fast Problem: Recommendation

You have identified a real boundary in the current design. IDs are not just another field with a
bad-value fallback. They introduce a **document-wide invariant** and, for universal spanners, a
graph of references between objects.

My recommendation is to separate three things that are currently being conflated:

1. **The identity used by `mx::api` to refer to a note.**
2. **The optional `id` attribute emitted into MusicXML.**
3. **The validation of the complete score graph.**

They should not necessarily be the same thing.

## What the existing code tells us

The repository already has an important precedent in the generated core model:

- `core::Token` represents the lexical domain of `xs:ID` and `xs:IDREF`.
- It ensures that an individual value is a valid NCName.
- It does **not** enforce document-wide uniqueness.
- The core design explicitly recognizes that uniqueness is a cross-object property requiring a
  document-level pass.

That distinction is visible in `src/private/mx/core/Token.h` and `docs/ai/design/mx-core-plan.md`.

The current API also already has several raw `std::string` IDs:

- `PartData::uniqueId`
- `InstrumentData::uniqueId`
- optional IDs on various direction and notation types
- no ID on `NoteData`

Those fields are mostly passed into `core::Token` by writers such as `PartWriter.cpp` and
`DirectionWriter.cpp`. Consequently, the API currently has no single, coherent identity policy. Some
IDs are serialized; some are only implementation identities; and none are centrally checked for
uniqueness.

`DocumentManager` is also already the API's natural error boundary. `createFromScore` can fail when
`ScoreData` cannot be represented by core. That makes it a better place for document-level
validation than an individual setter or aggregate constructor.

## Dimension 1: Should API IDs have XSD `xs:ID` semantics?

There are two defensible choices.

### Option A: API IDs are XSD-like IDs

An API ID would:

- be nonempty;
- have NCName syntax;
- be unique throughout the score;
- be suitable for emitting as MusicXML `id`;
- be usable as an IDREF.

This is attractive because it makes the API's identity model line up with MusicXML's model. It also
gives users a familiar rule: IDs look like `note-17`, `P1`, or `n42`, not arbitrary application
strings.

However, it has a serious cost: it makes the API's internal addressing model dependent on a
wire-format constraint that is not necessary for universal spanners. The design explicitly says that
authoring a universal spanner must not require writing note IDs into MusicXML. Therefore the API
needs an identity mechanism even when no XML `id` attribute is present.

Using one field for both purposes would create awkward semantics:

```cpp
note.id = "my application object";
```

Would that mean:

- the note must be serialized with `id="my application object"`?
- the value must be NCName-valid?
- it must be globally unique even if the application only uses it locally?
- changing it changes the serialized document?
- an imported XML ID must be preserved even if it conflicts with an authoring ID?

Those are different concerns.

### Option B: API IDs are opaque application identities

An API note identity would be used only for references inside `mx::api`. It would not be serialized
unless the caller separately requested that.

This is the cleaner choice for universal spanners. The identity needs only the properties required
by the API:

- stable equality;
- nonempty value;
- uniqueness within the score;
- usable in forward references;
- no dependence on XML syntax.

It could be represented as a dedicated type rather than a raw string:

```cpp
class NoteId;
```

or perhaps a more general:

```cpp
class ObjectId;
```

The wrapper would prevent accidental mixing with part IDs, MusicXML numbers, or arbitrary names.
Whether its textual constructor accepts arbitrary strings or a restricted grammar is a separate API
choice. The important point is that **NCName syntax should not be imposed merely because the
identity may be used internally**.

I favor this option.

## Recommended distinction

I would define two separate concepts:

### `api::NoteId`

An internal, score-scoped reference identity.

- Lives on `NoteData`.
- Is not automatically written to MusicXML.
- Is unique across the entire `ScoreData`, not merely within a part.
- May be used before the target note exists.
- Must not be confused with `xml:id` or MusicXML `note@id`.

### Optional wire ID

If a future API exposes the MusicXML `note@id` attribute, it should be a separate field, probably
using an NCName-like wrapper or a constrained string type.

For example, conceptually:

```cpp
struct NoteData
{
    std::optional<NoteId> id;       // API identity, not serialized
    std::optional<XmlId> xmlId;     // MusicXML note@id, serialized when present
};
```

It may be that exposing `xmlId` is not desirable yet. The key recommendation is not to make the
universal-spanner feature depend on it.

## Why score-wide uniqueness is preferable

The identity should be unique across the score, not merely within a part.

Part-local uniqueness is technically possible, but it creates unnecessary context dependence:

- a reference must carry both `partId` and `noteId`;
- moving a note between parts can change the meaning of an existing reference;
- non-note spanners later need an additional scope rule;
- generic APIs become more complicated;
- two notes with the same ID are no longer distinguishable without knowing their containing part.

A score-wide namespace is simpler:

```text
n1 -> exactly one note in the score
```

It also matches the natural interpretation of a graph reference and is stricter than necessary only
in a useful way.

The score-wide rule should apply to API identities, regardless of whether those identities are
emitted into XML.

## What should happen with duplicate IDs?

There is no benign fallback.

The following choices are all problematic:

- silently keep the first note;
- silently keep the last note;
- rewrite one ID;
- drop the universal spanner;
- choose based on traversal order.

Each changes the meaning of user-authored data. In particular, rewriting an ID is dangerous because
the caller may retain references to the original value.

Therefore duplicate IDs should be a **hard document validation error**.

The important question is not whether duplicates are errors. They are. The question is where that
error is reported.

## Why setter-level fail-fast is not realistic

The current API consists primarily of freely mutable value aggregates:

```cpp
ScoreData score;
score.parts[0].measures[3]...notes.push_back(note);
```

Because notes can be inserted, copied, moved, and mutated independently, no individual `NoteData`
setter can know whether its ID is unique. Even a `ScoreData` constructor cannot maintain the
invariant while callers directly mutate nested vectors.

An eagerly enforcing design would require one of:

1. replacing public vectors with controlled containers;
2. making `ScoreData` immutable after construction;
3. introducing a score builder/editor;
4. making IDs allocated by a score-owned registry rather than assigned as values;
5. using object handles or pointers instead of value references.

Those are substantial changes to the API's architecture. They may eventually be worthwhile, but
universal spanners should not quietly assume that the current aggregate model can provide eager
invariant checking.

The current model already accepts that some invariants are checked at a document boundary.
`createFromScore` is the appropriate boundary.

## Recommended failure mechanism

Use `Result` from the operation that assembles or serializes the document:

```cpp
Result<int> DocumentManager::createFromScore(const ScoreData &score);
Result<void> DocumentManager::writeToStream(...);
```

Validation should happen before any lossy conversion into `mx::core`.

For an author-created score:

```cpp
auto result = manager.createFromScore(score);
if (!result.ok()) {
    // duplicateNoteId, danglingNoteReference, ...
}
```

For a score already held by `DocumentManager`, `writeToStream` should not ordinarily discover a new
problem unless the underlying document was modified through an unsafe lower-level interface. But if
`getData` returns a copy, the normal flow is:

1. construct or modify `ScoreData`;
2. call `createFromScore`;
3. receive either a document handle or a validation error;
4. write the validated document.

That is not a violation of the API doctrine's "misuse must be boring" rule. It is precisely the kind
of real, document-level failure for which `Result` exists.

The doctrine says not to spread `Result` into the data model. It does **not** say that
`DocumentManager::createFromScore` must accept every graph-shaped value.

## Add specific error codes

The existing `ResultCode::invalidDocument` is usable initially, but it would be better to add
explicit codes if callers need to distinguish these failures:

```cpp
duplicateId,
danglingIdReference,
invalidId
```

Possibly:

```cpp
duplicateNoteId,
danglingNoteReference
```

The error should include a useful path and message:

```text
code: duplicateId
path: /score-partwise/part[2]/measure[8]/staff[1]/voice[1]/note[4]
message: note id 'n17' is already used by
         /score-partwise/part[1]/measure[3]/staff[1]/voice[1]/note[2]
```

This is much better than a generic "invalid document" error, especially because duplicates are
otherwise difficult to diagnose.

## What about forward references?

R2 requires this:

> A universal spanner may refer to a note that has not yet been added.

That means validation cannot happen when the reference is authored. The reference is temporarily
unresolved, but the *completed score* must be valid when passed to `createFromScore`.

This is no different in principle from constructing a data structure in stages and validating it
when submitted. The important guarantees are:

- a partially assembled `ScoreData` is not presented as a valid document;
- no XML is written from an unresolved graph;
- the error is deterministic;
- no reference is silently redirected.

You can describe the state as:

> `ScoreData` is an editable construction value. `DocumentManager::createFromScore` is the commit
> operation that validates and materializes it as a document.

That gives the model a clear transaction boundary without introducing `Result` into every field
mutation.

## What about imported files?

Imported files already have XML IDs with XSD semantics. Those should not automatically become API
note IDs unless the caller opts into a policy that says they should.

There are at least three possible read policies:

1. **No generated API IDs by default.** Universal-spanner authoring is available only after the
   caller assigns IDs.
2. **Generate API IDs for every note.** IDs are deterministic but are API identities, not XML IDs.
3. **Use existing XML IDs where present and generate the rest.** Convenient, but creates coupling
   between wire identity and API identity.

For the first implementation, I would avoid making read behavior part of the core identity contract.
Define authoring first:

- caller assigns API IDs when using universal spanners;
- imported scores preserve existing XML IDs separately if those are exposed;
- a later opt-in reader mode can generate or map API IDs.

If generated IDs are eventually needed, they should be deterministic and clearly documented as
generated identities, for example `mx-note-1`, `mx-note-2`, rather than being mistaken for source
IDs.

## A subtle issue: copying and equality

If `NoteData::id` participates in `NoteData::operator==`, then assigning an ID changes equality even
though it has no musical meaning. That may be undesirable.

The existing equality macros are used heavily in round-trip tests, so adding an internal identity
field there could cause noisy failures or make two musically identical scores unequal merely because
one has generated IDs.

You should decide whether API identity is:

- semantic data and therefore part of equality;
- authoring metadata and therefore excluded from musical equality;
- included only in a separate structural/document equality.

I would treat an internal `NoteId` as authoring/reference metadata and likely exclude it from
musical equality, unless the API promises identity-preserving equality. This is another argument for
distinguishing it from a serialized XML ID.

## Overall recommendation

I would make the following decision:

1. Add a dedicated API identity for notes, separate from MusicXML `note@id`.
2. Make it nonempty and score-wide unique.
3. Do not impose XSD NCName syntax unless the identity is being serialized as XML.
4. Do not automatically serialize API identities.
5. Permit forward references while the score is being assembled.
6. Validate uniqueness and reference resolution when `createFromScore` commits the score.
7. Return a structured `Result` error; do not throw, repair, rewrite, or drop.
8. Add precise error codes and paths.
9. Treat imported XML IDs and API IDs as separate namespaces unless an explicit mapping policy is
   introduced.
10. Revisit controlled construction only if experience shows that commit-time validation is
    insufficient.

The central principle is:

> Local values can be made valid by construction. Graph invariants can only be validated when the
> graph is assembled.

That is not a failure of the doctrine. It is the point at which the doctrine's third fallback -
`Result` at the real document boundary - becomes necessary. The mistake would be to force duplicate
IDs into the "defined fallback" category when no harmless fallback exists.
