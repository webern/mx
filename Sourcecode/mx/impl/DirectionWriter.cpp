// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#ifdef FAIL

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

namespace mx
{
    namespace impl
    {
        DirectionWriter::DirectionWriter( const core::PartwiseMeasurePtr& inPartwiseMeasure )
        : myDirection{ nullptr }
        , myPartwiseMeasure{ inPartwiseMeasure }
        , myConverter{}
        {
            allocate();
        }
        
        api::Placement DirectionWriter::getPlacement() const
        {
            return getPlacement( *myDirection );
        }
        
        
        int DirectionWriter::getStaffIndex() const
        {
            return getStaffIndex( *myDirection );
        }
        
        
        int DirectionWriter::getOffset() const
        {
            return getOffset( *myDirection );
        }
        
        api::Placement DirectionWriter::getPlacement( const core::Direction& direction ) const
        {
            if( !direction.getAttributes()->hasPlacement )
            {
                return api::Placement::unspecified;
            }
            
            return myConverter.convert( direction.getAttributes()->placement );
        }
        
        
        int DirectionWriter::getStaffIndex( const core::Direction& direction ) const
        {
            if( direction.getHasStaff() )
            {
                return direction.getStaff()->getValue().getValue();
            }
            return -1;
        }
        
        
        int DirectionWriter::getOffset( const core::Direction& direction ) const
        {
            if( direction.getOffset() )
            {
                return static_cast<int>( direction.getOffset()->getValue().getValue() );
            }
            return -1;
        }
        
        
        bool DirectionWriter::isCompatible( const api::MarkData mark, int staffIndex ) const
        {
            if( getIsDirectionEmpty() )
            {
                return true;
            }
            else if( getStaffIndex() != staffIndex )
            {
                return false;
            }
        }
        
        
        bool DirectionWriter::getIsDirectionEmpty() const
        {
            return !( myDirection->getHasSound() || myDirection->getDirectionTypeSet().size() > 0 );
        }
        
        
        void DirectionWriter::allocate()
        {
            myDirection = core::makeDirection();
        }
        
        
    }
}
#endif
