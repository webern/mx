// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/DynamicsData.h"

#include <optional>
#include <string>
#include <variant>

namespace mx
{
namespace api
{

// Payload for MarkType::tremoloStart / MarkType::tremoloStop: the measured-tremolo slash count
// (MusicXML <tremolo> text value, 0-8). Absent means "not specified" (the writer falls back to a
// default). The tremoloSingle* mark types encode their slash count in the enumerator itself and do
// not use this payload.
struct TremoloMarkData
{
    std::optional<int> tremoloMarks;
};

MXAPI_EQUALS_BEGIN(TremoloMarkData)
MXAPI_EQUALS_MEMBER(tremoloMarks)
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
    std::optional<std::string> id;
};

MXAPI_EQUALS_BEGIN(OtherNotationMarkData)
MXAPI_EQUALS_MEMBER(type)
MXAPI_EQUALS_MEMBER(number)
MXAPI_EQUALS_MEMBER(smufl)
MXAPI_EQUALS_MEMBER(id)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(OtherNotationMarkData);

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
// The choice's Kind SHOULD correspond to MarkData::markType (e.g. Kind::tremolo pairs with
// MarkType::tremoloStart/tremoloStop), but this is a convention that this class does not enforce.
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
        compoundDynamics,
        otherNotation
    };

    MarkDataChoice();

    MarkDataChoice(TremoloMarkData value);

    MarkDataChoice(ArpeggiateMarkData value);

    MarkDataChoice(NonArpeggiateMarkData value);

    MarkDataChoice(OtherMarkData value);

    MarkDataChoice(CompoundDynamicsData value);

    MarkDataChoice(OtherNotationMarkData value);

    Kind kind() const;
    bool isNone() const;
    bool isTremolo() const;
    bool isArpeggiate() const;
    bool isNonArpeggiate() const;
    bool isOtherMark() const;
    bool isCompoundDynamics() const;
    bool isOtherNotation() const;

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

    // Returns a copy of the internally held CompoundDynamicsData, or a default value for another kind.
    const CompoundDynamicsData compoundDynamics() const;

    // Returns a copy of the internally held OtherNotationMarkData, or a default value for another kind.
    const OtherNotationMarkData otherNotation() const;

    bool operator==(const MarkDataChoice &other) const;

  private:
    std::variant<std::monostate, TremoloMarkData, ArpeggiateMarkData, NonArpeggiateMarkData, OtherMarkData,
                 CompoundDynamicsData, OtherNotationMarkData>
        myValue;
};

MXAPI_NOT_EQUALS_AND_VECTORS(MarkDataChoice);

} // namespace api
} // namespace mx
