// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/MeasureLocation.h"
#include "mx/api/ScoreData.h"
namespace mx
{
    namespace api
    {
        MeasureLocation::MeasureLocation()
        : partIndex{ 0 }
        , staffIndex{ 0 }
        , measureIndex{ 0 }
        {
            
        }
        
        bool MeasureLocation::isValid( const ScoreData& scoreData ) const
        {
            if( partIndex < 0    ||
                staffIndex < 0   ||
                measureIndex < 0 )
            {
                return false;
            }
            
            if( static_cast<size_t>( partIndex ) > scoreData.parts.size() - 1 )
            {
                return false;
            }
            
            const auto& part = scoreData.parts.at( static_cast<size_t>( partIndex ) );
            
            if( static_cast<size_t>( measureIndex) > part.measures.size() - 1 )
            {
                return false;
            }
            
            const auto& measure = part.measures.at( static_cast<size_t>( measureIndex ) );
            
            if( static_cast<size_t>( staffIndex ) > measure.staves.size() - 1 )
            {
                return false;
            }
                        
            return true;
        }
    }
}
