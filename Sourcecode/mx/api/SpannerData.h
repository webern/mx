// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/PositionData.h"
#include "mx/api/PrintData.h"
#include "mx/api/LineData.h"

#include <string>

namespace mx
{
    namespace api
    {
        enum class SpannerType
        {
            unspecified,
            wavyLine,
            beatRepeat,      // ?
            measureRepeat,   // ?
            slashNotation,   // ?
            ending,          // ?
            barre,
            bracket,
            dashes,
            pedal,           // ?
            extend,          // ?
            glissando,
            hammerOnPullOff, // ?
            slide,
            slur,
            tie,
            tremolo,
        };
        
        struct SpannerData
        {
            int numberLevel;
            SpannerType spannerType;
            std::string name;
            int tickTimePosition;
            PrintData printData;
            PositionData positionData;
            LineData lineData;
        };
    }
}
