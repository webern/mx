// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/PartName.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        PartName::PartName()
        :myValue()
        ,myAttributes( std::make_shared<PartNameAttributes>() )
        {}


        PartName::PartName( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<PartNameAttributes>() )
        {}


        bool PartName::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool PartName::hasContents() const
        {
            return true;
        }


        std::ostream& PartName::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& PartName::streamName( std::ostream& os ) const
        {
            os << "part-name";
            return os;
        }


        std::ostream& PartName::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        PartNameAttributesPtr PartName::getAttributes() const
        {
            return myAttributes;
        }


        void PartName::setAttributes( const PartNameAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString PartName::getValue() const
        {
            return myValue;
        }


        void PartName::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool PartName::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
