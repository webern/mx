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
        
        MXAPI_EQUALS_BEGIN( SpannerData )
        MXAPI_EQUALS_FIRST_MEMBER( numberLevel )
        MXAPI_EQUALS_NEXT_MEMBER( spannerType )
        MXAPI_EQUALS_NEXT_MEMBER( name )
        MXAPI_EQUALS_NEXT_MEMBER( tickTimePosition )
        MXAPI_EQUALS_NEXT_MEMBER( printData )
        MXAPI_EQUALS_NEXT_MEMBER( positionData )
        MXAPI_EQUALS_LAST_MEMBER( lineData )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( SpannerData );
    }
}
