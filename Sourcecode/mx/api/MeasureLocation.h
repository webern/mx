// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/NoteData.h"

namespace mx
{
    namespace api
    {
        class ScoreData;
        
        class MeasureLocation
        {
        public:
            
            MeasureLocation();
            
            int partIndex;
            int staffIndex;
            int measureIndex;
            
            bool isValid( const ScoreData& scoreData ) const;
            
        };
    }
}
