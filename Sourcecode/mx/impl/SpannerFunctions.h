// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

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
            start.lineData = getLineData( inAttributes );
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
        
        MX_ATTR_SETFUNC_OPTIONAL( hasNumber, HasNumber, bool, false );
        MX_ATTR_SETFUNC_OPTIONAL_WITH_SETTER( number, Number, int, -1 );

        template <typename ATTRIBUTES_TYPE>
        void setAttributesFromSpannerStart( const api::SpannerStart& start, ATTRIBUTES_TYPE& outAttributes )
        {
            if( start.numberLevel > 0 )
            {
                lookForAndSetHasNumber( true, &outAttributes );
                lookForAndSetNumber( start.numberLevel, &outAttributes );
            }
            else
            {
                lookForAndSetHasNumber( false, &outAttributes );
                lookForAndSetNumber( 1, &outAttributes );
            }
            
        }
    }
}
