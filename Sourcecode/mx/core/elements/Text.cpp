// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Text.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Text::Text()
        :myValue()
        ,myAttributes( std::make_shared<TextAttributes>() )
        {}


        Text::Text( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<TextAttributes>() )
        {}


        bool Text::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Text::hasContents() const
        {
            return true;
        }


        std::ostream& Text::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Text::streamName( std::ostream& os ) const
        {
            os << "text";
            return os;
        }


        std::ostream& Text::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        TextAttributesPtr Text::getAttributes() const
        {
            return myAttributes;
        }


        void Text::setAttributes( const TextAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString Text::getValue() const
        {
            return myValue;
        }


        void Text::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool Text::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
