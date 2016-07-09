// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Pluck.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Pluck::Pluck()
        :myValue()
        ,myAttributes( std::make_shared<PluckAttributes>() )
        {}


        Pluck::Pluck( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<PluckAttributes>() )
        {}


        bool Pluck::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Pluck::hasContents() const
        {
            return true;
        }


        std::ostream& Pluck::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Pluck::streamName( std::ostream& os ) const
        {
            os << "pluck";
            return os;
        }


        std::ostream& Pluck::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        PluckAttributesPtr Pluck::getAttributes() const
        {
            return myAttributes;
        }


        void Pluck::setAttributes( const PluckAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString Pluck::getValue() const
        {
            return myValue;
        }


        void Pluck::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool Pluck::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            return isSuccess;
        }

    }
}
