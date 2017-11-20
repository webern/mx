// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/OtherPlay.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        OtherPlay::OtherPlay()
        :myValue()
        ,myAttributes( std::make_shared<OtherPlayAttributes>() )
        {}


        OtherPlay::OtherPlay( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<OtherPlayAttributes>() )
        {}


        bool OtherPlay::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool OtherPlay::hasContents() const
        {
            return true;
        }


        std::ostream& OtherPlay::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& OtherPlay::streamName( std::ostream& os ) const
        {
            os << "other-play";
            return os;
        }


        std::ostream& OtherPlay::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        OtherPlayAttributesPtr OtherPlay::getAttributes() const
        {
            return myAttributes;
        }


        void OtherPlay::setAttributes( const OtherPlayAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString OtherPlay::getValue() const
        {
            return myValue;
        }


        void OtherPlay::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool OtherPlay::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            return isSuccess;
        }

    }
}
