// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/VoiceData.h"
#include "mx/api/TimeSignatureData.h"

#include <string>
#include <map>

namespace mx
{
    namespace api
    {
    	using VoiceDataMap = std::map<int,VoiceData>;

        class MeasureData
        {
        public:
        	TimeSignatureData timeSignature;
            VoiceDataMap voices;
        };
    }
}
