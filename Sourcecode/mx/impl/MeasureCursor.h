// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/impl/Cursor.h"

namespace mx
{
    namespace impl
    {
        class MeasureCursor : public Cursor
        {
        public:
            int partIndex;
            int measureIndex;

            MeasureCursor( int numStaves, int globalTicksPerQuarter )
            : Cursor{ numStaves, globalTicksPerQuarter }
            {
                
            }

        };        
    }
}
