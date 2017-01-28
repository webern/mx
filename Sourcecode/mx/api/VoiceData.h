// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/NoteData.h"

#include <string>
#include <vector>

namespace mx
{
    namespace api
    {
        class VoiceData
        {
        public:
            std::vector<NoteData> notes;
        };

        
        inline bool operator==( const VoiceData& lhs, const VoiceData& rhs )
        {
            return areVectorsEqual( lhs.notes, rhs.notes );
        }
        MXAPI_NOT_EQUALS_AND_VECTORS( VoiceData );
    }
}
