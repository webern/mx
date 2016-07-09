// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/BarStyle.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        BarStyle::BarStyle()
        :myValue( BarStyleEnum::regular )
        ,myAttributes( std::make_shared<BarStyleAttributes>() )
        {}


        BarStyle::BarStyle( const BarStyleEnum& value )
        :myValue( value )
        ,myAttributes( std::make_shared<BarStyleAttributes>() )
        {}


        bool BarStyle::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool BarStyle::hasContents() const
        {
            return true;
        }


        std::ostream& BarStyle::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& BarStyle::streamName( std::ostream& os ) const
        {
            os << "bar-style";
            return os;
        }


        std::ostream& BarStyle::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        BarStyleAttributesPtr BarStyle::getAttributes() const
        {
            return myAttributes;
        }


        void BarStyle::setAttributes( const BarStyleAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        BarStyleEnum BarStyle::getValue() const
        {
            return myValue;
        }


        void BarStyle::setValue( const BarStyleEnum& value )
        {
            myValue = value;
        }


        bool BarStyle::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue = parseBarStyleEnum( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
