// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/impl/TimeFunctions.h"
#include "mx/core/elements/Beats.h"
#include "mx/core/elements/BeatType.h"
#include "mx/core/elements/Divisions.h"
#include "mx/core/elements/Duration.h"
#include "mx/core/elements/MusicDataChoice.h"
#include "mx/core/elements/MusicDataGroup.h"
#include "mx/core/elements/PartwiseMeasure.h"
#include "mx/core/elements/PartwisePart.h"
#include "mx/core/elements/Properties.h"
#include "mx/core/elements/Time.h"
#include "mx/core/elements/TimeChoice.h"
#include "mx/core/elements/TimeSignatureGroup.h"
#include "mx/impl/LcmGcd.h"
#include "mx/utility/StringToInt.h"
#include "mx/utility/Throw.h"

#include <string>
#include <cmath>
#include <set>

namespace mx
{
    namespace impl
    {
        bool TimeFunctions::findAndFillTimeSignature( const core::MusicDataChoice& inMdc, api::TimeSignatureData& outTimeSignatureData ) const
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
                        
                        if( !utility::stringToInt( beatsStr, beats) )
                        {
                            return false;
                        }
                        
                        const auto beatTypeStr = sigGroup.getBeatType()->getValue().getValue();
                        int beatType = outTimeSignatureData.beatType;
                        
                        if( !utility::stringToInt( beatTypeStr, beatType) )
                        {
                            return false;
                        }
                        
                        outTimeSignatureData.beats = beats;
                        outTimeSignatureData.beatType = beatType;
                        
                        if( time.getAttributes()->hasSymbol )
                        {
                            if( time.getAttributes()->symbol == core::TimeSymbol::common )
                            {
                                outTimeSignatureData.symbol = api::TimeSignatureSymbol::common;
                            }
                            else if( time.getAttributes()->symbol == core::TimeSymbol::cut )
                            {
                                outTimeSignatureData.symbol = api::TimeSignatureSymbol::cut;
                            }
                        }
                        else
                        {
                            outTimeSignatureData.symbol = api::TimeSignatureSymbol::unspecified;
                        }
                        
                        outTimeSignatureData.display = api::Bool::unspecified;
                        if( time.getAttributes()->hasPrintObject )
                        {
                            bool isPrint = time.getAttributes()->printObject == core::YesNo::yes;

                            if( isPrint )
                            {
                                outTimeSignatureData.display = api::Bool::yes;
                            }
                            else
                            {
                                outTimeSignatureData.display = api::Bool::no;
                            }
                        }
                        
                        return true;
                    }
                }
            }
            return false;
        }


        bool TimeFunctions::isTimeSignatureImplicit( const api::TimeSignatureData& previousTimeSignature, const api::TimeSignatureData& currentTimeSignature, const bool isFirstMeasureInPart ) const
        {
            if( isFirstMeasureInPart )
            {
                return false;
            }
            else
            {
                if(    ( !currentTimeSignature.isEqualTo( previousTimeSignature ) )
                   ||  (  currentTimeSignature.display == api::Bool::yes ) )
                    
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
        }
        
        
        int TimeFunctions::findMaxDivisionsPerQuarter( const core::ScorePartwise& inScorePartwise ) const
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
        
        
        int TimeFunctions::convertDurationToGlobalTickScale( const impl::Cursor& cursor, const core::Duration& duration ) const
        {
            return convertDurationToGlobalTickScale( cursor, static_cast<long double>( duration.getValue().getValue() ) );
        }
        
        
        int TimeFunctions::convertDurationToGlobalTickScale( const impl::Cursor& cursor, long double durationValue ) const
        {
            if( cursor.ticksPerQuarter == cursor.getGlobalTicksPerQuarter() )
            {
                return static_cast<int>( std::ceil( durationValue - 0.5L ) );
            }
            
            const long double currentTicksPerQuarter = static_cast<long double>( cursor.ticksPerQuarter );
            const long double globalTicksPerQuarter = static_cast<long double>( cursor.getGlobalTicksPerQuarter() );
            const long double convertedVal = durationValue * ( globalTicksPerQuarter / currentTicksPerQuarter );
            return static_cast<int>( std::ceil( convertedVal - 0.5L ) );
        }
    }
}
