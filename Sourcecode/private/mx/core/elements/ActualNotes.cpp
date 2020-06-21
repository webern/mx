// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/ActualNotes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        ActualNotes::ActualNotes()
        :myValue()
        {}


        ActualNotes::ActualNotes( const NonNegativeInteger& value )
        :myValue( value )
        {}


        bool ActualNotes::hasAttributes() const
        {
            return false;
        }


        bool ActualNotes::hasContents() const
        {
            return true;
        }


        std::ostream& ActualNotes::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& ActualNotes::streamName( std::ostream& os ) const
        {
            os << "actual-notes";
            return os;
        }


        std::ostream& ActualNotes::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        NonNegativeInteger ActualNotes::getValue() const
        {
            return myValue;
        }


        void ActualNotes::setValue( const NonNegativeInteger& value )
        {
            myValue = value;
        }


        bool ActualNotes::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
