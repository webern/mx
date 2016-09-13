// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/api/MeasureData.h"
#include "mx/core/elements/PartwiseMeasure.h"
#include "mx/api/TimeSignatureData.h"
#include "mx/utility/Throw.h"

#include <map>

namespace mx
{
    namespace impl
    {
    	using StaffIndexMeasureMap = std::map<int, api::MeasureData>;
		
        
        struct MeasureCursor
        {
            api::TimeSignatureData timeSignature;
            int currentTicksPerQuarter;
            int currentPositionTicks;
            int currentVoiceIndex;
            int currentStaffIndex;
            int numStaves;
            int globalTicksPerQuarter;
            int userRequestedVoiceNumber;
            bool isBackupInProgress;
            
            MeasureCursor()
            : timeSignature{}
            , currentTicksPerQuarter( 48 )
            , currentPositionTicks( 0 )
            , currentVoiceIndex( 0 )
            , currentStaffIndex( 0 )
            , numStaves( 1 )
            , globalTicksPerQuarter( 48 )
            , userRequestedVoiceNumber( 1 )
            , isBackupInProgress( false )
            {
                
            }
            
            
            inline void reset()
            {
                currentPositionTicks = 0;
                currentStaffIndex = 0;
                currentVoiceIndex = 0;
            }
            
            
            int convertToGlobalTickScale( const int divisions )
            {
                if( currentTicksPerQuarter != globalTicksPerQuarter )
                {
                    // TODO - handle documents with multiple divisions values
                    MX_THROW( "a document with multiple divisions values is currently unsupported" );
                }
                return divisions;
            }
        };
        
        
        StaffIndexMeasureMap parseMeasure( const core::PartwiseMeasure& inMxMeasure, MeasureCursor& cursor, bool isFirstMeasure );
    }
}
