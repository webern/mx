// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"

#include <string>

namespace mx
{
namespace api
{
// Whether a level covers one symbol or a run of symbols, MusicXML's type attribute of <level>.
enum class LevelType
{
    // The attribute is absent. MusicXML reads that as single.
    unspecified,

    // The first symbol of a run that the level covers.
    start,

    // The last symbol of a run that the level covers.
    stop,

    // The level covers this symbol alone.
    single
};

// The MusicXML <level> element.
//
// What <level> is, and why it has that name
//
// `<level>` is a provenance tag: metadata attached to a symbol that labels its editorial status.
// Like a `git blame` annotation, it records where a mark came from rather than changing the music.
//
// The name comes from MuseData, a 1980s-90s encoding format for critical editions. When asked about
// the confusing name, Michael Good, the creator of MusicXML, answered (w3c-cg/musicxml#146):
//
// > "This was a generalization of the integer level field in MuseData. In MusicXML it is used to
// > identify and describe different types of editorial information."
//
// In MuseData, each record could carry an integer "level". The editor defined what each number
// meant, for example 1 = from the autograph, 2 = editor's addition, 3 = from the first print. So
// "level" means an editorial layer, like a layer in a graphics program. MusicXML widened the integer
// to free text. That's why the content is just a string with no defined vocabulary.
//
// The XSD wording:
//
// > "The level type is used to specify editorial information for different MusicXML elements. The
// > content contains identifying and/or descriptive text about the editorial status of the parent
// > element."
//
// The second sentence was added in 4.0 to clarify the meaning (w3c-cg/musicxml#146).
//
// Good's example comes from his paper "Using MusicXML 2.0 for Editorial Applications" (Listing
// 19). Two slurs in a Brahms edition come from different sources:
//
//     <slur type="start" number="1"/> ... <level>1</level>
//     <slur type="start" number="2"/> ... <level reference="yes">2 (pencil)</level>
//
// Read it as: slur 1 belongs to editorial layer "1". Slur 2 belongs to layer "2 (pencil)", someone's
// pencil marking in a source, and it's shown for reference only. That's the original use case:
// scholarly editions that record where each mark came from.
//
// Why this design: MusicXML had to round-trip MuseData-style scholarly encodings without dictating
// anyone's source classification scheme. An open string tag fits that, since each edition defines
// its own layers. The cost is that the content means nothing to a program. Only the attributes carry
// machine-readable meaning.
//
// The attributes: where <level> becomes machine-readable
//
// Intuition: the text content is the human label. The attributes are three independent things a
// program can act on: a playback flag, a span marker, and display hints.
//
//     <level reference="yes|no"               playback: "this is reference material, don't play it"
//            type="start|stop|single"         span: does it cover one symbol or a run of them?
//            parentheses="yes|no"             display: how editorial-ness is drawn
//            bracket="yes|no"                 display
//            size="full|cue|grace-cue|large"  display
//     >free text label</level>
//
// `reference` (added in MusicXML 1.1). The XSD says:
//
// > "editorial information that is for display only and should not affect playback. For instance,
// > a modern edition of older music may set reference="yes" on the attributes containing the
// > music's original clef, key, and time signature."
//
// This is the "incipit" case. A modern Renaissance edition prints a small prefatory staff showing
// the original C clef and mensuration sign, then continues in treble clef. Both `<attributes>`
// elements exist in the file, but a player must ignore the original one.
//
// Display hints (`level-display`, also 1.1). The XSD:
//
// > "three common ways to indicate editorial indications: putting parentheses or square brackets
// > around a symbol, or making the symbol a different size."
//
// These are engraving conventions. An editor's added slur is often drawn dashed, an added dynamic in
// brackets, an added accidental small or in parentheses. The same attribute group sits on
// `<accidental>`, which is why AccidentalMarkData also has parentheses, bracket and size. Editorial
// accidentals were the first application.
//
// A rest is the common modern case. In w3c-cg/musicxml PR 736, Good wrote:
//
// > "Not really, since rests don't have noteheads. Parenthesizing a rest is best done with a
// > `<level parentheses="yes"/>` child element."
//
//     <note>
//       <rest/>
//       <duration>4</duration>
//       <level parentheses="yes"/>   <!-- no text: purely a display hint -->
//       <voice>1</voice>
//       <type>quarter</type>
//     </note>
//
// The element has drifted from "provenance tag" to "editorial display hint", often with empty
// content. MusicXML doesn't offer another clean way to parenthesize a rest. Some files use
// `<notehead parentheses="yes">` on a rest; Good calls that wrong because a rest doesn't have a
// notehead. To draw a rest in parentheses, give its NoteData a level with parentheses set to yes and
// the text left empty.
//
// `type` (added in 4.0, issue 288). Start/stop/single lets one `<level>` cover a run of symbols
// instead of tagging each one. In that same PR 736 thread, a Finale user asked how to parenthesize a
// whole chord (a Finale "entry"). A contributor proposed `type="start"` on the first chord note and
// `type="stop"` on the last. Good called it "a good clarification of the way this is indeed intended
// to work," but it isn't in the spec as an example. Good placed `type` on `<level>` itself, not in
// `level-display`, because it's semantic and `<accidental>` shares that group.
//
// `type` on a `<note>` creates an implicit start/stop pair across notes. Each note carries its own
// LevelData, and mx does not check that a start has a matching stop. That's like a spanner without
// an id: you must keep starts and stops consistent yourself.
//
// <footnote> and <level>
//
// `<footnote>` and `<level>` are two complementary editorial channels (see FootnoteData).
//
//     <footnote>
//       XSD says: "editorial information that appears in footnotes in the printed score"
//       Content:  formatted text: text plus font, color, position, justify, enclosure
//       Purpose:  explains in prose, printed at the page foot
//       Example:  "m. 12: slur absent in the autograph"
//
//     <level>
//       XSD says: "editorial status of the parent element"
//       Content:  plain string
//       Purpose:  classifies and tells a renderer how to draw it
//       Example:  <level parentheses="yes">editor</level>
//
// Think of it as a commit message vs a label. In a critical edition one symbol might carry both:
// `<level bracket="yes">2</level>` draws the slur bracketed as layer 2, and `<footnote>` holds the
// commentary text printed below. Each is optional and appears at most once per host.
struct LevelData
{
    // The editorial label, such as "editor", "1" or "2 (pencil)". Each edition decides what its
    // labels mean. It is often empty, as in <level parentheses="yes"/>, and the level is still
    // there: an empty text does not remove it.
    std::string text;

    // yes: the symbol is editorial material for display only, and a player should not perform it.
    // Leave it unspecified for MusicXML's default, which is no.
    Bool reference = Bool::unspecified;

    // Whether this level covers this symbol alone, or starts or stops a run of symbols. Leave it
    // unspecified for MusicXML's default, which is single.
    LevelType type = LevelType::unspecified;

    // yes: the symbol is drawn in parentheses to show that it is editorial. Leave it unspecified to
    // let the application decide.
    Bool parentheses = Bool::unspecified;

    // yes: the symbol is drawn in square brackets to show that it is editorial. Leave it
    // unspecified to let the application decide.
    Bool bracket = Bool::unspecified;

    // How large the symbol is drawn. A smaller size is another way to show that it is editorial.
    // Leave it unspecified to let the application decide.
    SymbolSize size = SymbolSize::unspecified;
};

MXAPI_EQUALS_BEGIN(LevelData)
MXAPI_EQUALS_MEMBER(text)
MXAPI_EQUALS_MEMBER(reference)
MXAPI_EQUALS_MEMBER(type)
MXAPI_EQUALS_MEMBER(parentheses)
MXAPI_EQUALS_MEMBER(bracket)
MXAPI_EQUALS_MEMBER(size)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(LevelData);
} // namespace api
} // namespace mx
