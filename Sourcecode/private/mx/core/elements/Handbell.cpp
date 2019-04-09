// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Handbell.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Handbell::Handbell()
        :myValue( HandbellValue::damp )
        ,myAttributes( std::make_shared<HandbellAttributes>() )
        {}


        Handbell::Handbell( const HandbellValue& value )
        :myValue( value )
        ,myAttributes( std::make_shared<HandbellAttributes>() )
        {}


        bool Handbell::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Handbell::hasContents() const
        {
            return true;
        }


        std::ostream& Handbell::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Handbell::streamName( std::ostream& os ) const
        {
            os << "handbell";
            return os;
        }


        std::ostream& Handbell::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        HandbellAttributesPtr Handbell::getAttributes() const
        {
            return myAttributes;
        }


        void Handbell::setAttributes( const HandbellAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        HandbellValue Handbell::getValue() const
        {
            return myValue;
        }


        void Handbell::setValue( const HandbellValue& value )
        {
            myValue = value;
        }


        bool Handbell::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue = parseHandbellValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
