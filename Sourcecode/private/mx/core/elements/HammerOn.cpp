// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/HammerOn.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        HammerOn::HammerOn()
        :myValue()
        ,myAttributes( std::make_shared<HammerOnAttributes>() )
        {}


        HammerOn::HammerOn( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<HammerOnAttributes>() )
        {}


        bool HammerOn::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool HammerOn::hasContents() const
        {
            return true;
        }


        std::ostream& HammerOn::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& HammerOn::streamName( std::ostream& os ) const
        {
            os << "hammer-on";
            return os;
        }


        std::ostream& HammerOn::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        HammerOnAttributesPtr HammerOn::getAttributes() const
        {
            return myAttributes;
        }


        void HammerOn::setAttributes( const HammerOnAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString HammerOn::getValue() const
        {
            return myValue;
        }


        void HammerOn::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool HammerOn::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
