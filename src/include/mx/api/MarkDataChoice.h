// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"

#include <optional>
#include <string>
#include <variant>

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
    // has no slash count, so neither uses this field.
    std::optional<int> tremoloMarks;

    // The canonical SMuFL glyph name to draw for an unmeasured tremolo (MarkType::tremoloUnmeasured),
    // from SMuFL's Tremolos range -- for example "pendereckiTremolo" or "unmeasuredTremolo". Leave
    // it absent to accept SMuFL's "buzzRoll", the glyph MusicXML assumes when no name is given.
    //
    // MusicXML allows this on a one-note or two-note tremolo too, but defines no meaning for it
    // there, and most applications ignore it.
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
    std::optional<std::string> id;
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
    std::optional<std::string> id;
};

MXAPI_EQUALS_BEGIN(NonArpeggiateMarkData)
MXAPI_EQUALS_MEMBER(placement)
MXAPI_EQUALS_MEMBER(number)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(NonArpeggiateMarkData);

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
        nonArpeggiate
    };

    MarkDataChoice();

    MarkDataChoice(TremoloMarkData value);

    MarkDataChoice(ArpeggiateMarkData value);

    MarkDataChoice(NonArpeggiateMarkData value);

    Kind kind() const;
    bool isNone() const;
    bool isTremolo() const;
    bool isArpeggiate() const;
    bool isNonArpeggiate() const;

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

    bool operator==(const MarkDataChoice &other) const;

  private:
    std::variant<std::monostate, TremoloMarkData, ArpeggiateMarkData, NonArpeggiateMarkData> myValue;
};

MXAPI_NOT_EQUALS_AND_VECTORS(MarkDataChoice);

} // namespace api
} // namespace mx
