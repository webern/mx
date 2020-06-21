// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Elision.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Elision::Elision()
        :myValue()
        ,myAttributes( std::make_shared<ElisionAttributes>() )
        {}


        Elision::Elision( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<ElisionAttributes>() )
        {}


        bool Elision::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Elision::hasContents() const
        {
            return true;
        }


        std::ostream& Elision::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Elision::streamName( std::ostream& os ) const
        {
            os << "elision";
            return os;
        }


        std::ostream& Elision::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        ElisionAttributesPtr Elision::getAttributes() const
        {
            return myAttributes;
        }


        void Elision::setAttributes( const ElisionAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString Elision::getValue() const
        {
            return myValue;
        }


        void Elision::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool Elision::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
