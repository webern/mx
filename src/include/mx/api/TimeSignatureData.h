// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"

#include <string>
#include <vector>

namespace mx
{
namespace api
{
enum class TimeSignatureSymbol
{
    unspecified,
    common,
    cut,
    singleNumber
};

// TimeSignatureComponent represents a single beats/beat-type pair. Most time signatures have
// exactly one component (e.g. "4/4"). Composite time signatures (e.g. "3+2/8" notated with
// multiple <time> children, or an additive meter written as separate beats/beat-type pairs)
// are represented by multiple components.
struct TimeSignatureComponent
{
    // the top number of the time signature, e.g. "5" in a "5/4" or "3+2" in a "(3+2)/8"
    std::string beats;

    // the bottom number of the time signature, e.g. "4" in a "5/4"
    std::string beatType;
};

MXAPI_EQUALS_BEGIN(TimeSignatureComponent)
MXAPI_EQUALS_MEMBER(beats)
MXAPI_EQUALS_MEMBER(beatType)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(TimeSignatureComponent);

class TimeSignatureData
{
  public:
    // common, cut
    TimeSignatureSymbol symbol;

    // one beats/beat-type pair per component. most time signatures have exactly one entry; a
    // composite/additive time signature (e.g. multiple <time> beats/beat-type pairs) has more
    // than one. use setSimple() to populate the common single-component case.
    std::vector<TimeSignatureComponent> components;

    // a time signature is implicit when it is not specified by the musicxml
    bool isImplicit;

    // use this to hide a time sigature with Bool::no. If a time signature is
    // implicit, the 'display' field will be ignored
    Bool display;

    // this value is optional. INDEX_UNSPECIFIED means unspecified. when value is
    // unspecified it means that the time signature applies to all staves within the part
    int staffIndex;

    // convenience helper for the common case of a single beats/beat-type pair.
    void setSimple(const std::string &beats, const std::string &beatType)
    {
        components.clear();
        components.push_back(TimeSignatureComponent{beats, beatType});
    }

    inline bool isEqualTo(const TimeSignatureData &other) const
    {
        return (components == other.components) && (symbol == other.symbol) && (staffIndex == other.staffIndex);
    }

    TimeSignatureData()
        : symbol{TimeSignatureSymbol::unspecified}, components{TimeSignatureComponent{"4", "4"}}, isImplicit{true},
          display{Bool::unspecified}, staffIndex{INDEX_UNSPECIFIED}
    {
    }
};

MXAPI_EQUALS_BEGIN(TimeSignatureData)
MXAPI_EQUALS_MEMBER(symbol)
MXAPI_EQUALS_MEMBER(components)
MXAPI_EQUALS_MEMBER(isImplicit)
MXAPI_EQUALS_MEMBER(display)
MXAPI_EQUALS_MEMBER(staffIndex)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(TimeSignatureData);
} // namespace api
} // namespace mx
