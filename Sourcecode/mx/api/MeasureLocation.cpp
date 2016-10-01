// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

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
            
            if( static_cast<size_t>( staffIndex ) > part.staves.size() - 1 )
            {
                return false;
            }
            
            const auto& staff = part.staves.at( static_cast<size_t>( staffIndex ) );
            
            if( static_cast<size_t>( measureIndex ) > staff.measures.size() - 1 )
            {
                return false;
            }
            
            return true;
        }
    }
}
