// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/MeasureStyleChoice.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/BeatRepeat.h"
#include "mx/core/elements/MeasureRepeat.h"
#include "mx/core/elements/MultipleRest.h"
#include "mx/core/elements/Slash.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        MeasureStyleChoice::MeasureStyleChoice()


        :myChoice( MeasureStyleChoice::Choice::multipleRest )
        ,myMultipleRest( makeMultipleRest() )
        ,myMeasureRepeat( makeMeasureRepeat() )
        ,myBeatRepeat( makeBeatRepeat() )
        ,mySlash( makeSlash() )
        {}


        bool MeasureStyleChoice::hasAttributes() const
        {
            return false;
        }


        std::ostream& MeasureStyleChoice::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& MeasureStyleChoice::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool MeasureStyleChoice::hasContents() const
        {
            return true;
        }


        std::ostream& MeasureStyleChoice::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            isOneLineOnly = false;
            switch ( myChoice )
            {
                case Choice::multipleRest:
                    myMultipleRest->toStream( os, indentLevel+1 );
                    break;
                case Choice::measureRepeat:
                    myMeasureRepeat->toStream( os, indentLevel+1 );
                    break;
                case Choice::beatRepeat:
                    myBeatRepeat->toStream( os, indentLevel+1 );
                    break;
                case Choice::slash:
                    mySlash->toStream( os, indentLevel+1 );
                    break;
                default:
                    break;
            }
            return os;
        }


        MeasureStyleChoice::Choice MeasureStyleChoice::getChoice() const
        {
            return myChoice;
        }


        void MeasureStyleChoice::setChoice( const Choice value )
        {
            myChoice = value;
        }


        MultipleRestPtr MeasureStyleChoice::getMultipleRest() const
        {
            return myMultipleRest;
        }


        void MeasureStyleChoice::setMultipleRest( const MultipleRestPtr& value )
        {
            if ( value )
            {
                myMultipleRest = value;
            }
        }


        MeasureRepeatPtr MeasureStyleChoice::getMeasureRepeat() const
        {
            return myMeasureRepeat;
        }


        void MeasureStyleChoice::setMeasureRepeat( const MeasureRepeatPtr& value )
        {
            if ( value )
            {
                myMeasureRepeat = value;
            }
        }


        BeatRepeatPtr MeasureStyleChoice::getBeatRepeat() const
        {
            return myBeatRepeat;
        }


        void MeasureStyleChoice::setBeatRepeat( const BeatRepeatPtr& value )
        {
            if ( value )
            {
                myBeatRepeat = value;
            }
        }


        SlashPtr MeasureStyleChoice::getSlash() const
        {
            return mySlash;
        }


        void MeasureStyleChoice::setSlash( const SlashPtr& value )
        {
            if ( value )
            {
                mySlash = value;
            }
        }


        bool MeasureStyleChoice::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            MX_CHOICE_IF( multipleRest, "multiple-rest", MultipleRest );
            MX_CHOICE_IF( measureRepeat, "measure-repeat", MeasureRepeat );
            MX_CHOICE_IF( beatRepeat, "beat-repeat", BeatRepeat );
            MX_CHOICE_IF( slash, "slash", Slash );
            MX_BAD_ELEMENT_FAILURE( MeasureStyleChoice );
        }

    }
}
