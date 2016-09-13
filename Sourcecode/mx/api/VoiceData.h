// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

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
    }
}
