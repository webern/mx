// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/EncodingData.h"
#include "mx/api/LayoutData.h"
#include "mx/api/PageTextData.h"
#include "mx/api/PartData.h"

#include <vector>
#include <cmath>

namespace mx
{
    namespace api
    {
        constexpr const long double MX_API_EQUALITY_EPSILON = 0.00000001;

        inline bool areSame( long double left, long double right )
        {
            return ( std::abs( left - right ) < MX_API_EQUALITY_EPSILON );
        }

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

            ScoreData()
            : musicXmlType( "partwise" )
            {

            }
        };
    }
}
