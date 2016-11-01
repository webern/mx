// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include <string>
#include <vector>

namespace mx
{
    namespace api
    {
        // The part-group element indicates groupings of parts in the score, usually indicated
        // by braces and brackets. Braces that are used for multi-staff parts should be defined
        // in the attributes element for that part. The part-group start element appears before
        // the first score-part in the group. The part-group stop element appears after the
        // last score-part in the group.
        
        // The number attribute is used to distinguish overlapping and nested part-groups, not
        // the sequence of groups. As with parts, groups can have a name and abbreviation.
        // Values for the child elements are ignored at the stop of a group.
            
        // A part-group element is not needed for a single multi-staff part. By default, multi-
        // staff parts include a brace symbol and (if appropriate given the bar-style) common
        // barlines. The symbol formatting for a multi-staff part can be more fully specified
        // using the part-symbol element.
        
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
            
            // The number attribute is used to distinguish overlapping
            // and nested part-groups, not the sequence of groups.
            // -1 indicates the absence of a number attribute
            int number;
            
            PartGroupData()
            : firstPartIndex{ -1 }
            , lastPartIndex{ -1 }
            , name{}
            , number{ -1 }
            {
            
            }
        };
        
        MXAPI_EQUALS_BEGIN( PartGroupData )
        MXAPI_EQUALS_MEMBER( firstPartIndex )
        MXAPI_EQUALS_MEMBER( lastPartIndex )
        MXAPI_EQUALS_MEMBER( name )
        MXAPI_EQUALS_MEMBER( displayName )
        MXAPI_EQUALS_MEMBER( abbreviation )
        MXAPI_EQUALS_MEMBER( displayAbbreviation )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( PartGroupData );
    }
}
