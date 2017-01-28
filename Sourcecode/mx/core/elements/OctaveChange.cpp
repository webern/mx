// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/OctaveChange.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        OctaveChange::OctaveChange()
        :myValue()
        {}


        OctaveChange::OctaveChange( const Integer& value )
        :myValue( value )
        {}


        bool OctaveChange::hasAttributes() const
        {
            return false;
        }


        bool OctaveChange::hasContents() const
        {
            return true;
        }


        std::ostream& OctaveChange::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& OctaveChange::streamName( std::ostream& os ) const
        {
            os << "octave-change";
            return os;
        }


        std::ostream& OctaveChange::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        Integer OctaveChange::getValue() const
        {
            return myValue;
        }


        void OctaveChange::setValue( const Integer& value )
        {
            myValue = value;
        }


        bool OctaveChange::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
