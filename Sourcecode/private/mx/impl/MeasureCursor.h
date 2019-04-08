// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

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
            , partIndex{ 0 }
            , measureIndex{ 0 }
            {
                
            }

            MeasureCursor()
            : Cursor{ -1, -1 }
            , partIndex{ -1 }
            , measureIndex{ -1 }
            {

            }

        };
    }
}
