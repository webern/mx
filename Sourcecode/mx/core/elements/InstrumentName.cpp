// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/InstrumentName.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        InstrumentName::InstrumentName()
        :myValue()
        {}


        InstrumentName::InstrumentName( const XsString& value )
        :myValue( value )
        {}


        bool InstrumentName::hasAttributes() const
        {
            return false;
        }


        bool InstrumentName::hasContents() const
        {
            return true;
        }


        std::ostream& InstrumentName::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& InstrumentName::streamName( std::ostream& os ) const
        {
            os << "instrument-name";
            return os;
        }


        std::ostream& InstrumentName::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        XsString InstrumentName::getValue() const
        {
            return myValue;
        }


        void InstrumentName::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool InstrumentName::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.setValue( xelement.getValue() );
            return true;
        }

    }
}
