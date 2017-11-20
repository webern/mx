// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/TuningAlter.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        TuningAlter::TuningAlter()
        :myValue()
        {}


        TuningAlter::TuningAlter( const Semitones& value )
        :myValue( value )
        {}


        bool TuningAlter::hasAttributes() const
        {
            return false;
        }


        bool TuningAlter::hasContents() const
        {
            return true;
        }


        std::ostream& TuningAlter::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& TuningAlter::streamName( std::ostream& os ) const
        {
            os << "tuning-alter";
            return os;
        }


        std::ostream& TuningAlter::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        Semitones TuningAlter::getValue() const
        {
            return myValue;
        }


        void TuningAlter::setValue( const Semitones& value )
        {
            myValue = value;
        }


        bool TuningAlter::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
