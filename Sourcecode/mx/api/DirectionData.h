// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/ApiCommon.h"

namespace mx
{
    namespace api
    {
    	struct DirectionData
    	{
            int dummy;
            
            DirectionData()
            : dummy( 0 )
            {
                
            }
    	};
        
        MXAPI_EQUALS_BEGIN( DirectionData )
        MXAPI_EQUALS_FIRST_MEMBER( dummy )
        MXAPI_EQUALS_NEXT_MEMBER( dummy )
        MXAPI_EQUALS_LAST_MEMBER( dummy )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( DirectionData );
	}
}
