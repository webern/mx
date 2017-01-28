// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

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
            
            // common, cut
            TimeSignatureSymbol symbol;
            
            // the top number of the time signature, e.g. '5' in a '5/4' signature
            int beats;
            
            // the bottom number of the time signature, e.g. '4' in a '5/4' signature
            int beatType;
            
            // a time signature is implicit when it is not specified by the musicxml
            bool isImplicit;
            
            // use this to hide a time sigature with Bool::no. If a time signature is
            // implicit, the 'display' field will be ignored
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
        MXAPI_EQUALS_MEMBER( symbol )
        MXAPI_EQUALS_MEMBER( beats )
        MXAPI_EQUALS_MEMBER( beatType )
        MXAPI_EQUALS_MEMBER( isImplicit )
        MXAPI_EQUALS_MEMBER( display )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( TimeSignatureData );
    }
}
