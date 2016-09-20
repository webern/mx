// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/MxApiCommon.h"
#include "mx/api/EncodingData.h"
#include "mx/api/LayoutData.h"
#include "mx/api/PageTextData.h"
#include "mx/api/PartData.h"
#include "mx/api/PartGroupData.h"

#include <vector>

namespace mx
{
    namespace api
    {
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
            std::string arranger;
            std::string publisher;
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

            inline int getNumMeasures() const
            {
                int numMeasures = 0;

                for( const auto& part : parts )
                {
                    for( const auto& staff : part.staves )
                    {
                        int size = static_cast<int>(staff.measures.size());

                        if( size > numMeasures )
                        {
                            numMeasures = size;
                        }
                    }
                }

                return numMeasures;
            }


            inline int getNumStaves() const
            {
                int numStaves = 0;

                for( const auto& p : parts )
                {
                    numStaves += static_cast<int>( p.staves.size() );
                }

                return numStaves;
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
