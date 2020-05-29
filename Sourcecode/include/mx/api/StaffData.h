// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ClefData.h"
#include "mx/api/VoiceData.h"
#include "mx/api/DirectionData.h"
#include "mx/api/KeyData.h"

#include <vector>

namespace mx
{
    namespace api
    {
        class StaffData
        {
        public:
            std::vector<ClefData> clefs;

            // for the use case where key signatures
            // must differ per staff, use this vector.
            // otherwise use the keys vector in
            // MeasureData to apply a key to all staves
            std::vector<KeyData> keys;
            std::vector<DirectionData> directions;
            std::map<int, VoiceData> voices;
        };
        
        inline bool voicesAreEqual( const std::map<int, VoiceData>& l, const std::map<int, VoiceData>& r )
        {
            if( l.size() != r.size() )
            {
                return false;
            }
            auto li = l.cbegin();
            auto ri = r.cbegin();
            auto le = l.cend();
            for( ; li != le; ++li, ++ri )
            {
                if( li->first != ri->first )
                {
                    return false;
                }
                else if ( li->second != ri->second )
                {
                    return false;
                }
            }
            return true;
        }
        
        MXAPI_EQUALS_BEGIN( StaffData )
        MXAPI_EQUALS_MEMBER( clefs )
        MXAPI_EQUALS_MEMBER( keys )
        MXAPI_EQUALS_MEMBER( directions )
        if( !( voicesAreEqual( lhs.voices, rhs.voices ) ) )
        {
            MX_SHOW_UNEQUAL( "StaffData", "voices" );
            return false;
        }
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( StaffData );
    }
}
