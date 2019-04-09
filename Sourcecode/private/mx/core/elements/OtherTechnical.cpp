// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/OtherTechnical.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        OtherTechnical::OtherTechnical()
        :myValue()
        ,myAttributes( std::make_shared<OtherTechnicalAttributes>() )
        {}


        OtherTechnical::OtherTechnical( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<OtherTechnicalAttributes>() )
        {}


        bool OtherTechnical::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool OtherTechnical::hasContents() const
        {
            return true;
        }


        std::ostream& OtherTechnical::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& OtherTechnical::streamName( std::ostream& os ) const
        {
            os << "other-technical";
            return os;
        }


        std::ostream& OtherTechnical::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        OtherTechnicalAttributesPtr OtherTechnical::getAttributes() const
        {
            return myAttributes;
        }


        void OtherTechnical::setAttributes( const OtherTechnicalAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString OtherTechnical::getValue() const
        {
            return myValue;
        }


        void OtherTechnical::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool OtherTechnical::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
