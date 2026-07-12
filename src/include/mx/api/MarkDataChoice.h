// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"

#include <optional>
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
        tremolo
    };

    MarkDataChoice();

    MarkDataChoice(TremoloMarkData value);

    Kind kind() const;
    bool isNone() const;
    bool isTremolo() const;

    // Returns a copy of the internally held TremoloMarkData.
    //
    // Check isTremolo() first. If this is not a tremolo payload, a default constructed
    // TremoloMarkData is returned.
    const TremoloMarkData tremolo() const;

    bool operator==(const MarkDataChoice &other) const;

  private:
    std::variant<std::monostate, TremoloMarkData> myValue;
};

MXAPI_NOT_EQUALS_AND_VECTORS(MarkDataChoice);

} // namespace api
} // namespace mx
