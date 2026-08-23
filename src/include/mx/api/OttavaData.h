// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once
#include "mx/api/ApiCommon.h"
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
    o15mb, // 2 octaves down (writes octave-shift type="up")
    o22ma, // 3 octaves up (writes octave-shift type="down")
    o22mb  // 3 octaves down (writes octave-shift type="up")
};

class OttavaStart
{
  public:
    SpannerStart spannerStart;
    OttavaType ottavaType;

    // Most users can ignore this; leave it false. It only controls whether an 8va/8vb line writes
    // the redundant size="8" attribute. The size (8, 15, or 22) follows from ottavaType, and 8 is
    // MusicXML's default, so a 15ma/15mb or 22ma/22mb line always writes its size while an 8va/8vb
    // line omits it. When true, that redundant size="8" is written too; the flag has no effect on
    // the larger lines, whose size is always written. It exists for round-trip fidelity - reading a
    // file sets it when the source spelled the attribute out.
    bool writeDefaultSize;

    OttavaStart() : spannerStart{}, ottavaType{OttavaType::unspecified}, writeDefaultSize{false}
    {
    }
};

class OttavaStop
{
  public:
    SpannerStop spannerStop;

    // Most users can ignore this; leave it unspecified. It only controls whether the stop end of an
    // octave-shift line repeats the line's size (8, 15, or 22). The size is not the stop's own
    // fact: it belongs to the OttavaStart that opened the line, so an authored ottava states its
    // octave shift once, on the start, and the writer puts the matching size on the stop.
    //
    // unspecified (the default) and yes both write that size; no omits it. Omitting it is legal
    // MusicXML, but some importers (MuseScore among them) expect it, so the default is the
    // interoperable spelling. It exists for round-trip fidelity - reading a file sets yes or no
    // according to whether the source spelled the attribute out.
    //
    // The start a stop closes is found by spannerStop.number: an identity, an explicit level, or an
    // unspecified number pairs with the nearest still-open start of the same kind, within the part
    // and across measures. A stop that matches no start writes MusicXML's default size of 8.
    Bool writeSize;

    OttavaStop() : spannerStop{}, writeSize{Bool::unspecified}
    {
    }
};

MXAPI_EQUALS_BEGIN(OttavaStart)
MXAPI_EQUALS_MEMBER(spannerStart)
MXAPI_EQUALS_MEMBER(ottavaType)
MXAPI_EQUALS_MEMBER(writeDefaultSize)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(OttavaStart);

MXAPI_EQUALS_BEGIN(OttavaStop)
MXAPI_EQUALS_MEMBER(spannerStop)
MXAPI_EQUALS_MEMBER(writeSize)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(OttavaStop);
} // namespace api
} // namespace mx
