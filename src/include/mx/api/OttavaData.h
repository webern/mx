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

    // Octave-shift size fidelity knob. The size value (8, 15, or 22) follows from ottavaType, so
    // 15ma/15mb and 22ma/22mb lines always write their size while an 8va/8vb line omits the
    // redundant, spec-default size="8". When true, the writer also emits that redundant size="8";
    // the reader sets it when the source spelled the attribute out. Leave false (the default) when
    // authoring. It has no effect on a 15ma/15mb or 22ma/22mb line, whose size is always written.
    bool writeDefaultSize;

    OttavaStart() : spannerStart{}, ottavaType{OttavaType::unspecified}, writeDefaultSize{false}
    {
    }
};

class OttavaStop
{
  public:
    SpannerStop spannerStop;

    // Octave-shift size fidelity knob. A stop's size (8, 15, or 22) is never its own musical fact:
    // it belongs to the OttavaStart that opened the line, so the writer takes it from there. The
    // stop is paired with its start by spannerStop.number -- an identity, an explicit level, or an
    // unspecified number pairs with the nearest still-open start of the same kind, within the part
    // and across measures.
    //
    // unspecified (the default) and yes both write the size the start implies; no omits the
    // attribute. Omitting it is legal MusicXML, but some importers (e.g. MuseScore) expect it, so
    // authoring leaves this alone and gets the interoperable spelling. The reader sets yes or no to
    // record whether the source spelled the attribute out.
    //
    // A stop with no matching start writes MusicXML's default size of 8.
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
