// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/ApiCommon.h"

#include <string>

namespace mx
{
    namespace api
    {
        enum class TimeSignatureSymbol
        {
            unspecified,
            common,
            cut
        };

        class TimeSignatureData
        {
        public:
            TimeSignatureSymbol symbol;
            int beats;
            int beatType;
            bool isImplicit;
            Bool display;

            inline bool isEqualTo( const TimeSignatureData& other ) const
            {
                return    ( beats == other.beats )
                       && ( beatType == other.beatType )
                       && ( symbol == other.symbol );
            }

            TimeSignatureData()
            : symbol{ TimeSignatureSymbol::unspecified }
            , beats{ 4 }
            , beatType{ 4 }
            , isImplicit{ true }
            , display{ Bool::unspecified }
            {

            }
        };
        
        MXAPI_EQUALS_BEGIN( TimeSignatureData )
        MXAPI_EQUALS_FIRST_MEMBER( symbol )
        MXAPI_EQUALS_NEXT_MEMBER( beats )
        MXAPI_EQUALS_NEXT_MEMBER( beatType )
        MXAPI_EQUALS_NEXT_MEMBER( isImplicit )
        MXAPI_EQUALS_LAST_MEMBER( display )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( TimeSignatureData );
    }
}
