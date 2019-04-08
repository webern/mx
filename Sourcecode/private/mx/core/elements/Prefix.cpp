// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Prefix.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Prefix::Prefix()
        :myValue()
        ,myAttributes( std::make_shared<PrefixAttributes>() )
        {}


        Prefix::Prefix( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<PrefixAttributes>() )
        {}


        bool Prefix::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Prefix::hasContents() const
        {
            return true;
        }


        std::ostream& Prefix::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Prefix::streamName( std::ostream& os ) const
        {
            os << "prefix";
            return os;
        }


        std::ostream& Prefix::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        PrefixAttributesPtr Prefix::getAttributes() const
        {
            return myAttributes;
        }


        void Prefix::setAttributes( const PrefixAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString Prefix::getValue() const
        {
            return myValue;
        }


        void Prefix::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool Prefix::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
