// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/CueNoteGroup.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Cue.h"
#include "mx/core/elements/Duration.h"
#include "mx/core/elements/FullNoteGroup.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        CueNoteGroup::CueNoteGroup()
        :myCue( makeCue() )
        ,myFullNoteGroup( makeFullNoteGroup() )
        ,myDuration( makeDuration() )
        {}


        bool CueNoteGroup::hasAttributes() const
        {
            return false;
        }


        std::ostream& CueNoteGroup::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& CueNoteGroup::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool CueNoteGroup::hasContents() const
        {
            return true;
        }


        std::ostream& CueNoteGroup::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            myCue->toStream( os, indentLevel );
            os << std::endl;
            myFullNoteGroup->streamContents( os, indentLevel, isOneLineOnly );
            os << std::endl;
            myDuration->toStream( os, indentLevel );
            isOneLineOnly = false;
            return os;
        }


        CuePtr CueNoteGroup::getCue() const
        {
            return myCue;
        }


        void CueNoteGroup::setCue( const CuePtr& value )
        {
            if ( value )
            {
                myCue = value;
            }
        }


        FullNoteGroupPtr CueNoteGroup::getFullNoteGroup() const
        {
            return myFullNoteGroup;
        }


        void CueNoteGroup::setFullNoteGroup( const FullNoteGroupPtr& value )
        {
            if ( value )
            {
                myFullNoteGroup = value;
            }
        }


        DurationPtr CueNoteGroup::getDuration() const
        {
            return myDuration;
        }


        void CueNoteGroup::setDuration( const DurationPtr& value )
        {
            if ( value )
            {
                myDuration = value;
            }
        }
        
        
        MX_FROM_XELEMENT_UNUSED( CueNoteGroup );
        
    }
}
