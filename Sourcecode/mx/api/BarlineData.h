// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/PositionData.h"

namespace mx
{
    namespace api
    {

        enum class BarlineType
        {
            unspecified,
            unsupported,
            none,
            normal,
            lightLight,
            lightHeavy,
            heavyLight,
            dotted,
            dashed,
            heavy,
            heavyHeavy
        };
        
        enum class EndingType
        {
            none,
            start,
            stop,
            discontinue
        };


        class BarlineData
        {
        public:
            int tickTimePosition;
            BarlineType barlineType;
            EndingType endingType;
            int endingNumber;
<<<<<<< 3f3f1ab43b3f03510a344fdd0d480315a1ade6bb
<<<<<<< 082227f5ac3f2004cd7884731308aef6d954274d
            bool repeat;
=======
>>>>>>> Add support for reading and writing of endings
=======
            bool repeat;
>>>>>>> Add repeat
            HorizontalAlignment location;
            
            BarlineData()
            : tickTimePosition{ 0 }
            , barlineType{ BarlineType::normal }
            , endingType{ EndingType::none }
            , endingNumber { 0 }
<<<<<<< 3f3f1ab43b3f03510a344fdd0d480315a1ade6bb
<<<<<<< 082227f5ac3f2004cd7884731308aef6d954274d
            , repeat { false }
            , location{ HorizontalAlignment::unspecified }
=======
=======
            , repeat { false }
>>>>>>> Add repeat
            , location{ HorizontalAlignment::right }
>>>>>>> Add support for reading and writing of endings
            {
            }
        };
        
        MXAPI_EQUALS_BEGIN( BarlineData )
        MXAPI_EQUALS_MEMBER( tickTimePosition )
        MXAPI_EQUALS_MEMBER( barlineType )
        MXAPI_EQUALS_MEMBER( endingType )
        MXAPI_EQUALS_MEMBER( endingNumber )
<<<<<<< 3f3f1ab43b3f03510a344fdd0d480315a1ade6bb
<<<<<<< 082227f5ac3f2004cd7884731308aef6d954274d
        MXAPI_EQUALS_MEMBER( repeat )
=======
>>>>>>> Add support for reading and writing of endings
=======
        MXAPI_EQUALS_MEMBER( repeat )
>>>>>>> Add repeat
        MXAPI_EQUALS_MEMBER( location )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( BarlineData );
    }
}
