// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/NormalNotes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        NormalNotes::NormalNotes()
        :myValue()
        {}


        NormalNotes::NormalNotes( const NonNegativeInteger& value )
        :myValue( value )
        {}


        bool NormalNotes::hasAttributes() const
        {
            return false;
        }


        bool NormalNotes::hasContents() const
        {
            return true;
        }


        std::ostream& NormalNotes::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& NormalNotes::streamName( std::ostream& os ) const
        {
            os << "normal-notes";
            return os;
        }


        std::ostream& NormalNotes::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        NonNegativeInteger NormalNotes::getValue() const
        {
            return myValue;
        }


        void NormalNotes::setValue( const NonNegativeInteger& value )
        {
            myValue = value;
        }


        bool NormalNotes::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
