// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/OtherNotation.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        OtherNotation::OtherNotation()
        :myValue()
        ,myAttributes( std::make_shared<OtherNotationAttributes>() )
        {}


        OtherNotation::OtherNotation( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<OtherNotationAttributes>() )
        {}


        bool OtherNotation::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool OtherNotation::hasContents() const
        {
            return true;
        }


        std::ostream& OtherNotation::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& OtherNotation::streamName( std::ostream& os ) const
        {
            os << "other-notation";
            return os;
        }


        std::ostream& OtherNotation::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        OtherNotationAttributesPtr OtherNotation::getAttributes() const
        {
            return myAttributes;
        }


        void OtherNotation::setAttributes( const OtherNotationAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString OtherNotation::getValue() const
        {
            return myValue;
        }


        void OtherNotation::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool OtherNotation::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
