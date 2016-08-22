// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/CreditWords.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        CreditWords::CreditWords()
        :myValue()
        ,myAttributes( std::make_shared<CreditWordsAttributes>() )
        {}


        CreditWords::CreditWords( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<CreditWordsAttributes>() )
        {}


        bool CreditWords::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool CreditWords::hasContents() const
        {
            return true;
        }


        std::ostream& CreditWords::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& CreditWords::streamName( std::ostream& os ) const
        {
            os << "credit-words";
            return os;
        }


        std::ostream& CreditWords::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        CreditWordsAttributesPtr CreditWords::getAttributes() const
        {
            return myAttributes;
        }


        void CreditWords::setAttributes( const CreditWordsAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString CreditWords::getValue() const
        {
            return myValue;
        }


        void CreditWords::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool CreditWords::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
