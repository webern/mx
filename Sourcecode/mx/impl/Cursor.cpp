// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/impl/Cursor.h"

namespace mx
{
    namespace impl
    {
        Cursor::Cursor( int numStaves, int globalTicksPerQuarter )
        : timeSignature{}
        , ticksPerQuarter( globalTicksPerQuarter )
        , tickTimePosition( 0 )
        , voiceIndex( 0 )
        , staffIndex( 0 )
        , isBackupInProgress( false )
        , isFirstMeasureInPart( true )
        , myNumStaves( numStaves )
        , myGlobalTicksPerQuarter( globalTicksPerQuarter )
        {
            
        }


        int Cursor::getNumStaves() const
        {
            return myNumStaves;
        }


        int Cursor::getGlobalTicksPerQuarter() const
        {
            return myGlobalTicksPerQuarter;
        }
        

        void Cursor::reset()
        {
            tickTimePosition = 0;
            staffIndex = 0;
            voiceIndex = 0;
        }
    }
}
