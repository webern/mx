// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/impl/PositionFunctions.h"
#include "mx/impl/PrintFunctions.h"
#include "mx/impl/LineFunctions.h"
#include "mx/api/SpannerData.h"

namespace mx
{
    namespace impl
    {
        using LongDouble = long double;
        
        MX_ATTR_FUNC_OPTIONAL( hasNumber, HasNumber, bool, false );
        MX_ATTR_FUNC_OPTIONAL_WITH_GETTER( number, Number, int, -1 );

        template <typename ATTRIBUTES_TYPE>
        api::SpannerStart getSpannerStart( const ATTRIBUTES_TYPE& inAttributes )
        {
            api::SpannerStart start;
            start.numberLevel = -1;
            if( checkHasNumber( &inAttributes ) )
            {
                start.numberLevel = checkNumber( &inAttributes );
            }
            start.positionData = getPositionData( inAttributes );
            start.printData = getPrintData( inAttributes );
            
            // TODO - why doesn't this line compile?
            //start.lineData = getLineData( inAttributes );
            return start;
        }
        
        template <typename ATTRIBUTES_TYPE>
        api::SpannerStop getSpannerStop( const ATTRIBUTES_TYPE& inAttributes )
        {
            api::SpannerStop stop;
            stop.numberLevel = -1;
            if( checkHasNumber( &inAttributes ) )
            {
                stop.numberLevel = checkNumber( &inAttributes );
            }
            stop.positionData = getPositionData( inAttributes );
            return stop;
        }
    }
}
