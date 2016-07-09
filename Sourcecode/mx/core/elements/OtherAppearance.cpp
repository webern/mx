// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/OtherAppearance.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        OtherAppearance::OtherAppearance()
        :myValue()
        ,myAttributes( std::make_shared<OtherAppearanceAttributes>() )
        {}


        OtherAppearance::OtherAppearance( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<OtherAppearanceAttributes>() )
        {}


        bool OtherAppearance::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool OtherAppearance::hasContents() const
        {
            return true;
        }


        std::ostream& OtherAppearance::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& OtherAppearance::streamName( std::ostream& os ) const
        {
            os << "other-appearance";
            return os;
        }


        std::ostream& OtherAppearance::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        OtherAppearanceAttributesPtr OtherAppearance::getAttributes() const
        {
            return myAttributes;
        }


        void OtherAppearance::setAttributes( const OtherAppearanceAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString OtherAppearance::getValue() const
        {
            return myValue;
        }


        void OtherAppearance::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool OtherAppearance::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
