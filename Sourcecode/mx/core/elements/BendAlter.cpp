// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/BendAlter.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        BendAlter::BendAlter()
        :myValue()
        {}


        BendAlter::BendAlter( const Semitones& value )
        :myValue( value )
        {}


        bool BendAlter::hasAttributes() const
        {
            return false;
        }


        bool BendAlter::hasContents() const
        {
            return true;
        }


        std::ostream& BendAlter::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& BendAlter::streamName( std::ostream& os ) const
        {
            os << "bend-alter";
            return os;
        }


        std::ostream& BendAlter::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        Semitones BendAlter::getValue() const
        {
            return myValue;
        }


        void BendAlter::setValue( const Semitones& value )
        {
            myValue = value;
        }


        bool BendAlter::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
