// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

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
#include "mx/impl/PrintFunctions.h"
#include "mx/api/WedgeData.h"
#include "mx/impl/SpannerFunctions.h"
#include "mx/impl/DynamicsReader.h"
#include "mx/impl/MarkDataFunctions.h"
#include "mx/core/elements/Harmony.h"

namespace mx
{
    namespace impl
    {
        DirectionReader::DirectionReader( std::shared_ptr<const core::Direction> inDirection, Cursor inCursor )
        : myDirection{ inDirection }
        , myHarmony{ nullptr }
        , myCursor{ inCursor }
        , myConverter{}
        , myOutDirectionData{}
        {
            
        }


        DirectionReader::DirectionReader( std::shared_ptr<const core::Harmony> inHarmony, Cursor inCursor )
        : myDirection{ nullptr }
        , myHarmony{ inHarmony }
        , myCursor{ inCursor }
        , myConverter{}
        , myOutDirectionData{}
        {

        }


        api::DirectionData DirectionReader::getDirectionData()
        {
            myOutDirectionData = initializeData();
            updateTimeForOffset();
            parsePlacement();
            parseValues();

            // need to make sure all the MarkData item times match the DirectionData time
            fixTimes();
            return returnData();
        }


        mx::api::DirectionData DirectionReader::initializeData()
        {
            auto result = api::DirectionData{};
            result.tickTimePosition = myCursor.tickTimePosition;

            if( myDirection )
            {
                result.isStaffValueSpecified = myDirection->getHasStaff();
            }
            else if ( myHarmony )
            {
                result.isStaffValueSpecified = myHarmony->getHasStaff();
            }

            return result;
        }


        void DirectionReader::updateTimeForOffset()
        {
            if( myDirection )
            {
                if( myDirection->getHasOffset() )
                {
                    const auto offset = static_cast<int>( std::ceil( myDirection->getOffset()->getValue().getValue() - 0.5 ) );
                    myOutDirectionData.tickTimePosition += offset;
                }
            }
            else if( myHarmony )
            {
                if( myHarmony->getHasOffset() )
                {
                    const auto offset = static_cast<int>( std::ceil( myHarmony->getOffset()->getValue().getValue() - 0.5 ) );
                    myOutDirectionData.tickTimePosition += offset;
                }
            }
        }


        void DirectionReader::parsePlacement()
        {
            if( myDirection )
            {
                if( myDirection->getAttributes()->hasPlacement )
                {
                    myOutDirectionData.placement = myConverter.convert( myDirection->getAttributes()->placement );
                }
            }
            else if( myHarmony )
            {
                if( myHarmony->getAttributes()->hasPlacement )
                {
                    myOutDirectionData.placement = myConverter.convert( myHarmony->getAttributes()->placement );
                }
            }
        }


        void DirectionReader::parseValues()
        {
            if( myDirection )
            {
                for ( const auto& dtPtr : myDirection->getDirectionTypeSet() )
                {
                    const auto& dt = *dtPtr;
                    parseDirectionType( dt );
                }
            }
            else if( myHarmony )
            {
                MX_THROW("not implemented");
            }
        }


        void DirectionReader::fixTimes()
        {
            for( auto& mark : myOutDirectionData.marks )
            {
                mark.tickTimePosition = myOutDirectionData.tickTimePosition;
            }
        }


        mx::api::DirectionData DirectionReader::returnData()
        {
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
            const auto& wordSet = directionType.getWordsSet();

            for( const auto& wordPtr : wordSet )
            {
                api::WordsData outWords;
                const auto& attr = *wordPtr->getAttributes();
                outWords.text = wordPtr->getValue().getValue();
                outWords.positionData = getPositionData( attr );
                outWords.colorData = getColor( attr );
                outWords.fontData = getFontData( attr );
                myOutDirectionData.words.emplace_back( std::move( outWords ) );
            }
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
            auto positionData = getPositionData( attr );
            auto lineData = getLineData( attr );
            auto colorData = getColor( attr );
            
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
                stop.positionData = positionData;
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
                start.positionData = positionData;
                start.lineData = lineData;
                start.colorData = colorData;
                myOutDirectionData.wedgeStarts.emplace_back( std::move( start ) );
            }
        }
        
        
        void DirectionReader::parseDynamics( const core::DirectionType& directionType )
        {
            for( const auto& dynamic : directionType.getDynamicsSet() )
            {
                DynamicsReader reader{ *dynamic, myCursor };
                reader.parseDynamics( myOutDirectionData.marks );
                //parseDynamic( *dynamic );
            }
        }
        
        
        void DirectionReader::parseDynamic( const core::Dynamics& dynamic )
        {
            auto mark = api::MarkData{};
            const auto valueObject = dynamic.getValue();
            mark.markType = myConverter.convertDynamic( valueObject.getValue() );
            mark.name = valueObject.getValueString();
            impl::parseMarkDataAttributes( *dynamic.getAttributes(), mark );
            
            auto placement = mark.positionData.placement;
            
            if( placement == api::Placement::unspecified )
            {
                placement = myOutDirectionData.placement;
            }
            
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
                mark.smuflName = api::MarkSmufl::getName( mark.markType, placement );
                mark.smuflCodepoint = api::MarkSmufl::getCodepoint( mark.markType, placement );
            }
            myOutDirectionData.marks.emplace_back( std::move( mark ) );
        }
        
        
        void DirectionReader::parseDashes( const core::DirectionType& directionType)
        {
            MX_UNUSED( directionType );
        }
        
        
        void DirectionReader::parseBracket( const core::DirectionType& directionType)
        {
            const auto& bracket = *directionType.getBracket();
            const auto& attr = *bracket.getAttributes();
            
            if( attr.type == core::StartStopContinue::stop )
            {
                api::SpannerStop stop;
                stop.tickTimePosition = myCursor.tickTimePosition;
                stop.numberLevel = impl::checkNumber( &attr );
                stop.positionData = this->parsePositionData( attr );
                myOutDirectionData.bracketStops.emplace_back( std::move( stop ) );
                return;
            }
            else if( attr.type == core::StartStopContinue::start )
            {
                api::SpannerStart start;
                start.tickTimePosition = myCursor.tickTimePosition;
                start.numberLevel = impl::checkNumber( &attr );
                start.positionData = this->parsePositionData( attr );
                start.lineData = impl::getLineData( attr );
                start.printData = impl::getPrintData( attr );
                myOutDirectionData.bracketStarts.emplace_back( std::move( start ) );
                return;
            }
        }
        
        
        void DirectionReader::parsePedal( const core::DirectionType& directionType)
        {
            const auto& pedalPtr = *directionType.getPedal();
            const auto& attr = *pedalPtr.getAttributes();

            if( attr.type != core::StartStopChangeContinue::start &&
                attr.type != core::StartStopChangeContinue::stop )
            {
                return;
            }

            auto pedalType = api::MarkType::pedal;

            if( attr.type == core::StartStopChangeContinue::stop )
            {
                pedalType = api::MarkType::damp;
            }

            const auto placement =
                myDirection->getAttributes()->hasPlacement ?
                    ( myDirection->getAttributes()->placement == core::AboveBelow::above ?
                      api::Placement::above :
                      api::Placement::below )
                : api::Placement::unspecified;

            auto mark = api::MarkData{ placement, pedalType };
            mark.positionData = getPositionData( attr );
            mark.positionData.placement = placement;
            myOutDirectionData.placement = placement;
            myOutDirectionData.marks.emplace_back( std::move( mark ) );
        }

        
        void DirectionReader::parseMetronome( const core::DirectionType& directionType)
        {
            const auto& metronome = *directionType.getMetronome();
            MetronomeReader reader{ metronome };
            myOutDirectionData.tempos.emplace_back( reader.getTempoData() );
        }
        
        
        void DirectionReader::parseOctaveShift( const core::DirectionType& directionType)
        {
            const auto& octaveShift = *directionType.getOctaveShift();
            auto& attr = *octaveShift.getAttributes();
            
            if( attr.type == core::UpDownStopContinue::continue_ )
            {
                // not supported
                return;
            }
            
            bool isStop = attr.type == core::UpDownStopContinue::stop;
            if( isStop )
            {
                auto stop = impl::getSpannerStop( attr );
                stop.tickTimePosition = myCursor.tickTimePosition;
                myOutDirectionData.ottavaStops.emplace_back( std::move( stop ) );
                return;
            }
            
            auto ottavaType = api::OttavaType::unspecified;
            int amount = 8;
            
            if( attr.hasSize )
            {
                amount = attr.size.getValue();
            }
            
            bool isUp = attr.type == core::UpDownStopContinue::up;
            
            if( isUp && amount > 8 )
            {
                ottavaType = api::OttavaType::o15ma;
            }
            else if ( isUp )
            {
                ottavaType = api::OttavaType::o8va;
            }
            else if ( !isUp && amount > 8 )
            {
                ottavaType = api::OttavaType::o15mb;
            }
            else if ( !isUp )
            {
                ottavaType = api::OttavaType::o8vb;
            }

            api::OttavaStart start;
            start.spannerStart = impl::getSpannerStart( attr );
            start.ottavaType = ottavaType;
            start.spannerStart.tickTimePosition = myCursor.tickTimePosition;
            myOutDirectionData.ottavaStarts.emplace_back( std::move( start ) );
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
