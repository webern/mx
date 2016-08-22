// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Tap.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Tap::Tap()
        :myValue()
        ,myAttributes( std::make_shared<TapAttributes>() )
        {}


        Tap::Tap( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<TapAttributes>() )
        {}


        bool Tap::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Tap::hasContents() const
        {
            return true;
        }


        std::ostream& Tap::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Tap::streamName( std::ostream& os ) const
        {
            os << "tap";
            return os;
        }


        std::ostream& Tap::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        TapAttributesPtr Tap::getAttributes() const
        {
            return myAttributes;
        }


        void Tap::setAttributes( const TapAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString Tap::getValue() const
        {
            return myValue;
        }


        void Tap::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool Tap::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
