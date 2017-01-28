// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/InstrumentAbbreviation.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        InstrumentAbbreviation::InstrumentAbbreviation()
        :myValue()
        {}


        InstrumentAbbreviation::InstrumentAbbreviation( const XsString& value )
        :myValue( value )
        {}


        bool InstrumentAbbreviation::hasAttributes() const
        {
            return false;
        }


        bool InstrumentAbbreviation::hasContents() const
        {
            return true;
        }


        std::ostream& InstrumentAbbreviation::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& InstrumentAbbreviation::streamName( std::ostream& os ) const
        {
            os << "instrument-abbreviation";
            return os;
        }


        std::ostream& InstrumentAbbreviation::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        XsString InstrumentAbbreviation::getValue() const
        {
            return myValue;
        }


        void InstrumentAbbreviation::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool InstrumentAbbreviation::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.setValue( xelement.getValue() );
            return true;
        }

    }
}
