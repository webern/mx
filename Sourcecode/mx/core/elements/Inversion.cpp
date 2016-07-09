// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Inversion.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Inversion::Inversion()
        :myValue()
        ,myAttributes( std::make_shared<InversionAttributes>() )
        {}


        Inversion::Inversion( const NonNegativeInteger& value )
        :myValue( value )
        ,myAttributes( std::make_shared<InversionAttributes>() )
        {}


        bool Inversion::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Inversion::hasContents() const
        {
            return true;
        }


        std::ostream& Inversion::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Inversion::streamName( std::ostream& os ) const
        {
            os << "inversion";
            return os;
        }


        std::ostream& Inversion::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        InversionAttributesPtr Inversion::getAttributes() const
        {
            return myAttributes;
        }


        void Inversion::setAttributes( const InversionAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        NonNegativeInteger Inversion::getValue() const
        {
            return myValue;
        }


        void Inversion::setValue( const NonNegativeInteger& value )
        {
            myValue = value;
        }


        bool Inversion::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.parse( xelement.getValue() );
            return isSuccess;
        }

    }
}
