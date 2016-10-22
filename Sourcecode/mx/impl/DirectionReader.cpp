// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/impl/DirectionReader.h"
#include "mx/core/elements/AccordionRegistration.h"
#include "mx/core/elements/Bracket.h"
#include "mx/core/elements/Coda.h"
#include "mx/core/elements/Damp.h"
#include "mx/core/elements/DampAll.h"
#include "mx/core/elements/Dashes.h"
#include "mx/core/elements/Direction.h"
#include "mx/core/elements/DirectionType.h"
#include "mx/core/elements/Dynamics.h"
#include "mx/core/elements/EditorialVoiceDirectionGroup.h"
#include "mx/core/elements/Eyeglasses.h"
#include "mx/core/elements/Footnote.h"
#include "mx/core/elements/HarpPedals.h"
#include "mx/core/elements/Image.h"
#include "mx/core/elements/Level.h"
#include "mx/core/elements/Metronome.h"
#include "mx/core/elements/OctaveShift.h"
#include "mx/core/elements/Offset.h"
#include "mx/core/elements/OtherDirection.h"
#include "mx/core/elements/Pedal.h"
#include "mx/core/elements/Percussion.h"
#include "mx/core/elements/PrincipalVoice.h"
#include "mx/core/elements/Rehearsal.h"
#include "mx/core/elements/Scordatura.h"
#include "mx/core/elements/Segno.h"
#include "mx/core/elements/Sound.h"
#include "mx/core/elements/Staff.h"
#include "mx/core/elements/StringMute.h"
#include "mx/core/elements/Voice.h"
#include "mx/core/elements/Wedge.h"
#include "mx/core/elements/Words.h"
#include "mx/impl/MetronomeReader.h"
#include "mx/impl/PositionFunctions.h"
#include "mx/impl/PrintFunctions.h"
#include "mx/api/WedgeData.h"

namespace mx
{
    namespace impl
    {
        DirectionReader::DirectionReader( const core::Direction& inDirection, Cursor inCursor )
        : myDirection{ inDirection }
        , myCursor{ inCursor }
        , myConverter{}
        , myOutDirectionData{}
        {
            
        }

        
        api::DirectionData DirectionReader::getDirectionData()
        {
            myOutDirectionData = api::DirectionData{};
            myOutDirectionData.tickTimePosition = myCursor.tickTimePosition;
            myOutDirectionData.isStaffValueSpecified = !myDirection.getHasStaff();
           
            if( myDirection.getHasOffset() )
            {
                myOutDirectionData.isOffsetSpecified = true;
                myOutDirectionData.offset = static_cast<int>( std::ceil( myDirection.getOffset()->getValue().getValue() - 0.5 ) );
                if( myDirection.getOffset()->getAttributes()->hasSound )
                {
                    myOutDirectionData.offsetSound = myConverter.convert( myDirection.getOffset()->getAttributes()->sound );
                }
            }
            
            if( myDirection.getAttributes()->hasPlacement )
            {
                myOutDirectionData.placement = myConverter.convert( myDirection.getAttributes()->placement );
            }
            
            for ( const auto& dtPtr : myDirection.getDirectionTypeSet() )
            {
                const auto& dt = *dtPtr;
                parseDirectionType( dt );
            }
            
            api::DirectionData temp{ std::move( myOutDirectionData ) };
            myOutDirectionData = api::DirectionData{};
            return temp;
        }
        
        
        void DirectionReader::parseDirectionType( const core::DirectionType& directionType )
        {
            switch ( directionType.getChoice() )
            {
                case core::DirectionType::Choice::rehearsal:
                {
                    parseRehearsal( directionType );
                    break;
                }
                case core::DirectionType::Choice::segno:
                {
                    parseSegno( directionType );
                    break;
                }
                case core::DirectionType::Choice::words:
                {
                    parseWords( directionType );
                    break;
                }
                case core::DirectionType::Choice::coda:
                {
                    parseCoda( directionType );
                    break;
                }
                case core::DirectionType::Choice::wedge:
                {
                    parseWedge( directionType );
                    break;
                }
                case core::DirectionType::Choice::dynamics:
                {
                    parseDynamics( directionType );
                    break;
                }
                case core::DirectionType::Choice::dashes:
                {
                    parseDashes( directionType );
                    break;
                }
                case core::DirectionType::Choice::bracket:
                {
                    parseBracket( directionType );
                    break;
                }
                case core::DirectionType::Choice::pedal:
                {
                    parsePedal( directionType );
                    break;
                }
                case core::DirectionType::Choice::metronome:
                {
                    parseMetronome( directionType );
                    break;
                }
                case core::DirectionType::Choice::octaveShift:
                {
                    parseOctaveShift( directionType );
                    break;
                }
                case core::DirectionType::Choice::harpPedals:
                {
                    parseHarpPedals( directionType );
                    break;
                }
                case core::DirectionType::Choice::damp:
                {
                    parseDamp( directionType );
                    break;
                }
                case core::DirectionType::Choice::dampAll:
                {
                    parseRehearsal( directionType );
                    break;
                }
                case core::DirectionType::Choice::eyeglasses:
                {
                    parseEyeglasses( directionType );
                    break;
                }
                case core::DirectionType::Choice::stringMute:
                {
                    parseStringMute( directionType );
                    break;
                }
                case core::DirectionType::Choice::scordatura:
                {
                    parseScordatura( directionType );
                    break;
                }
                case core::DirectionType::Choice::image:
                {
                    parseImage( directionType );
                    break;
                }
                case core::DirectionType::Choice::principalVoice:
                {
                    parsePrincipalVoice( directionType );
                    break;
                }
                case core::DirectionType::Choice::accordionRegistration:
                {
                    parseAccordionRegistration( directionType );
                    break;
                }
                case core::DirectionType::Choice::percussion:
                {
                    parsePercussion( directionType );
                    break;
                }
                case core::DirectionType::Choice::otherDirection:
                {
                    parseOtherDirection( directionType );
                    break;
                }
                default: break;
            }
        }
       
        
        void DirectionReader::parseRehearsal( const core::DirectionType& directionType)
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseSegno( const core::DirectionType& directionType)
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseWords( const core::DirectionType& directionType)
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseCoda( const core::DirectionType& directionType)
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseWedge( const core::DirectionType& directionType)
        {
            const auto& wedge = *directionType.getWedge();
            const auto& attr = *wedge.getAttributes();
            const auto wedgeType = myConverter.convert( attr.type );
            const bool isSpreadSpecified = attr.hasSpread;
            const long double spread = attr.spread.getValue();
            
            if( attr.type == core::WedgeType::stop )
            {
                api::WedgeStop stop;
                if( attr.hasNumber )
                {
                    stop.numberLevel = attr.number.getValue();
                }
                if( isSpreadSpecified )
                {
                    stop.isSpreadSpecified = true;
                    stop.spread = spread;
                }
                myOutDirectionData.wedgeStops.emplace_back( std::move( stop ) );
                return;
            }
            else
            {
                api::WedgeStart start;
                if( attr.hasNumber )
                {
                    start.numberLevel = attr.number.getValue();
                }
                if( isSpreadSpecified )
                {
                    start.isSpreadSpecified = true;
                    start.spread = spread;
                }
                start.wedgeType = wedgeType;
                myOutDirectionData.wedgeStarts.emplace_back( std::move( start ) );
            }
        }
        
        
        void DirectionReader::parseDynamics( const core::DirectionType& directionType )
        {
            MX_UNUSED( directionType );
            for( const auto& dynamic : directionType.getDynamicsSet() )
            {
                parseDynamic( *dynamic );
            }
        }
        
        
        void DirectionReader::parseDynamic( const core::Dynamics& dynamic )
        {
            //const auto& attr = *dynamic.getAttributes();
            auto mark = api::DirectionMark{};
//            mark.tickTimePosition = myCursor.tickTimePosition;
//            mark.positionData = getPositionData( attr );
//            mark.printData = getPrintData( attr );
            const auto valueObject = dynamic.getValue();
            
//            if( myDirection.getAttributes()->hasPlacement )
//            {
//                if( markData.positionData.placement == api::Placement::unspecified )
//                {
//                    markData.positionData.placement = myConverter.convert( myDirection.getAttributes()->placement );
//                }
//            }
            
            mark.markType = myConverter.convertDynamic( valueObject.getValue() );
            mark.name = valueObject.getValueString();
            if( valueObject.getValue() == core::DynamicsEnum::otherDynamics )
            {
                auto codePoint = api::Smufl::findCodepoint( mark.name );
                if( codePoint > 0 )
                {
                    mark.smuflCodepoint = codePoint;
                    mark.smuflName = mark.name;
                }
            }
            else
            {
                mark.smuflName = api::MarkSmufl::getName( mark.markType, myOutDirectionData.placement );
                mark.smuflCodepoint = api::MarkSmufl::getCodepoint( mark.markType, myOutDirectionData.placement );
            }
            myOutDirectionData.marks.emplace_back( std::move( mark ) );
        }
        
        
        void DirectionReader::parseDashes( const core::DirectionType& directionType)
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseBracket( const core::DirectionType& directionType)
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parsePedal( const core::DirectionType& directionType)
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseMetronome( const core::DirectionType& directionType)
        {
            const auto& metronome = *directionType.getMetronome();
            MetronomeReader reader{ metronome };
            myOutDirectionData.tempos.emplace_back( reader.getTempoData() );
        }
        
        
        void DirectionReader::parseOctaveShift( const core::DirectionType& directionType)
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseHarpPedals( const core::DirectionType& directionType)
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseDamp( const core::DirectionType& directionType)
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseDampAll( const core::DirectionType& directionType)
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseEyeglasses( const core::DirectionType& directionType)
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseStringMute( const core::DirectionType& directionType)
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseScordatura( const core::DirectionType& directionType)
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseImage( const core::DirectionType& directionType)
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parsePrincipalVoice( const core::DirectionType& directionType)
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseAccordionRegistration( const core::DirectionType& directionType)
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parsePercussion( const core::DirectionType& directionType)
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseOtherDirection( const core::DirectionType& directionType)
        {
            MX_UNUSED( directionType );
        }
    }
}
