// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/MultipleRest.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        MultipleRest::MultipleRest()
        :myValue()
        ,myAttributes( std::make_shared<MultipleRestAttributes>() )
        {}


        MultipleRest::MultipleRest( const PositiveIntegerOrEmpty& value )
        :myValue( value )
        ,myAttributes( std::make_shared<MultipleRestAttributes>() )
        {}


        bool MultipleRest::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool MultipleRest::hasContents() const
        {
            return true;
        }


        std::ostream& MultipleRest::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& MultipleRest::streamName( std::ostream& os ) const
        {
            os << "multiple-rest";
            return os;
        }


        std::ostream& MultipleRest::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        MultipleRestAttributesPtr MultipleRest::getAttributes() const
        {
            return myAttributes;
        }


        void MultipleRest::setAttributes( const MultipleRestAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        PositiveIntegerOrEmpty MultipleRest::getValue() const
        {
            return myValue;
        }


        void MultipleRest::setValue( const PositiveIntegerOrEmpty& value )
        {
            myValue = value;
        }


        bool MultipleRest::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.parse( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
