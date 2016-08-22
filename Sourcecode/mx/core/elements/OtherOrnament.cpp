// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/OtherOrnament.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        OtherOrnament::OtherOrnament()
        :myValue()
        ,myAttributes( std::make_shared<OtherOrnamentAttributes>() )
        {}


        OtherOrnament::OtherOrnament( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<OtherOrnamentAttributes>() )
        {}


        bool OtherOrnament::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool OtherOrnament::hasContents() const
        {
            return true;
        }


        std::ostream& OtherOrnament::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& OtherOrnament::streamName( std::ostream& os ) const
        {
            os << "other-ornament";
            return os;
        }


        std::ostream& OtherOrnament::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        OtherOrnamentAttributesPtr OtherOrnament::getAttributes() const
        {
            return myAttributes;
        }


        void OtherOrnament::setAttributes( const OtherOrnamentAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString OtherOrnament::getValue() const
        {
            return myValue;
        }


        void OtherOrnament::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool OtherOrnament::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
