// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once
#include "mx/api/SpannerData.h"

namespace mx
{
namespace api
{
// Names follow common notation usage (an "8va" line raises the sounding pitch an octave above
// what is written). Per the MusicXML spec, the underlying octave-shift/@type attribute instead
// describes the direction the *written* notes are shifted from the true pitch, which is the
// opposite sense: o8va/o15ma write type="down" and o8vb/o15mb write type="up".
enum class OttavaType
{
    unspecified,
    o8va,  // octave up (writes octave-shift type="down")
    o8vb,  // octave down (writes octave-shift type="up")
    o15ma, // 2 octaves up (writes octave-shift type="down")
    o15mb  // 2 octaves down (writes octave-shift type="up")
};

class OttavaStart
{
  public:
    SpannerStart spannerStart;
    OttavaType ottavaType;

    OttavaStart() : spannerStart{}, ottavaType{OttavaType::unspecified}
    {
    }
};

// SpannerStop is used for OttavaStops

MXAPI_EQUALS_BEGIN(OttavaStart)
MXAPI_EQUALS_MEMBER(spannerStart)
MXAPI_EQUALS_MEMBER(ottavaType)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(OttavaStart);
} // namespace api
} // namespace mx
