// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Stem.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Stem::Stem()
        :myValue( StemValue::none )
        ,myAttributes( std::make_shared<StemAttributes>() )
        {}


        Stem::Stem( const StemValue& value )
        :myValue( value )
        ,myAttributes( std::make_shared<StemAttributes>() )
        {}


        bool Stem::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Stem::hasContents() const
        {
            return true;
        }


        std::ostream& Stem::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Stem::streamName( std::ostream& os ) const
        {
            os << "stem";
            return os;
        }


        std::ostream& Stem::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        StemAttributesPtr Stem::getAttributes() const
        {
            return myAttributes;
        }


        void Stem::setAttributes( const StemAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        StemValue Stem::getValue() const
        {
            return myValue;
        }


        void Stem::setValue( const StemValue& value )
        {
            myValue = value;
        }


        bool Stem::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue = parseStemValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
