// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/String.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        String::String()
        :myAttributes( std::make_shared<StringAttributes>() )
        ,myValue()
        {

        }


        String::String( const StringNumber& value )
        :myAttributes( std::make_shared<StringAttributes>() )
        ,myValue( value )
        {
            
        }


        bool String::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& String::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& String::streamName( std::ostream& os ) const
        {
            os << "string";
            return os;
        }


        bool String::hasContents() const
        {
            return true;
        }


        std::ostream& String::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            core::toStream( os, myValue );
            return os;
        }


        StringAttributesPtr String::getAttributes() const
        {
            return myAttributes;
        }


        void String::setAttributes( const StringAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        StringNumber String::getValue() const
        {
            return myValue;
        }


        void String::setValue( const StringNumber& value )
        {
            myValue = value;
        }


        bool String::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.parse( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
