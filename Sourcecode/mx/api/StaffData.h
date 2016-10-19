// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/ClefData.h"
#include "mx/api/VoiceData.h"
#include "mx/api/MarkData.h"
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
            std::vector<MarkData> marks;

            // for the use case where key signatures
            // must differ per staff, use this vector.
            // otherwise use the keys vector in
            // MeasureData to apply a key to all staves
            std::vector<KeyData> keys;
            
            std::map<int, VoiceData> voices;
        };
    }
}
