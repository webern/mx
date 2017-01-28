// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Fret.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Fret::Fret()
        :myValue()
        ,myAttributes( std::make_shared<FretAttributes>() )
        {}


        Fret::Fret( const NonNegativeInteger& value )
        :myValue( value )
        ,myAttributes( std::make_shared<FretAttributes>() )
        {}


        bool Fret::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Fret::hasContents() const
        {
            return true;
        }


        std::ostream& Fret::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Fret::streamName( std::ostream& os ) const
        {
            os << "fret";
            return os;
        }


        std::ostream& Fret::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        FretAttributesPtr Fret::getAttributes() const
        {
            return myAttributes;
        }


        void Fret::setAttributes( const FretAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        NonNegativeInteger Fret::getValue() const
        {
            return myValue;
        }


        void Fret::setValue( const NonNegativeInteger& value )
        {
            myValue = value;
        }


        bool Fret::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.parse( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
