// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/PartAbbreviation.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        PartAbbreviation::PartAbbreviation()
        :myValue()
        ,myAttributes( std::make_shared<PartAbbreviationAttributes>() )
        {}


        PartAbbreviation::PartAbbreviation( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<PartAbbreviationAttributes>() )
        {}


        bool PartAbbreviation::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool PartAbbreviation::hasContents() const
        {
            return true;
        }


        std::ostream& PartAbbreviation::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& PartAbbreviation::streamName( std::ostream& os ) const
        {
            os << "part-abbreviation";
            return os;
        }


        std::ostream& PartAbbreviation::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        PartAbbreviationAttributesPtr PartAbbreviation::getAttributes() const
        {
            return myAttributes;
        }


        void PartAbbreviation::setAttributes( const PartAbbreviationAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString PartAbbreviation::getValue() const
        {
            return myValue;
        }


        void PartAbbreviation::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool PartAbbreviation::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            return isSuccess;
        }

    }
}
