# The MusicXML level element, explained

## What `<level>` is, and why it has that name

`<level>` is a provenance tag: metadata attached to a symbol that labels its editorial status. Like
a `git blame` annotation, it records where a mark came from rather than changing the music.

The name comes from MuseData, a 1980s-90s encoding format for critical editions. When asked about
the confusing name, Good answered (w3c-cg/musicxml#146):

> "This was a generalization of the integer level field in MuseData. In MusicXML it is used to
> identify and describe different types of editorial information."

In MuseData, each record could carry an integer "level". The editor defined what each number meant,
for example 1 = from the autograph, 2 = editor's addition, 3 = from the first print. So "level"
means an editorial layer, like a layer in a graphics program. MusicXML widened the integer to free
text. That's why the content is just `xs:string` with no defined vocabulary.

The XSD wording (`docs/musicxml-4.0-ed15c23.xsd:2645`):

> "The level type is used to specify editorial information for different MusicXML elements. The
> content contains identifying and/or descriptive text about the editorial status of the parent
> element."

The second sentence was added in 4.0 to clarify the meaning (w3c-cg/musicxml#146).

Good's example comes from his paper *Using MusicXML 2.0 for Editorial Applications* (Listing
19). Two slurs in a Brahms edition come from different sources:

```xml
<slur type="start" number="1"/> ... <level>1</level>
<slur type="start" number="2"/> ... <level reference="yes">2 (pencil)</level>
```

Read it as: slur 1 belongs to editorial layer "1". Slur 2 belongs to layer "2 (pencil)", someone's
pencil marking in a source, and it's shown for reference only. That's the original use case:
scholarly editions that record where each mark came from.

Why this design: MusicXML had to round-trip MuseData-style scholarly encodings without dictating
anyone's source classification scheme. An open string tag fits that, since each edition defines its
own layers. The cost is that the content means nothing to a program. Only the attributes carry
machine-readable meaning.

`mx::api` doesn't model `<level>`. In core it's `src/private/mx/core/generated/Level.h:30`. Our
corpus doesn't contain a real-world exported file using it, and the research didn't find evidence
that Finale, Sibelius, MuseScore or Dorico write it.

## The attributes: where `<level>` becomes machine-readable

Intuition: the text content is the human label. The attributes are three independent things a
program can act on: a playback flag, a span marker, and display hints.

```
<level reference="yes|no"          ← playback: "this is reference material, don't play it"
       type="start|stop|single"    ← span: does it cover one symbol or a run of them?
       parentheses="yes|no"        ┐
       bracket="yes|no"            ├ display (level-display group): how editorial-ness is drawn
       size="full|cue|grace-cue|large" ┘
>free text label</level>
```

`reference` (added in MusicXML 1.1). The XSD (`:2647`) says:

> "editorial information that is for display only and should not affect playback. For instance, a
> modern edition of older music may set reference="yes" on the attributes containing the music's
> original clef, key, and time signature."

This is the "incipit" case. A modern Renaissance edition prints a small prefatory staff showing the
original C clef and mensuration sign, then continues in treble clef. Both `<attributes>` elements
exist in the file, but a player must ignore the original one. This explains why `<attributes>` is on
the list of elements that can carry `<level>`: an editorial `<attributes>` is a real use, not
padding.

Display hints (`level-display`, also 1.1). The XSD (`:2044`):

> "three common ways to indicate editorial indications: putting parentheses or square brackets
> around a symbol, or making the symbol a different size."

These are engraving conventions. An editor's added slur is often drawn dashed, an added dynamic in
brackets, an added accidental small or in parentheses. The same attribute group sits on
`<accidental>`, which is why `AccidentalMarkData` in mx already has `parentheses`/`bracket`/`size`.
Editorial accidentals were the first application.

This is rpatters1's use case. In w3c-cg/musicxml PR 736, Good wrote:

> "Not really, since rests don't have noteheads. Parenthesizing a rest is best done with a
> `<level parentheses="yes"/>` child element."

```xml
<note>
  <rest/>
  <duration>4</duration>
  <level parentheses="yes"/>   <!-- no text: purely a display hint -->
  <voice>1</voice>
  <type>quarter</type>
</note>
```

The element has drifted from "provenance tag" to "editorial display hint", often with empty content.
MusicXML doesn't offer another clean way to parenthesize a rest. One LilyPond test file in our
corpus uses `<notehead parentheses="yes">`; Good calls that wrong because a rest doesn't have a
notehead.

`type` (added in 4.0, issue 288). Start/stop/single lets one `<level>` cover a run of symbols
instead of tagging each one. In that same PR 736 thread, rpatters1 himself asked how to parenthesize
a whole chord (a Finale "entry"). A contributor proposed `type="start"` on the first chord note and
`type="stop"` on the last. Good called it "a good clarification of the way this is indeed intended
to work," but it isn't in the spec as an example. Good placed `type` on `<level>` itself, not in
`level-display`, because it's semantic and `<accidental>` shares that group.

Why this matters for the design:
- `reference` and the display hints are booleans and enums, so they map cleanly onto `Bool` and
  `SymbolSize`.
- `type` on a `<note>` creates an implicit start/stop pair across notes. `mx::api` can carry it as a
  flag on each note, but nothing validates the pairing. That's like a spanner without an id: the
  client must keep starts and stops consistent.
- rpatters1 is clearly writing Finale -> MusicXML (denigma), and has been in the spec discussion. He
  knows exactly what he wants.

## `<footnote>`, `<level>`, and the "editorial" groups

`editorial`, `editorial-voice` and `editorial-voice-direction` are XSD groups, not elements: named
sequences inlined into a host's content model. The XSD even labels them (`:6133`) "Element groups
derived from common.mod entities". They're leftovers from DTD parameter entities (`%editorial;`). In
an actual file you only ever see `<footnote>`, `<level>` and `<voice>` as direct children of the
host:

```
group definition (XSD :6135)          what appears in a <note>
────────────────────────────          ───────────────────────────
editorial-voice = (                   <note>
    footnote?,                          <pitch>…</pitch>
    level?,                             <duration>4</duration>
    voice? )                            <footnote>…</footnote>   ┐ inlined
                                        <level>…</level>         │ from the
                                        <voice>1</voice>         ┘ group
                                        <type>quarter</type>
                                      </note>
```

The three groups are one idea with variations:

| Group                       | Expands to                  | Used by                                                                                  |
|-----------------------------|-----------------------------|------------------------------------------------------------------------------------------|
| `editorial`                 | `footnote?, level?`         | attributes, backup, barline, figure, figured-bass, harmony, lyric, notations, part-group |
| `editorial-voice`           | `footnote?, level?, voice?` | note, forward                                                                            |
| `editorial-voice-direction` | same as `editorial-voice`   | direction                                                                                |

`voice` is bundled only because note, forward and direction all want voice right after the editorial
pair, so the DTD authors folded it into one macro. The XSD (`:6145`) calls it "the common
combination". It isn't conceptually editorial. The direction variant exists only so XSD derivation
could later restrict it separately; it's the same content today.

`<footnote>` vs `<level>`: two complementary editorial channels.

|          | `<footnote>`                                                                     | `<level>`                                          |
|----------|----------------------------------------------------------------------------------|----------------------------------------------------|
| XSD says | "editorial information that appears in footnotes in the printed score" (`:6169`) | "editorial status of the parent element" (`:2645`) |
| Content  | `formatted-text`: text plus font, color, position, justify, enclosure            | plain string                                       |
| Purpose  | explains in prose, printed at the page foot                                      | classifies and tells a renderer how to draw it     |
| Example  | "m. 12: slur absent in the autograph"                                            | `<level parentheses="yes">editor</level>`          |

Think of it as a commit message vs a label. In a critical edition one symbol might carry both:
`<level bracket="yes">2</level>` draws the slur bracketed as layer 2, and `<footnote>` holds the
commentary text printed below. Each is optional and appears at most once per host.

In mx::core the generator keeps the groups as classes: `EditorialGroup.h`, `EditorialVoiceGroup.h`,
`EditorialVoiceDirectionGroup.h`. Each has `std::optional<Level> level()` and
`std::optional<FormattedText> footnote()`. Hosts hold the group by value (`Note::editorialVoice()`
at `src/private/mx/core/generated/Note.h:123`). That's also why `mx::impl` already touches this
group for every note: it writes `<voice>` through it. Any level work has to read-modify-write the
group so it doesn't clobber `<voice>` or the footnote.

## `##misc-data##`: an application-data side channel

Added in `4fb48a8e` (2017-02-17, "mxapi note misc-data"), this mechanism lets an app attach
arbitrary strings to a note and recover them after a MusicXML round trip. Other software ignores the
data.

The api surface is `NoteData::miscData`, a `std::vector<std::string>`
(`src/include/mx/api/NoteData.h:279`).

How it actually serializes. The trick is choosing `<footnote>`'s `font-family` attribute as the
carrier. The XSD types `font-family` as `comma-separated-text`
(`docs/musicxml-4.0-ed15c23.xsd:135`), so it's already a string-list type, and the generated core
gives it to you as a vector of items. The writer (`src/private/mx/impl/NoteWriter.cpp:757`):

```
miscData = {"Bob", "Cat", "Hello"}
      │
      ▼  prefix the first item with the magic tag; commas inside items → "_" (+ diagnostic)
<note>
  …
  <footnote font-family="##misc-data##Bob,Cat,Hello"></footnote>   ← empty text
  <voice>1</voice>
  …
```

The reader (`src/private/mx/impl/NoteFunctions.cpp:355`) only claims a footnote that has empty text
and whose first `font-family` item starts with `##misc-data##`. A real footnote with text is left
alone. It's a decent guard.

The surprise: the header comment is wrong. `NoteData.h:269-278` says the output is:

```xml
<footnote print-object="no">##misc-data##Bob,Cat,Hello</footnote>
```

The code never produced that, not even in the 2017 original, which already wrote `fontFamily`. It
also couldn't: `<footnote>` is `formatted-text`, whose attributes are text-formatting,
print-style-align, enclosure and so on. `print-object` isn't one of them, so that output would fail
schema validation. It's a stale doc bug worth an issue, or a drive-by fix in whatever PR touches
note editorial code.

Why this way (2017 trade-offs):
- Processing instructions or `<other-*>` elements: PIs weren't modeled in the core of that era.
  `ChordData::miscData` is a separate `MiscData{name,value}` mechanism that was meant for PIs, and
  the new generated core can't write it (`src/private/mx/impl/DirectionWriter.cpp:1729`, "miscData
  is skipped").
- Footnote text: other apps might print it.
- `font-family` on an empty footnote: schema-valid, invisible in practice, list-shaped for free.
  Clever, if ugly.

Costs today:
- It occupies the note's one `<footnote>` slot. A real editorial footnote and misc-data can't
  coexist on a note: if the text is non-empty, the reader skips misc-data, and the writer overwrites
  any footnote. So if `mx::api` ever models `<footnote>` properly, this collides head-on.
- Commas are lossy. They become `_` on write (now with a diagnostic).
- No coverage from real files. No corpus file uses it; only
  `src/private/mxtest/api/NoteDataTest.cpp:739` tests it.

Connection to `<level>`: `<footnote>`, `<level>` and `<voice>` all sit in `EditorialVoiceGroup`. The
misc-data writer does read-modify-write correctly (`NoteWriter.cpp:803-805`). A level writer must do
the same, or adding `<level>` silently deletes misc-data. That's a concrete test case for the PR.

## Design implications for #559, adding note-level editorial data

`<level>` serves two uses:

```
<level>
 ├─ provenance layer: text + reference   ← MuseData heritage; scholarly editions
 │     "this slur is layer 2 (pencil), don't play it"
 └─ editorial display hint: parentheses / bracket / size (+ type for spans)
       "draw this rest in parentheses"   ← what the spec owner recommends today
```

rpatters1 wants the second one, for Finale -> MusicXML. That's the use the spec owner endorses, and
the one with an actual consumer behind it. The first is real but scholarly, and it has zero
real-world corpus files.

Proposed API:

```cpp
enum class LevelType { unspecified, start, stop, single };   // MusicXML: absent means single

struct LevelData
{
    std::string text;                        // required string in MusicXML, often empty
    Bool reference = Bool::unspecified;      // yes: display only, not played
    LevelType type = LevelType::unspecified;
    Bool parentheses = Bool::unspecified;
    Bool bracket = Bool::unspecified;
    SymbolSize size = SymbolSize::unspecified;
};

// on NoteData
std::optional<LevelData> level;
```

- Presence is the optional, not the text. `<level parentheses="yes"/>` has empty content, so an
  empty `text` must still mean "present".
- Keep `type` in, even for note-only. rpatters1 raised the chord-parentheses question in the spec
  thread himself; he'll want start/stop on the first and last notes of a chord. The api carries the
  flag per note and doesn't validate pairing, which matches how mx handles other client-managed
  spans.
- The required test: put `level`, `miscData` and a voice on one note and check all three survive a
  round trip. All three share `EditorialVoiceGroup`, and dropping one is the likeliest bug.
- Header comment: say what it is for a musician ("editorial status of this note; for example,
  parentheses around a rest the editor added") and say that a rest's parentheses go here, not on the
  notehead.

Scope recommendation, unchanged: note only. The XSD's `reference` example is on `<attributes>`
(original clef/key/time). That's the strongest case for another element, but `mx::api` doesn't model
`<attributes>` as an object, so it stays out unless someone designs one.

Naming recommendation: `NoteData::level` and `LevelData` keep the MusicXML name searchable; the
comment explains the meaning. `editorialLevel` is an alternative.

Side items I'd raise as issues, not PR scope:
- The `NoteData.h:269-278` misc-data comment describes the wrong format.
- `ChordData::miscData` is silently dropped on write (`DirectionWriter.cpp:1729`).
