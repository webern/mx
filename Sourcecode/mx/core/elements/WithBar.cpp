// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/WithBar.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        WithBar::WithBar()
        :myValue()
        ,myAttributes( std::make_shared<WithBarAttributes>() )
        {}


        WithBar::WithBar( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<WithBarAttributes>() )
        {}


        bool WithBar::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool WithBar::hasContents() const
        {
            return true;
        }


        std::ostream& WithBar::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& WithBar::streamName( std::ostream& os ) const
        {
            os << "with-bar";
            return os;
        }


        std::ostream& WithBar::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        WithBarAttributesPtr WithBar::getAttributes() const
        {
            return myAttributes;
        }


        void WithBar::setAttributes( const WithBarAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString WithBar::getValue() const
        {
            return myValue;
        }


        void WithBar::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool WithBar::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
