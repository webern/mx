// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/PositionData.h"
#include "mx/api/PrintData.h"

#include <string>

namespace mx
{
    namespace api
    {

        enum class MarkType
        {
            // articulations
            accent,
            strongAccent,    // marcato
            staccato,
            tenuto,
            detachedLegato,  // tenuto with dot
            staccatissimo,
            spiccato,        // ? using "StaccatissimoWedge"
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

        using MarkSmuflEntry = std::pair<const MarkType, const SmuflGlyphname>;
        using MarkSmuflMap = std::map<const MarkType, const SmuflGlyphname>;
        
        class MarkSmufl
        {
        public:
            static const std::string& getName( MarkType mark );
            static const std::string& getName( MarkType mark, Placement placement );
            static char16_t getCodepoint( MarkType mark );
            static char16_t getCodepoint( MarkType mark, Placement placement );
            static const SmuflGlyphname& getSmuflGlyphname( MarkType mark );
            static const MarkSmuflMap& getMap();
            
        private:
            MarkSmufl();
            static const MarkSmufl& getInstance();
            MarkSmuflMap myMap;
        };
        
        
        struct MarkData
        {
            int absoluteMarkId;
            MarkType markType;
            std::string name;
            std::string smuflName;
            char16_t smuflCodepoint;
            int tickPosition;
            PrintData print;
            PositionData position;
       };
    }
}
