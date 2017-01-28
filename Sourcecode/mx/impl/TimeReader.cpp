// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/TimeReader.h"
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
#include "mx/core/elements/MusicDataChoice.h"
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
        TimeReader::TimeReader( const core::MusicDataChoiceSet& inMusicDataChoices )
        : myMusicDataChoiceSet{ inMusicDataChoices }
        , myIsTimeFound{ false }
        , myTimeSignatureData{}
        {
            myIsTimeFound = initialize();
        }
        
        
        bool TimeReader::getIsTimeFound() const
        {
            return myIsTimeFound;
        }
        
        
        mx::api::TimeSignatureData TimeReader::getTimeSignatureData() const
        {
            return myTimeSignatureData;
        }
        
        
        bool TimeReader::initialize()
        {
            for( const auto& mdc : myMusicDataChoiceSet )
            {
                if( mdc->getChoice() == core::MusicDataChoice::Choice::properties )
                {
                    const auto& props = *mdc->getProperties();
                    if( props.getTimeSet().size() > 0 )
                    {
                        myTime = props.getTimeSet().front();
                        return parseTime();
                    }
                }
            }
            return false;
        }
        
        
        
        bool TimeReader::parseTime()
        {
            const auto& timeChoice = *myTime->getTimeChoice();
            const auto& sigGroupSet = timeChoice.getTimeSignatureGroupSet();

            if( timeChoice.getChoice() == core::TimeChoice::Choice::timeSignature )
            {
                MX_ASSERT( sigGroupSet.size() > 0 );
                return parseTimeSignatureGroup( *sigGroupSet.front() );
            }
            else
            {
                return false;
                // MX_THROW( "TODO - other time signature stuff" );
            }
        }
        
        
        bool TimeReader::parseTimeSignatureGroup( const core::TimeSignatureGroup& timeSig )
        {
            const auto beatsStr = timeSig.getBeats()->getValue().getValue();
            int beats = myTimeSignatureData.beats;
            
            if( !utility::stringToInt( beatsStr, beats) )
            {
                return false;
            }
            
            const auto beatTypeStr = timeSig.getBeatType()->getValue().getValue();
            int beatType = myTimeSignatureData.beatType;
            
            if( !utility::stringToInt( beatTypeStr, beatType) )
            {
                return false;
            }
            
            myTimeSignatureData.beats = beats;
            myTimeSignatureData.beatType = beatType;
            
            if( myTime->getAttributes()->hasSymbol )
            {
                if( myTime->getAttributes()->symbol == core::TimeSymbol::common )
                {
                    myTimeSignatureData.symbol = api::TimeSignatureSymbol::common;
                }
                else if( myTime->getAttributes()->symbol == core::TimeSymbol::cut )
                {
                    myTimeSignatureData.symbol = api::TimeSignatureSymbol::cut;
                }
            }
            else
            {
                myTimeSignatureData.symbol = api::TimeSignatureSymbol::unspecified;
            }
            
            myTimeSignatureData.display = api::Bool::unspecified;
            if( myTime->getAttributes()->hasPrintObject )
            {
                bool isPrint = myTime->getAttributes()->printObject == core::YesNo::yes;
                
                if( isPrint )
                {
                    myTimeSignatureData.display = api::Bool::yes;
                }
                else
                {
                    myTimeSignatureData.display = api::Bool::no;
                }
            }
            
            myTimeSignatureData.isImplicit = false;
            
            return true;
        }
        
        
//        bool TimeReader::findAndFillTimeSignature( const core::MusicDataChoice& inMdc, api::TimeSignatureData& outTimeSignatureData ) const
//        {
//            if( ! ( inMdc.getChoice() == core::MusicDataChoice::Choice::properties ) )
//            {
//                return false;
//            }
//
//            const auto& prop = *inMdc.getProperties();
//            
//            if( prop.getTimeSet().size() > 0 )
//            {
//                const auto& time = **( prop.getTimeSet().cbegin() );
//                const auto& timeChoice = *time.getTimeChoice();
//                const auto& sigGroupSet = timeChoice.getTimeSignatureGroupSet();
//                
//                if( timeChoice.getChoice() == core::TimeChoice::Choice::timeSignature )
//                {
//                    if( sigGroupSet.size() > 0 )
//                    {
//                        const auto& sigGroup = **( sigGroupSet.cbegin() );
//                        const auto beatsStr = sigGroup.getBeats()->getValue().getValue();
//                        int beats = outTimeSignatureData.beats;
//                        
//                        if( !utility::stringToInt( beatsStr, beats) )
//                        {
//                            return false;
//                        }
//                        
//                        const auto beatTypeStr = sigGroup.getBeatType()->getValue().getValue();
//                        int beatType = outTimeSignatureData.beatType;
//                        
//                        if( !utility::stringToInt( beatTypeStr, beatType) )
//                        {
//                            return false;
//                        }
//                        
//                        outTimeSignatureData.beats = beats;
//                        outTimeSignatureData.beatType = beatType;
//                        
//                        if( time.getAttributes()->hasSymbol )
//                        {
//                            if( time.getAttributes()->symbol == core::TimeSymbol::common )
//                            {
//                                outTimeSignatureData.symbol = api::TimeSignatureSymbol::common;
//                            }
//                            else if( time.getAttributes()->symbol == core::TimeSymbol::cut )
//                            {
//                                outTimeSignatureData.symbol = api::TimeSignatureSymbol::cut;
//                            }
//                        }
//                        else
//                        {
//                            outTimeSignatureData.symbol = api::TimeSignatureSymbol::unspecified;
//                        }
//                        
//                        outTimeSignatureData.display = api::Bool::unspecified;
//                        if( time.getAttributes()->hasPrintObject )
//                        {
//                            bool isPrint = time.getAttributes()->printObject == core::YesNo::yes;
//
//                            if( isPrint )
//                            {
//                                outTimeSignatureData.display = api::Bool::yes;
//                            }
//                            else
//                            {
//                                outTimeSignatureData.display = api::Bool::no;
//                            }
//                        }
//                        
//                        return true;
//                    }
//                }
//            }
//            return false;
//        }
//
//
//        bool TimeReader::isTimeSignatureImplicit( const api::TimeSignatureData& previousTimeSignature, const api::TimeSignatureData& currentTimeSignature, const bool isFirstMeasureInPart ) const
//        {
//            if( isFirstMeasureInPart )
//            {
//                return false;
//            }
//            else
//            {
//                if(    ( !currentTimeSignature.isEqualTo( previousTimeSignature ) )
//                   ||  (  currentTimeSignature.display == api::Bool::yes ) )
//                    
//                {
//                    return false;
//                }
//                else
//                {
//                    return true;
//                }
//            }
//        }
        
        
//        int TimeReader::findMaxDivisionsPerQuarter( const core::ScorePartwise& inScorePartwise ) const
//        {
//            std::set<int> foundDivisions;
//            
//            for( const auto& pp : inScorePartwise.getPartwisePartSet() )
//            {
//                for( const auto& m : pp->getPartwiseMeasureSet() )
//                {
//                    for( const auto& mdc : m->getMusicDataGroup()->getMusicDataChoiceSet() )
//                    {
//                        if( mdc->getChoice() != core::MusicDataChoice::Choice::properties )
//                        {
//                            continue;
//                        }
//                        
//                        const auto& props = *mdc->getProperties();
//                        
//                        if( !props.getHasDivisions() )
//                        {
//                            continue;
//                        }
//                        
//                        const auto& divisions = *props.getDivisions();
//                        const auto tempDiv = divisions.getValue().getValue();
//                        const int tempDivInt = static_cast<int>( std::ceil( tempDiv - 0.5 ) );
//                        if( tempDivInt > 0 )
//                        {
//                            foundDivisions.insert( tempDivInt );
//                        }
//                    }
//                }
//            }
//                
//            return mx::impl::leastCommonMultiple( foundDivisions );
//        }
//        
//        
//        int TimeReader::convertDurationToGlobalTickScale( const impl::Cursor& cursor, const core::Duration& duration ) const
//        {
//            return convertDurationToGlobalTickScale( cursor, static_cast<long double>( duration.getValue().getValue() ) );
//        }
//        
//        
//        int TimeReader::convertDurationToGlobalTickScale( const impl::Cursor& cursor, long double durationValue ) const
//        {
//            if( cursor.ticksPerQuarter == cursor.getGlobalTicksPerQuarter() )
//            {
//                return static_cast<int>( std::ceil( durationValue - 0.5L ) );
//            }
//            
//            const long double currentTicksPerQuarter = static_cast<long double>( cursor.ticksPerQuarter );
//            const long double globalTicksPerQuarter = static_cast<long double>( cursor.getGlobalTicksPerQuarter() );
//            const long double convertedVal = durationValue * ( globalTicksPerQuarter / currentTicksPerQuarter );
//            return static_cast<int>( std::ceil( convertedVal - 0.5L ) );
//        }
    }
}
