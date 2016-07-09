// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/BeatUnitPer.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/BeatUnitGroup.h"
#include "mx/core/elements/PerMinuteOrBeatUnitChoice.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        BeatUnitPer::BeatUnitPer()
        :myBeatUnitGroup( makeBeatUnitGroup() )
        ,myPerMinuteOrBeatUnitChoice( makePerMinuteOrBeatUnitChoice() )
        {}


        bool BeatUnitPer::hasAttributes() const
        {
            return false;
        }


        std::ostream& BeatUnitPer::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& BeatUnitPer::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool BeatUnitPer::hasContents() const
        {
            return true;
        }


        std::ostream& BeatUnitPer::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            myBeatUnitGroup->streamContents( os, indentLevel, isOneLineOnly );
            os << std::endl;
            myPerMinuteOrBeatUnitChoice->streamContents( os, indentLevel, isOneLineOnly );
            isOneLineOnly = false;
            return os;
        }


        BeatUnitGroupPtr BeatUnitPer::getBeatUnitGroup() const
        {
            return myBeatUnitGroup;
        }


        void BeatUnitPer::setBeatUnitGroup( const BeatUnitGroupPtr& value )
        {
            if ( value )
            {
                myBeatUnitGroup = value;
            }
        }


        PerMinuteOrBeatUnitChoicePtr BeatUnitPer::getPerMinuteOrBeatUnitChoice() const
        {
            return myPerMinuteOrBeatUnitChoice;
        }


        void BeatUnitPer::setPerMinuteOtBeatUnitChoice( const PerMinuteOrBeatUnitChoicePtr& value )
        {
            if ( value )
            {
                myPerMinuteOrBeatUnitChoice = value;
            }
        }


        bool BeatUnitPer::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                importGroup( message, it, endIter, isSuccess, myBeatUnitGroup );
                if( it->getName() == "beat-unit-per" || it->getName() == "note-relation-note" )
                {
                    isSuccess &= myPerMinuteOrBeatUnitChoice->fromXElement( message, *it );
                }
            }

            MX_RETURN_IS_SUCCESS;
        }
        
    }
}
