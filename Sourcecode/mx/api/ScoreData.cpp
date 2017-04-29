// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/ScoreData.h"

#include <vector>
#include <set>

namespace mx
{
    namespace api
    {

        ScoreData::ScoreData()
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

        int ScoreData::getNumMeasures() const
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


        int ScoreData::getNumStavesPerSystem() const
        {
            int numStaves = 0;

            for( const auto& p : parts )
            {
                numStaves += p.getNumStaves();
            }

            return numStaves;
        }

        std::vector<PartData>::iterator findPart( std::vector<PartData>& inParts, const std::string& inPartId )
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
