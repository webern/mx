// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Slide.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Slide::Slide()
        :myValue()
        ,myAttributes( std::make_shared<SlideAttributes>() )
        {}


        Slide::Slide( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<SlideAttributes>() )
        {}


        bool Slide::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Slide::hasContents() const
        {
            return true;
        }


        std::ostream& Slide::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Slide::streamName( std::ostream& os ) const
        {
            os << "slide";
            return os;
        }


        std::ostream& Slide::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        SlideAttributesPtr Slide::getAttributes() const
        {
            return myAttributes;
        }


        void Slide::setAttributes( const SlideAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString Slide::getValue() const
        {
            return myValue;
        }


        void Slide::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool Slide::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
