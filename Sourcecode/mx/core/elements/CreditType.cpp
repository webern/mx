// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/CreditType.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        CreditType::CreditType()
        :myValue()
        {}


        CreditType::CreditType( const XsString& value )
        :myValue( value )
        {}


        bool CreditType::hasAttributes() const
        {
            return false;
        }


        bool CreditType::hasContents() const
        {
            return true;
        }


        std::ostream& CreditType::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& CreditType::streamName( std::ostream& os ) const
        {
            os << "credit-type";
            return os;
        }


        std::ostream& CreditType::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        XsString CreditType::getValue() const
        {
            return myValue;
        }


        void CreditType::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool CreditType::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            myValue.setValue( xelement.getValue() );
            return true;
        }

    }
}
