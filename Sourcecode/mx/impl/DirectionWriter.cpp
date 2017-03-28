// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License


#include "mx/impl/DirectionWriter.h"
#include "mx/api/BarlineData.h"
#include "mx/core/elements/AccordionRegistration.h"
#include "mx/core/elements/BeatUnit.h"
#include "mx/core/elements/BeatUnitDot.h"
#include "mx/core/elements/BeatUnitGroup.h"
#include "mx/core/elements/BeatUnitPer.h"
#include "mx/core/elements/BeatUnitPerOrNoteRelationNoteChoice.h"
#include "mx/core/elements/Bracket.h"
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
#include "mx/core/elements/Pedal.h"
#include "mx/core/elements/Percussion.h"
#include "mx/core/elements/PerMinute.h"
#include "mx/core/elements/PerMinuteOrBeatUnitChoice.h"
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
#include "mx/impl/DynamicsWriter.h"
#include "mx/impl/LineFunctions.h"
#include "mx/impl/MarkDataFunctions.h"
#include "mx/impl/SpannerFunctions.h"

namespace mx
{
    namespace impl
    {
        DirectionWriter::DirectionWriter( const api::DirectionData& inDirectionData, const Cursor& inCursor )
        : myDirectionData{ inDirectionData }
        , myCursor{ inCursor }
        , myOutDirectionPtr{ nullptr }
        , myConverter{}
        , myPlacements{}
        {
            
        }
        
        core::DirectionPtr DirectionWriter::getDirection()
        {
            myOutDirectionPtr = core::makeDirection();
            myPlacements.clear();
            myIsFirstDirectionTypeAdded = false;
            auto& directionAttributes = *myOutDirectionPtr->getAttributes();
            
            if( myDirectionData.placement != api::Placement::unspecified )
            {
                directionAttributes.hasPlacement = true;
                directionAttributes.placement = myConverter.convert( myDirectionData.placement );
            }
            
            if( myDirectionData.isStaffValueSpecified || myCursor.staffIndex != 0 )
            {
                myOutDirectionPtr->setHasStaff( true );
                myOutDirectionPtr->getStaff()->setValue( core::StaffNumber{ myCursor.staffIndex + 1 } );
            }
            
            if( myDirectionData.tickTimePosition != myCursor.tickTimePosition )
            {
                auto offset = myDirectionData.tickTimePosition - myCursor.tickTimePosition;
                myOutDirectionPtr->setHasOffset( true );
                myOutDirectionPtr->getOffset()->setValue( core::DivisionsValue{ static_cast<core::DecimalType>( offset ) } );
                myOutDirectionPtr->getOffset()->getAttributes()->hasSound = true;
                myOutDirectionPtr->getOffset()->getAttributes()->sound = core::YesNo::yes;
            }
            
            for( auto mark : myDirectionData.marks )
            {
                mark.tickTimePosition = myDirectionData.tickTimePosition;
                
                // TODO - skip marks that aren't of the correct type (i.e. direction marks)
                // if !isDirection( mark ) continue;
                if( isMarkDynamic( mark.markType ) )
                {
                    auto directionTypePtr = core::makeDirectionType();
                    this->addDirectionType( directionTypePtr );
                    directionTypePtr->setChoice( core::DirectionType::Choice::dynamics );
                    DynamicsWriter dynamicsWriter{ mark, myCursor };
                    MX_ASSERT( directionTypePtr->getDynamicsSet().size() == 1 );
                    directionTypePtr->addDynamics( dynamicsWriter.getDynamics() );
                    directionTypePtr->removeDynamics( directionTypePtr->getDynamicsSet().cbegin() );
                }

                if( isMarkPedal( mark.markType ) )
                {
                    auto directionTypePtr = core::makeDirectionType();
                    this->addDirectionType( directionTypePtr );
                    directionTypePtr->setChoice( core::DirectionType::Choice::pedal );
                    auto pedalPtr = directionTypePtr->getPedal();
                    auto attr = pedalPtr->getAttributes();

                    if( mark.positionData.placement != api::Placement::unspecified )
                    {
                        myOutDirectionPtr->getAttributes()->hasPlacement = true;
                        Converter c;
                        myOutDirectionPtr->getAttributes()->placement = c.convert( mark.positionData.placement );
                    }

                    if( mark.markType == api::MarkType::pedal )
                    {
                        attr->type = core::StartStopChangeContinue::start;
                    }
                    else if ( mark.markType == api::MarkType::damp )
                    {
                        attr->type = core::StartStopChangeContinue::stop;
                    }

                    attr->hasLine = true;
                    attr->line = core::YesNo::no;
                    attr->hasSign = true;
                    attr->sign = core::YesNo::yes;
                    setAttributesFromPositionData( mark.positionData, *attr );
                }
            }
            
            for( const auto& wedgeStart : myDirectionData.wedgeStarts )
            {
                auto wedgeStartDirectionTypePtr = core::makeDirectionType();
                wedgeStartDirectionTypePtr->setChoice( core::DirectionType::Choice::wedge );
                this->addDirectionType( wedgeStartDirectionTypePtr );
                auto wedgePtr = wedgeStartDirectionTypePtr->getWedge();
                auto& attr = *wedgePtr->getAttributes();

                if( wedgeStart.wedgeType != api::WedgeType::unspecified )
                {
                    attr.type = myConverter.convert( wedgeStart.wedgeType );
                }
                
                if( wedgeStart.isSpreadSpecified )
                {
                    attr.hasSpread = true;
                    attr.spread = core::DivisionsValue{ static_cast<core::DecimalType>( wedgeStart.spread ) };
                }
                
                auto& attributes = *wedgePtr->getAttributes();
                
                setAttributesFromPositionData( wedgeStart.positionData, attributes );
                setAttributesFromLineData(wedgeStart.lineData, attributes);
                setAttributesFromColorData(wedgeStart.colorData, attributes);
            }
            
            for( const auto& wedgeStop : myDirectionData.wedgeStops )
            {
                auto wedgeStartDirectionTypePtr = core::makeDirectionType();
                wedgeStartDirectionTypePtr->setChoice( core::DirectionType::Choice::wedge );
                this->addDirectionType( wedgeStartDirectionTypePtr );
                auto wedgePtr = wedgeStartDirectionTypePtr->getWedge();
                auto& attr = *wedgePtr->getAttributes();
                attr.type = core::WedgeType::stop;
                
                if( wedgeStop.isSpreadSpecified )
                {
                    attr.hasSpread = true;
                    attr.spread = core::DivisionsValue{ static_cast<core::DecimalType>( wedgeStop.spread ) };
                }
                auto& attributes = *wedgePtr->getAttributes();
                setAttributesFromPositionData( wedgeStop.positionData, attributes );
            }
            
            for( const auto& ottavaStart : myDirectionData.ottavaStarts )
            {
                auto oStartDir = core::makeDirectionType();
                this->addDirectionType( oStartDir );
                oStartDir->setChoice( core::DirectionType::Choice::octaveShift );
                auto oStart = oStartDir->getOctaveShift();
                auto& attr = *oStart->getAttributes();
                impl::setAttributesFromLineData( ottavaStart.spannerStart.lineData, attr );
                
                attr.hasSize = true;
                
                switch( ottavaStart.ottavaType )
                {
                    case api::OttavaType::o15ma:
                    {
                        attr.type = core::UpDownStopContinue::up;
                        attr.size = core::PositiveInteger{ 15 };
                        break;
                    }
                    case api::OttavaType::o15mb:
                    {
                        attr.type = core::UpDownStopContinue::down;
                        attr.size = core::PositiveInteger{ 15 };
                        break;
                    }
                    case api::OttavaType::o8va:
                    {
                        attr.type = core::UpDownStopContinue::up;
                        attr.size = core::PositiveInteger{ 8 };
                        break;
                    }
                    case api::OttavaType::o8vb:
                    {
                        attr.type = core::UpDownStopContinue::down;
                        attr.size = core::PositiveInteger{ 8 };
                        break;
                    }
                    default: break;
                }
            }
            
            for( const auto& ottavaStop : myDirectionData.ottavaStops )
            {
                auto oStopDir = core::makeDirectionType();
                this->addDirectionType( oStopDir );
                oStopDir->setChoice( core::DirectionType::Choice::octaveShift );
                auto oStart = oStopDir->getOctaveShift();
                auto& attr = *oStart->getAttributes();
                attr.type = core::UpDownStopContinue::stop;
                MX_UNUSED( ottavaStop );
            }
            
            for( const auto& item : myDirectionData.bracketStarts )
            {
                auto outDirType = core::makeDirectionType();
                this->addDirectionType( outDirType );
                outDirType->setChoice( core::DirectionType::Choice::bracket );
                auto outElement = outDirType->getBracket();
                auto& attr = *outElement->getAttributes();
                setAttributesFromSpannerStart( item, attr );
            }

            for( const auto& tempo : myDirectionData.tempos )
            {
                if( tempo.tempoType != api::TempoType::beatsPerMinute )
                {
                    MX_THROW( "Only api::TempoType::beatsPerMinute is supported, others are not implemented" );
                }
                
                auto outDirType = core::makeDirectionType();
                this->addDirectionType( outDirType );
                outDirType->setChoice( core::DirectionType::Choice::metronome );
                auto outElement = outDirType->getMetronome();
                auto choice = outElement->getBeatUnitPerOrNoteRelationNoteChoice();
                choice->setChoice( core::BeatUnitPerOrNoteRelationNoteChoice::Choice::beatUnitPer );
                auto bpm = choice->getBeatUnitPer();
                auto beatUnitGroup = bpm->getBeatUnitGroup();
                auto beatUnit = beatUnitGroup->getBeatUnit();
                Converter converter;
                beatUnit->setValue(converter.convert(tempo.beatsPerMinute.durationName));
                
                for( int d = 0; d < tempo.beatsPerMinute.dots; ++d )
                {
                    beatUnitGroup->addBeatUnitDot(core::makeBeatUnitDot());
                }
                
                auto pmobuc = bpm->getPerMinuteOrBeatUnitChoice();
                pmobuc->setChoice( core::PerMinuteOrBeatUnitChoice::Choice::perMinute );
                auto pm = pmobuc->getPerMinute();
                auto str = std::to_string( tempo.beatsPerMinute.beatsPerMinute );
                pm->setValue( core::XsString{ str } );
                //auto& attr = *outElement->getAttributes();
                //setAttributesFromSpannerStart( item, attr );
            }

            if( myDirectionData.words.size() > 0 )
            {
                bool isFirstWordsAdded = false;
                auto outDirType = core::makeDirectionType();
                outDirType->setChoice( core::DirectionType::Choice::words );

                for( const auto& wordsData : myDirectionData.words )
                {
                    auto outWords = core::makeWords();
                    outDirType->addWords( outWords );

                    if (!isFirstWordsAdded)
                    {
                        isFirstWordsAdded = true;
                        outDirType->removeWords( outDirType->getWordsSet().cbegin() );
                    }

                    auto& attr = *(outWords->getAttributes());
                    outWords->setValue( core::XsString{ wordsData.text } );
                    setAttributesFromPositionData( wordsData.positionData, attr );
                    setAttributesFromFontData( wordsData.fontData, attr );
                    setAttributesFromColorData( wordsData.colorData, attr );
                }

                if( isFirstWordsAdded )
                {
                    addDirectionType( outDirType );
                }
            }

            myPlacements.clear();
            myIsFirstDirectionTypeAdded = false;
            return myOutDirectionPtr;
        }
        
        
        void DirectionWriter::addDirectionType( const core::DirectionTypePtr& directionType )
        {
            myOutDirectionPtr->addDirectionType( directionType );

            if( !myIsFirstDirectionTypeAdded && myOutDirectionPtr->getDirectionTypeSet().size() == 2 )
            {
                myOutDirectionPtr->removeDirectionType( myOutDirectionPtr->getDirectionTypeSet().cbegin() );
            }
            
            myIsFirstDirectionTypeAdded = true;
        }
    }
}
