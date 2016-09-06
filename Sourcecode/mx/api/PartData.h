// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/StaffData.h"

#include <string>
#include <vector>

namespace mx
{
    namespace api
    {
        class PartData
        {
        public:
            std::string uniqueId;
            std::string name;
            std::string abbreviation;
            std::string displayName;
            std::string displayAbbreviation;
            std::string instrumentUniqueId;
            std::string instrumentName;
            std::string instrumentSound;
            std::string virtualLibrary;
            std::string virtualName;
            std::string midiDevice;
            std::string midiUniqueId;
            int midiChannel;
            int midiProgram;
            int midiVolume;
            int midiPan;
            std::vector<StaffData> staves;
        };
    }
}
