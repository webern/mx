// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/HoleType.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        HoleType::HoleType()
        :myValue()
        {}


        HoleType::HoleType( const XsString& value )
        :myValue( value )
        {}


        bool HoleType::hasAttributes() const
        {
            return false;
        }


        bool HoleType::hasContents() const
        {
            return true;
        }


        std::ostream& HoleType::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& HoleType::streamName( std::ostream& os ) const
        {
            os << "hole-type";
            return os;
        }


        std::ostream& HoleType::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        XsString HoleType::getValue() const
        {
            return myValue;
        }


        void HoleType::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool HoleType::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.setValue( xelement.getValue() );
            return true;
        }

    }
}
