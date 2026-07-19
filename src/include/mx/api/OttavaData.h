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

    // Octave-shift size fidelity knob. The size value (8 or 15) follows from ottavaType, so a
    // 15ma/15mb line always writes size="15" while an 8va/8vb line omits the redundant, spec-
    // default size="8". When true, the writer also emits that redundant size="8"; the reader sets
    // it when the source spelled the attribute out. Leave false (the default) when authoring. It
    // has no effect on a 15ma/15mb line, whose size is always written.
    bool writeDefaultSize;

    OttavaStart() : spannerStart{}, ottavaType{OttavaType::unspecified}, writeDefaultSize{false}
    {
    }
};

class OttavaStop
{
  public:
    SpannerStop spannerStop;

    // MusicXML's octave-shift allows a size attribute ("8" or "15") on the stop, not just the
    // start. Most writers omit it there since it is implied by the corresponding start, but some
    // importers (e.g. MuseScore) expect it to be present. Absent by default; set it to have the
    // writer emit it explicitly.
    std::optional<int> size;

    OttavaStop() : spannerStop{}, size{std::nullopt}
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
MXAPI_EQUALS_MEMBER(size)
MXAPI_EQUALS_END;
MXAPI_NOT_EQUALS_AND_VECTORS(OttavaStop);
} // namespace api
} // namespace mx
