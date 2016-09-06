// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include <string>
#include <vector>

namespace mx
{
    namespace api
    {
        class PartGroupData
        {
        public:
            int firstPartIndex;
            int lastPartIndex;
            std::string name;
            std::string displayName;
            std::string abbreviation;
            std::string displayAbbreviation;
            // TODO - group symbol
            // TODO - group barline
            // TODO - group time
            // TODO - group editorial
            
            PartGroupData()
            : firstPartIndex( -1 )
            , lastPartIndex( -1 )
            , name()
            {
            
            }
        };
    }
}
