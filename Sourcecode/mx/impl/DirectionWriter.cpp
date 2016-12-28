// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs


#include "mx/impl/DirectionWriter.h"
#include "mx/core/elements/Direction.h"
#include "mx/core/elements/DirectionType.h"
#include "mx/core/elements/EditorialVoiceDirectionGroup.h"
#include "mx/core/elements/Footnote.h"
#include "mx/core/elements/Level.h"
#include "mx/core/elements/Offset.h"
#include "mx/core/elements/Sound.h"
#include "mx/core/elements/Staff.h"
#include "mx/core/elements/Voice.h"
#include "mx/core/elements/AccordionRegistration.h"
#include "mx/core/elements/Bracket.h"
#include "mx/core/elements/Coda.h"
#include "mx/core/elements/Damp.h"
#include "mx/core/elements/DampAll.h"
#include "mx/core/elements/Dashes.h"
#include "mx/core/elements/Dynamics.h"
#include "mx/core/elements/Eyeglasses.h"
#include "mx/core/elements/HarpPedals.h"
#include "mx/core/elements/Image.h"
#include "mx/core/elements/Metronome.h"
#include "mx/core/elements/OctaveShift.h"
#include "mx/core/elements/OtherDirection.h"
#include "mx/core/elements/Pedal.h"
#include "mx/core/elements/Percussion.h"
#include "mx/core/elements/PrincipalVoice.h"
#include "mx/core/elements/Rehearsal.h"
#include "mx/core/elements/Scordatura.h"
#include "mx/core/elements/Segno.h"
#include "mx/core/elements/StringMute.h"
#include "mx/core/elements/Wedge.h"
#include "mx/core/elements/Words.h"
#include "mx/core/elements/Bracket.h"
#include "mx/api/BarlineData.h"
#include "mx/impl/LineFunctions.h"
#include "mx/impl/SpannerFunctions.h"
#include "mx/impl/MarkDataFunctions.h"
#include "mx/impl/DynamicsWriter.h"

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
            
            if( myDirectionData.isOffsetSpecified )
            {
                myOutDirectionPtr->setHasOffset( true );
                myOutDirectionPtr->getOffset()->setValue( core::DivisionsValue{ static_cast<core::DecimalType>( myDirectionData.offset ) } );
                if( myDirectionData.offsetSound != api::Bool::unspecified )
                {
                    myOutDirectionPtr->getOffset()->getAttributes()->hasSound = true;
                    myOutDirectionPtr->getOffset()->getAttributes()->sound = myConverter.convert( myDirectionData.offsetSound );
                }
            }
            
            for( const auto& mark : myDirectionData.marks )
            {
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
