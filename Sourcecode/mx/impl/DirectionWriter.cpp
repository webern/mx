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
#include "mx/api/BarlineData.h"

namespace mx
{
    namespace impl
    {
        DirectionWriter::DirectionWriter( const api::DirectionData& inDirectionData, const Cursor& inCursor )
        : myDirectionData{ inDirectionData }
        , myCursor{ inCursor }
        , myOutDirectionPtr{ nullptr }
        , myConverter{}
        {
            
        }
        
        core::DirectionPtr DirectionWriter::getDirection()
        {
            myOutDirectionPtr = core::makeDirection();
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
                    MX_ASSERT( directionTypePtr->getDynamicsSet().size() == 1 );
                    auto dynamics = directionTypePtr->getDynamicsSet().front();
                    core::DynamicsValue dynamicsValue;
                    dynamicsValue.setValue( myConverter.convertDynamic( mark.markType ) );
                    dynamics->setValue( dynamicsValue );
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
            }
            
            //core::DirectionPtr temp{ std::move( myOutDirectionPtr ) };
            myIsFirstDirectionTypeAdded = false;
            //myOutDirectionPtr = nullptr;
            return myOutDirectionPtr;//temp;
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
