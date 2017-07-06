// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/PerMinuteOrBeatUnitChoice.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/BeatUnitGroup.h"
#include "mx/core/elements/PerMinute.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        PerMinuteOrBeatUnitChoice::PerMinuteOrBeatUnitChoice()
        :myChoice( Choice::perMinute )
        ,myPerMinute( makePerMinute() )
        ,myBeatUnitGroup( makeBeatUnitGroup() )
        {}


        bool PerMinuteOrBeatUnitChoice::hasAttributes() const
        {
            return false;
        }


        std::ostream& PerMinuteOrBeatUnitChoice::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& PerMinuteOrBeatUnitChoice::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool PerMinuteOrBeatUnitChoice::hasContents() const
        {
            return true;
        }


        std::ostream& PerMinuteOrBeatUnitChoice::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            switch ( myChoice )
            {
                case Choice::perMinute:
                {
                    myPerMinute->toStream( os, indentLevel );
                }
                    break;
                case Choice::beatUnitGroup:
                {
                    myBeatUnitGroup->streamContents( os, indentLevel, isOneLineOnly );
                }
                    break;
                default:
                    break;
            }
            isOneLineOnly = false;
            return os;
        }


        PerMinuteOrBeatUnitChoice::Choice PerMinuteOrBeatUnitChoice::getChoice() const
        {
            return myChoice;
        }


        void PerMinuteOrBeatUnitChoice::setChoice( const PerMinuteOrBeatUnitChoice::Choice value )
        {
            myChoice = value;
        }


        PerMinutePtr PerMinuteOrBeatUnitChoice::getPerMinute() const
        {
            return myPerMinute;
        }


        void PerMinuteOrBeatUnitChoice::setPerMinute( const PerMinutePtr& value )
        {
            if ( value )
            {
                myPerMinute = value;
            }
        }


        BeatUnitGroupPtr PerMinuteOrBeatUnitChoice::getBeatUnitGroup() const
        {
            return myBeatUnitGroup;
        }


        void PerMinuteOrBeatUnitChoice::setBeatUnitGroup( const BeatUnitGroupPtr& value )
        {
            if ( value )
            {
                myBeatUnitGroup = value;
            }
        }


        bool PerMinuteOrBeatUnitChoice::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            MX_CHOICE_IF( perMinute, "per-minute", PerMinute );
            MX_CHOICE_IF( beatUnitGroup, "beat-unit-group", BeatUnitGroup );
            MX_BAD_ELEMENT_FAILURE( PerMinuteOrBeatUnitChoice );
        }

    }
}
