// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/DisplayOctave.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        DisplayOctave::DisplayOctave()
        :myValue()
        {}


        DisplayOctave::DisplayOctave( const OctaveValue& value )
        :myValue( value )
        {}


        bool DisplayOctave::hasAttributes() const
        {
            return false;
        }


        bool DisplayOctave::hasContents() const
        {
            return true;
        }


        std::ostream& DisplayOctave::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& DisplayOctave::streamName( std::ostream& os ) const
        {
            os << "display-octave";
            return os;
        }


        std::ostream& DisplayOctave::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        OctaveValue DisplayOctave::getValue() const
        {
            return myValue;
        }


        void DisplayOctave::setValue( const OctaveValue& value )
        {
            myValue = value;
        }


        bool DisplayOctave::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
