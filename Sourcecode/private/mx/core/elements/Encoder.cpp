// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Encoder.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Encoder::Encoder()
        :myValue()
        ,myAttributes( std::make_shared<EncoderAttributes>() )
        {}


        Encoder::Encoder( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<EncoderAttributes>() )
        {}


        bool Encoder::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Encoder::hasContents() const
        {
            return true;
        }


        std::ostream& Encoder::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Encoder::streamName( std::ostream& os ) const
        {
            os << "encoder";
            return os;
        }


        std::ostream& Encoder::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        EncoderAttributesPtr Encoder::getAttributes() const
        {
            return myAttributes;
        }


        void Encoder::setAttributes( const EncoderAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString Encoder::getValue() const
        {
            return myValue;
        }


        void Encoder::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool Encoder::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            return isSuccess;
        }

    }
}
