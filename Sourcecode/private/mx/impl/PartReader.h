// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/PartData.h"
#include "mx/impl/MeasureCursor.h"

#include <mutex>
#include <memory>

namespace mx
{
	namespace core
	{
        class ScorePartwise;
		class PartwisePart;
        class ScorePart;
        using ScorePartPtr = std::shared_ptr<ScorePart>;
        class Properties;
        class DisplayTextOrAccidentalText;
        using DisplayTextOrAccidentalTextPtr = std::shared_ptr<DisplayTextOrAccidentalText>;
        using DisplayTextOrAccidentalTextSet = std::vector<DisplayTextOrAccidentalTextPtr>;
        class ScoreInstrument;
        class VirtualInstrument;
        class MidiDeviceInstrumentGroup;
        class MidiInstrument;
	}

    namespace impl
    {
    	class PartReader
    	{
    	public:
            PartReader( const core::ScorePart& inScorePart, const core::PartwisePart& inPartwisePartRef, int globalTicksPerMeasure, const core::ScorePartwise& inScore, int inDivisionsValue );

    		api::PartData getPartData();
            impl::MeasureCursor getCursor() const;

    	private:
    		const core::PartwisePart& myPartwisePart;
            const core::ScorePart& myScorePart;
            int myNumStaves;
            const int myGlobalTicksPerMeasure;
            const core::ScorePartwise& myScore;
            int myPartIndex;
            const int myConstructedDivisionsValue;
            MeasureCursor myCurrentCursor;
            MeasureCursor myPreviousCursor;
            
        private:
            mutable std::mutex myMutex;
            mutable api::PartData myOutPartData;
            
            int calculateNumStaves() const;
            void parseScorePart() const;
            std::string extractDisplayText( const core::DisplayTextOrAccidentalTextSet& items ) const;
            void parseScoreInstrument( const core::ScoreInstrument& scoreInstrument ) const;
            void parseVirtualInstrument( const core::VirtualInstrument& virtualInstrument ) const;
            void parseMidiDeviceInstrumentGroup( const core::MidiDeviceInstrumentGroup& grp ) const;
            void parseMidiInstrument( const core::MidiInstrument& inst ) const;
            int findPartIndex( const std::string& inPartId ) const;
            
            template<typename ELEMENT_TYPE>
            void updateNumStaves( const ELEMENT_TYPE& element, int& outNumStaves ) const
            {
                if( element.getHasStaff() )
                {
                    int temp = element.getStaff()->getValue().getValue();
                    if( temp > outNumStaves )
                    {
                        outNumStaves = temp;
                    }
                }
            }
    	};
    }
}
