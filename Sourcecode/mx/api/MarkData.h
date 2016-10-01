// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include <string>

namespace mx
{
    namespace api
    {

        enum class MarkType
        {
            // articulations
            accent,
            strongAccent,
            staccato,
            tenuto,
            detachedLegato,
            staccatissimo,
            spiccato,
            scoop,
            plop,
            doit,
            falloff,
            breathMark,
            caesura,
            stress,
            unstress,
            otherArticulation
        };
        
        
        struct MarkData
        {
            int absoluteMarkId;
            MarkType markType;
            std::string name;
            std::string smuflName;
            char16_t smuflCodepoint;
            int tickPosition;
            
            // TODO - font stuff
            // TODO - placement stuff
        };
    }
}
