// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/EncodingData.h"
#include "mx/api/LayoutData.h"
#include "mx/api/PageTextData.h"
#include "mx/api/PartData.h"
#include "mx/api/PartGroupData.h"

#include <vector>
#include <cmath>
#include <algorithm>

namespace mx
{
    namespace api
    {
        constexpr const long double MX_API_EQUALITY_EPSILON = 0.00000001;

        inline bool areSame( long double left, long double right )
        {
            return ( std::abs( left - right ) < MX_API_EQUALITY_EPSILON );
        }

        constexpr int DEFAULT_TICKS_PER_QUARTER = 3 * 4 * 5 * 7;
        
        class ScoreData
        {
        public:
            std::string musicXmlType;
            std::string workTitle;
            std::string workNumber;
            std::string movementTitle;
            std::string movementNumber;
            std::string composer;
            std::string lyricist;
            std::string copyright;
            EncodingData encoding;
            std::vector<PageTextData> pageTextItems;
            LayoutData layout;
            std::vector<PartData> parts;
            std::vector<PartGroupData> partGroups;
            int ticksPerQuarter;
            
            ScoreData()
            : musicXmlType{ "partwise" }
            , workTitle{}
            , workNumber{}
            , movementTitle{}
            , movementNumber{}
            , composer{}
            , lyricist{}
            , copyright{}
            , encoding{}
            , pageTextItems{}
            , layout{}
            , parts{}
            , partGroups{}
            , ticksPerQuarter{ DEFAULT_TICKS_PER_QUARTER }
            {

            }
        };
        
        inline std::vector<PartData>::iterator findPart( std::vector<PartData>& inParts, const std::string& inPartId )
        {
            auto predicate = [&inPartId] ( const PartData& partData )
            {
                return inPartId == partData.uniqueId;
            };
            
            auto it = std::find_if( inParts.begin(), inParts.end(), predicate );
            return it;
        }
    }
}
