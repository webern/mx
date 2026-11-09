// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/DynamicsData.h"
#include "mx/api/Id.h"
#include "mx/api/PitchData.h"
#include "mx/api/PositionData.h"
#include "mx/api/PrintData.h"

#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace mx
{
namespace api
{

// Payload for the tremolo mark types. Which kind of tremolo a mark is -- a one-note tremolo, one
// end of a two-note (measured) tremolo, or an unmeasured tremolo -- is stated by the MarkType; this
// payload carries what the MarkType cannot say.
struct TremoloMarkData
{
    // How many slashes are drawn through the stem of a two-note tremolo, 0 to 8. Set this on the
    // MarkType::tremoloStart and MarkType::tremoloStop marks that bracket the pair, using the same
    // count on both. Leave it absent to accept a three-slash default.
    //
    // A one-note tremolo states its slash count in the MarkType itself
    // (MarkType::tremoloSingleOne through MarkType::tremoloSingleFive), and an unmeasured tremolo
    // does not have a slash count, so neither uses this field.
    std::optional<int> tremoloMarks;

    // The canonical SMuFL glyph name to draw for an unmeasured tremolo (MarkType::tremoloUnmeasured),
    // from SMuFL's Tremolos range -- for example "pendereckiTremolo" or "unmeasuredTremolo". Leave
    // it absent to accept SMuFL's "buzzRoll", the glyph MusicXML assumes when no name is given.
    //
    // MusicXML allows this on a one-note or two-note tremolo too, but does not define a meaning
    // for it there, and most applications ignore it.
    std::optional<std::string> smufl;
};

MXAPI_EQUALS_BEGIN(TremoloMarkData)
MXAPI_EQUALS_MEMBER(tremoloMarks)
MXAPI_EQUALS_MEMBER(smufl)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(TremoloMarkData);

// Payload for the arpeggiate mark types (MarkType::arpeggiate / arpeggiateUp / arpeggiateDown).
// The up/down direction of the arpeggio's wavy line is encoded in the MarkType itself.
struct ArpeggiateMarkData
{
    // Distinguishes overlapping arpeggios: notes sharing a number belong to one arpeggio (e.g.
    // an arpeggio spanning both staves of a keyboard part) while different numbers arpeggiate
    // independently. Corresponds to the `number` attribute.
    std::optional<int> number;

    // yes: the arpeggio is drawn as one unbroken line across staves instead of one line per
    // staff. Corresponds to the `unbroken` attribute (MusicXML 4.0).
    Bool unbroken = Bool::unspecified;

    // The element's `id` attribute (MusicXML 3.1).
    std::optional<Id> id;
};

MXAPI_EQUALS_BEGIN(ArpeggiateMarkData)
MXAPI_EQUALS_MEMBER(number)
MXAPI_EQUALS_MEMBER(unbroken)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(ArpeggiateMarkData);

// Which end of a non-arpeggiate bracket a note carries: the bracket's top or bottom.
enum class NonArpeggiatePlacement
{
    top,
    bottom,
};

// Payload for MarkType::nonArpeggiate: a bracket indicating that a chord is NOT to be
// arpeggiated. MusicXML marks only the top and bottom notes of the chord, each carrying a
// <non-arpeggiate> element that states which end of the bracket it is.
struct NonArpeggiateMarkData
{
    // Which end of the bracket this note carries. Corresponds to the required `type` attribute.
    NonArpeggiatePlacement placement = NonArpeggiatePlacement::top;

    // Distinguishes overlapping brackets, like ArpeggiateMarkData::number. Corresponds to the
    // `number` attribute.
    std::optional<int> number;

    // The element's `id` attribute (MusicXML 3.1).
    std::optional<Id> id;
};

MXAPI_EQUALS_BEGIN(NonArpeggiateMarkData)
MXAPI_EQUALS_MEMBER(placement)
MXAPI_EQUALS_MEMBER(number)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(NonArpeggiateMarkData);

// The exact glyph used by an other-articulation, other-dynamics, other-ornament, or
// other-technical mark. The mark's visible fallback text remains in MarkData::name.
struct OtherMarkData
{
    std::optional<std::string> smufl;
};

MXAPI_EQUALS_BEGIN(OtherMarkData)
MXAPI_EQUALS_MEMBER(smufl)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(OtherMarkData);

// Whether an other-notation is a standalone symbol or one end of a multi-note notation.
enum class OtherNotationType
{
    start,
    stop,
    single
};

// Payload for MusicXML's general other-notation extension. The visible fallback text, position,
// and print appearance use MarkData's common fields.
struct OtherNotationMarkData
{
    OtherNotationType type = OtherNotationType::single;
    std::optional<int> number;
    std::optional<std::string> smufl;
    std::optional<Id> id;
};

MXAPI_EQUALS_BEGIN(OtherNotationMarkData)
MXAPI_EQUALS_MEMBER(type)
MXAPI_EQUALS_MEMBER(number)
MXAPI_EQUALS_MEMBER(smufl)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(OtherNotationMarkData);

// How a harmonic is produced on a string instrument.
enum class HarmonicKind
{
    // Leave the production method unstated, drawing only the circular harmonic symbol.
    unspecified,

    // The string sounds open and is touched lightly at a node, so the harmonic's pitch is fixed by
    // where the node falls on the open string.
    natural,

    // The string is stopped at one pitch and touched lightly a fixed interval above it -- usually a
    // fourth, sometimes a major third or a fifth. The stopped note is written as an ordinary
    // notehead and the touched note as a diamond (see NoteData::notehead).
    artificial,
};

// Which of a harmonic's three pitches the note's written notehead states.
enum class HarmonicPitch
{
    // Leave it unstated. Notation that writes only one note for the harmonic, or that relies on
    // notehead shape alone to say which pitch is meant, does not need this.
    unspecified,

    // The stopped pitch: where the finger presses the string. The lower note of an artificial
    // harmonic pair.
    basePitch,

    // The lightly touched pitch. The upper, diamond-notehead note of an artificial harmonic pair,
    // and the node touched on an open string for a natural harmonic.
    touchingPitch,

    // The pitch that actually sounds. Written when the notation states the sounding result
    // explicitly, often as a small or parenthesized note above the pair.
    soundingPitch,
};

// Payload for MarkType::harmonic: the <harmonic> symbol placed on a string-instrument note.
//
// A note in an artificial harmonic pair carries HarmonicKind::artificial along with the
// HarmonicPitch its notehead states -- basePitch on the stopped note, touchingPitch on the diamond
// note -- so each note says what it is on its own. Leaving both fields unspecified draws the plain
// circular harmonic symbol, which is what a natural harmonic most often needs.
struct HarmonicMarkData
{
    // Whether the harmonic is natural or artificial.
    HarmonicKind kind = HarmonicKind::unspecified;

    // Which pitch this note's notehead states.
    HarmonicPitch pitch = HarmonicPitch::unspecified;
};

MXAPI_EQUALS_BEGIN(HarmonicMarkData)
MXAPI_EQUALS_MEMBER(kind)
MXAPI_EQUALS_MEMBER(pitch)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(HarmonicMarkData);

// Payload for the accidental mark types (MarkType::accidentalMarkSharp and the rest): how the
// accidental is drawn. Its font and color are in MarkData::printData, whose printObject is not used.
// Leaving every field at its default is the same as having no payload.
struct AccidentalMarkData
{
    // yes: the accidental is drawn in parentheses. Parentheses and square brackets are common ways
    // to show an editorial accidental. Leave it unspecified to let the application decide.
    Bool parentheses = Bool::unspecified;

    // yes: the accidental is drawn in square brackets. Leave it unspecified to let the application
    // decide.
    Bool bracket = Bool::unspecified;

    // How large the accidental is drawn. Like parentheses and brackets, a different size is a common
    // way to show an editorial accidental.
    SymbolSize size = SymbolSize::unspecified;

    // The SMuFL glyph to draw when a specific one is wanted, for example "accidentalSharp". It must
    // be the name of a SMuFL accidental, one that starts with acc, medRenFla, medRenNatura,
    // medRenShar or kievanAccidental; another name is not written. Leave it absent to draw the
    // accidental the MarkType names.
    std::optional<std::string> smufl;

    // An identifier for this accidental, unique in the document (see Id.h).
    std::optional<Id> id;
};

MXAPI_EQUALS_BEGIN(AccidentalMarkData)
MXAPI_EQUALS_MEMBER(parentheses)
MXAPI_EQUALS_MEMBER(bracket)
MXAPI_EQUALS_MEMBER(size)
MXAPI_EQUALS_MEMBER(smufl)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(AccidentalMarkData);

// An accidental printed with an ornament: the sharp over a trill that raises its upper note, or the
// accidentals above and below a turn that alter its upper and lower notes.
struct OrnamentAccidental
{
    // The accidental to show. An entry left at none is not written.
    Accidental accidental = Accidental::none;

    // Where the accidental sits, measured from the ornament rather than from the note. Placement
    // above puts the accidental over the ornament, below puts it under. The horizontal and vertical
    // alignment fields are not used.
    PositionData positionData;

    // The accidental's font and color. printObject is not used.
    PrintData printData;

    // As AccidentalMarkData::parentheses.
    Bool parentheses = Bool::unspecified;

    // As AccidentalMarkData::bracket.
    Bool bracket = Bool::unspecified;

    // As AccidentalMarkData::size.
    SymbolSize size = SymbolSize::unspecified;

    // As AccidentalMarkData::smufl. Leave it absent to draw the accidental named by accidental.
    std::optional<std::string> smufl;

    // As AccidentalMarkData::id.
    std::optional<Id> id;
};

MXAPI_EQUALS_BEGIN(OrnamentAccidental)
MXAPI_EQUALS_MEMBER(accidental)
MXAPI_EQUALS_MEMBER(positionData)
MXAPI_EQUALS_MEMBER(printData)
MXAPI_EQUALS_MEMBER(parentheses)
MXAPI_EQUALS_MEMBER(bracket)
MXAPI_EQUALS_MEMBER(size)
MXAPI_EQUALS_MEMBER(smufl)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(OrnamentAccidental);

// Payload for an ornament that has accidentals. A trill whose upper note is sharpened is one
// MarkType::trillMark mark holding an OrnamentAccidental with Accidental::sharp.
//
// These marks can hold accidentals: a trill, a turn of any kind, a shake, a mordent, an inverted
// mordent, a schleifer and a haydn. A tremolo or an other-ornament cannot, because its choice
// already carries TremoloMarkData or OtherMarkData; neither can a wavy line, which is not a mark.
// An accidental written with one of those, or one that stands above or below the note on its own,
// is a MarkType::accidentalMark* mark of its own. On any other mark this payload is not written.
struct OrnamentMarkData
{
    // The ornament's accidentals, usually one. A turn may have one above it and one below it.
    std::vector<OrnamentAccidental> accidentals;
};

MXAPI_EQUALS_BEGIN(OrnamentMarkData)
MXAPI_EQUALS_MEMBER(accidentals)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(OrnamentMarkData);

// A variant class that carries data for MarkType values whose payload does not fit MarkData's
// common fields.
//
// Justification: MarkData represents every kind of MusicXML mark (articulation, ornament,
// technical, fermata, dynamic, ...) with a single struct. Most of MarkData's fields are common to
// (nearly) every mark. A minority of fields only make sense for one specific mark, or a narrow
// family of marks; adding those directly to MarkData as ad hoc fields does not scale and leaves it
// unclear which fields apply to which mark. New mark-specific payloads belong here, as a new
// alternative, rather than as a new direct field on MarkData.
//
// The choice's Kind SHOULD correspond to MarkData::markType (e.g. Kind::tremolo pairs with the
// MarkType::tremolo* values), but this is a convention that this class does not enforce.
//
// Defaults to none (no mark-specific payload).
class MarkDataChoice
{
  public:
    enum class Kind
    {
        none,
        tremolo,
        arpeggiate,
        nonArpeggiate,
        otherMark,
        dynamic,
        compoundDynamics,
        otherNotation,
        harmonic,
        ornament,
        accidentalMark
    };

    MarkDataChoice();

    MarkDataChoice(TremoloMarkData value);

    MarkDataChoice(ArpeggiateMarkData value);

    MarkDataChoice(NonArpeggiateMarkData value);

    MarkDataChoice(OtherMarkData value);

    MarkDataChoice(StandardDynamic value);

    // Builds a compound dynamic, unless the value is a single standard symbol, in which case the
    // result is a Kind::dynamic choice (auto-collapse). A lone other-dynamics symbol does not
    // collapse -- it does not have a dedicated MusicXML element, so it stays a compound of one.
    MarkDataChoice(CompoundDynamicsData value);

    MarkDataChoice(OtherNotationMarkData value);

    MarkDataChoice(HarmonicMarkData value);

    // Builds an ornament payload, unless it has no accidentals, in which case the result is none:
    // an ornament without accidentals needs no payload.
    MarkDataChoice(OrnamentMarkData value);

    // Builds an accidental mark payload, unless every field is left at its default, in which case
    // the result is none.
    MarkDataChoice(AccidentalMarkData value);

    Kind kind() const;
    bool isNone() const;
    bool isTremolo() const;
    bool isArpeggiate() const;
    bool isNonArpeggiate() const;
    bool isOtherMark() const;
    bool isDynamic() const;
    bool isCompoundDynamics() const;
    bool isOtherNotation() const;
    bool isHarmonic() const;
    bool isOrnament() const;
    bool isAccidentalMark() const;

    // Returns a copy of the internally held TremoloMarkData.
    //
    // Check isTremolo() first. If this is not a tremolo payload, a default constructed
    // TremoloMarkData is returned.
    const TremoloMarkData tremolo() const;

    // Returns a copy of the internally held ArpeggiateMarkData.
    //
    // Check isArpeggiate() first. If this is not an arpeggiate payload, a default constructed
    // ArpeggiateMarkData is returned.
    const ArpeggiateMarkData arpeggiate() const;

    // Returns a copy of the internally held NonArpeggiateMarkData.
    //
    // Check isNonArpeggiate() first. If this is not a non-arpeggiate payload, a default
    // constructed NonArpeggiateMarkData is returned.
    const NonArpeggiateMarkData nonArpeggiate() const;

    // Returns a copy of the internally held OtherMarkData, or a default value for another kind.
    const OtherMarkData otherMark() const;

    // Returns the standard dynamic symbol, or p for another kind.
    StandardDynamic dynamic() const;

    // Returns a copy of the internally held CompoundDynamicsData, or a default value for another kind.
    const CompoundDynamicsData compoundDynamics() const;

    // Returns a copy of the internally held OtherNotationMarkData, or a default value for another kind.
    const OtherNotationMarkData otherNotation() const;

    // Returns a copy of the internally held HarmonicMarkData.
    //
    // Check isHarmonic() first. If this is not a harmonic payload, a default constructed
    // HarmonicMarkData is returned.
    const HarmonicMarkData harmonic() const;

    // Returns a copy of the internally held OrnamentMarkData.
    //
    // Check isOrnament() first. If this is not an ornament payload, a default constructed
    // OrnamentMarkData, with no accidentals, is returned.
    const OrnamentMarkData ornament() const;

    // Returns a copy of the internally held AccidentalMarkData.
    //
    // Check isAccidentalMark() first. If this is not an accidental mark payload, a default
    // constructed AccidentalMarkData is returned.
    const AccidentalMarkData accidentalMark() const;

    bool operator==(const MarkDataChoice &other) const;

  private:
    std::variant<std::monostate, TremoloMarkData, ArpeggiateMarkData, NonArpeggiateMarkData, OtherMarkData,
                 StandardDynamic, CompoundDynamicsData, OtherNotationMarkData, HarmonicMarkData, OrnamentMarkData,
                 AccidentalMarkData>
        myValue;
};

MXAPI_NOT_EQUALS_AND_VECTORS(MarkDataChoice);

} // namespace api
} // namespace mx
