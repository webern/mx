// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/MeasureData.h"
#include "mx/api/TimeSignatureData.h"
#include "mx/core/elements/PartwiseMeasure.h"

namespace mx
{
    namespace impl
    {
        class Cursor
        {
        public:

            api::TimeSignatureData timeSignature;
            int ticksPerQuarter;
            int tickTimePosition;
            int voiceIndex;
            int staffIndex;
            bool isBackupInProgress;
            bool isFirstMeasureInPart;

            Cursor( int numStaves, int globalTicksPerQuarter );
            virtual ~Cursor() = default;
            Cursor( const Cursor& ) = default;
            Cursor( Cursor&& ) = default;
            Cursor& operator=( const Cursor& ) = default;
            Cursor& operator=( Cursor&& ) = default;
            
            int getNumStaves() const;
            int getGlobalTicksPerQuarter() const;

			// use this to clear the state
			// when starting a new measure     
            void reset();
            
        private:
            int myNumStaves;
            int myGlobalTicksPerQuarter;
        };        
    }
}
