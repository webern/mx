// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Cancel.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Cancel::Cancel()
        :myValue()
        ,myAttributes( std::make_shared<CancelAttributes>() )
        {}


        Cancel::Cancel( const FifthsValue& value )
        :myValue( value )
        ,myAttributes( std::make_shared<CancelAttributes>() )
        {}


        bool Cancel::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Cancel::hasContents() const
        {
            return true;
        }


        std::ostream& Cancel::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Cancel::streamName( std::ostream& os ) const
        {
            os << "cancel";
            return os;
        }


        std::ostream& Cancel::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        CancelAttributesPtr Cancel::getAttributes() const
        {
            return myAttributes;
        }


        void Cancel::setAttributes( const CancelAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        FifthsValue Cancel::getValue() const
        {
            return myValue;
        }


        void Cancel::setValue( const FifthsValue& value )
        {
            myValue = value;
        }


        bool Cancel::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.parse( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
