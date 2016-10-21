// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/EncodingData.h"
#include "mx/api/LayoutData.h"
#include "mx/api/PageTextData.h"
#include "mx/api/PartData.h"
#include "mx/api/PartGroupData.h"
#include "mx/api/SystemData.h"

#include <vector>
#include <set>

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
            std::set<SystemData> systems;
            
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
                    int temp = static_cast<int>( part.measures.size() );
                    if( temp > numMeasures )
                    {
                        numMeasures = temp;
                    }
                }

                return numMeasures;
            }


            inline int getNumStavesPerSystem() const
            {
                int numStaves = 0;

                for( const auto& p : parts )
                {
                    numStaves += p.getNumStaves();
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
        
        MXAPI_EQUALS_BEGIN( ScoreData )
        MXAPI_EQUALS_FIRST_MEMBER( musicXmlType )
        MXAPI_EQUALS_NEXT_MEMBER( workTitle )
        MXAPI_EQUALS_NEXT_MEMBER( workNumber )
        MXAPI_EQUALS_NEXT_MEMBER( movementTitle )
        MXAPI_EQUALS_NEXT_MEMBER( movementNumber )
        MXAPI_EQUALS_NEXT_MEMBER( composer )
        MXAPI_EQUALS_NEXT_MEMBER( lyricist )
        MXAPI_EQUALS_NEXT_MEMBER( arranger )
        MXAPI_EQUALS_NEXT_MEMBER( publisher )
        MXAPI_EQUALS_NEXT_MEMBER( copyright )
        MXAPI_EQUALS_NEXT_MEMBER( encoding )
        MXAPI_EQUALS_NEXT_MEMBER( pageTextItems )
        MXAPI_EQUALS_NEXT_MEMBER( layout )
        MXAPI_EQUALS_NEXT_MEMBER( parts )
        MXAPI_EQUALS_NEXT_MEMBER( partGroups )
        MXAPI_EQUALS_NEXT_MEMBER( ticksPerQuarter )
        MXAPI_EQUALS_LAST_MEMBER( systems )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( ScoreData );
    }
}
