// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/MidiProgram.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        MidiProgram::MidiProgram()
        :myValue()
        {}


        MidiProgram::MidiProgram( const Midi128& value )
        :myValue( value )
        {}


        bool MidiProgram::hasAttributes() const
        {
            return false;
        }


        bool MidiProgram::hasContents() const
        {
            return true;
        }


        std::ostream& MidiProgram::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& MidiProgram::streamName( std::ostream& os ) const
        {
            os << "midi-program";
            return os;
        }


        std::ostream& MidiProgram::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        Midi128 MidiProgram::getValue() const
        {
            return myValue;
        }


        void MidiProgram::setValue( const Midi128& value )
        {
            myValue = value;
        }


        bool MidiProgram::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
