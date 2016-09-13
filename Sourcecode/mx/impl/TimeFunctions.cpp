// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/impl/TimeFunctions.h"
#include "mx/utility/Throw.h"
#include "mx/core/elements/MusicDataGroup.h"
#include "mx/core/elements/MusicDataChoice.h"
#include "mx/core/elements/Properties.h"
#include "mx/core/elements/Time.h"
#include "mx/core/elements/TimeChoice.h"
#include "mx/core/elements/TimeSignatureGroup.h"
#include "mx/core/elements/Beats.h"
#include "mx/core/elements/BeatType.h"
#include "mx/core/elements/PartwisePart.h"
#include "mx/core/elements/PartwiseMeasure.h"
#include "mx/core/elements/Divisions.h"
#include "mx/impl/MxMath.h"

#include <string>
#include <cmath>
#include <set>

namespace mx
{
    namespace impl
    {
        bool findAndFillTimeSignature( const core::MusicDataChoice& inMdc, api::TimeSignatureData& outTimeSignatureData )
        {
            if( ! ( inMdc.getChoice() == core::MusicDataChoice::Choice::properties ) )
            {
                return false;
            }

            const auto& prop = *inMdc.getProperties();
                    
            if( prop.getTimeSet().size() > 0 )
            {
                const auto& time = **( prop.getTimeSet().cbegin() );
                const auto& timeChoice = *time.getTimeChoice();
                const auto& sigGroupSet = timeChoice.getTimeSignatureGroupSet();
                
                if( timeChoice.getChoice() == core::TimeChoice::Choice::timeSignature )
                {
                    if( sigGroupSet.size() > 0 )
                    {
                        const auto& sigGroup = **( sigGroupSet.cbegin() );
                        const auto beatsStr = sigGroup.getBeats()->getValue().getValue();
                        int beats = outTimeSignatureData.beats;
                        try { beats = std::stoi( beatsStr ); } catch ( ... ) { return false; }
                        const auto beatTypeStr = sigGroup.getBeatType()->getValue().getValue();
                        int beatType = outTimeSignatureData.beatType;
                        try { beatType = std::stoi( beatTypeStr ); } catch ( ... ) { return false; }
                        
                        outTimeSignatureData.beats = beats;
                        outTimeSignatureData.beatType = beatType;
                        
                        if( time.getAttributes()->hasSymbol )
                        {
                            if( time.getAttributes()->symbol == core::TimeSymbol::common )
                            {
                                outTimeSignatureData.symbol == api::TimeSignatureSymbol::common;
                            }
                            else if( time.getAttributes()->symbol == core::TimeSymbol::cut )
                            {
                                outTimeSignatureData.symbol == api::TimeSignatureSymbol::cut;
                            }
                        }
                        
                        outTimeSignatureData.isPrintObjectYes = false;
                        if( time.getAttributes()->hasPrintObject )
                        {
                            outTimeSignatureData.isPrintObjectYes = ( time.getAttributes()->printObject == core::YesNo::yes );
                        }
                        
                        return true;
                    }
                }
            }
            return false;
        }
        
        int findMaxDivisionsPerQuarter( const core::ScorePartwise& inScorePartwise )
        {
            std::set<int> foundDivisions;
            
            for( const auto& pp : inScorePartwise.getPartwisePartSet() )
            {
                for( const auto& m : pp->getPartwiseMeasureSet() )
                {
                    for( const auto& mdc : m->getMusicDataGroup()->getMusicDataChoiceSet() )
                    {
                        if( mdc->getChoice() != core::MusicDataChoice::Choice::properties )
                        {
                            continue;
                        }
                        
                        const auto& props = *mdc->getProperties();
                        
                        if( !props.getHasDivisions() )
                        {
                            continue;
                        }
                        
                        const auto& divisions = *props.getDivisions();
                        const auto tempDiv = divisions.getValue().getValue();
                        const int tempDivInt = static_cast<int>( std::ceil( tempDiv - 0.5 ) );
                        if( tempDivInt > 0 )
                        {
                            foundDivisions.insert( tempDivInt );
                        }
                    }
                }
            }
                
            return mx::impl::leastCommonMultiple( foundDivisions );
        }
    }
}
