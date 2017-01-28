// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/TuningOctave.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        TuningOctave::TuningOctave()
        :myValue()
        {}


        TuningOctave::TuningOctave( const OctaveValue& value )
        :myValue( value )
        {}


        bool TuningOctave::hasAttributes() const
        {
            return false;
        }


        bool TuningOctave::hasContents() const
        {
            return true;
        }


        std::ostream& TuningOctave::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& TuningOctave::streamName( std::ostream& os ) const
        {
            os << "tuning-octave";
            return os;
        }


        std::ostream& TuningOctave::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        OctaveValue TuningOctave::getValue() const
        {
            return myValue;
        }


        void TuningOctave::setValue( const OctaveValue& value )
        {
            myValue = value;
        }


        bool TuningOctave::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
