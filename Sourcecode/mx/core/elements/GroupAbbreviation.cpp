// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/GroupAbbreviation.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        GroupAbbreviation::GroupAbbreviation()
        :myValue()
        ,myAttributes( std::make_shared<GroupAbbreviationAttributes>() )
        {}


        GroupAbbreviation::GroupAbbreviation( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<GroupAbbreviationAttributes>() )
        {}


        bool GroupAbbreviation::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool GroupAbbreviation::hasContents() const
        {
            return true;
        }


        std::ostream& GroupAbbreviation::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& GroupAbbreviation::streamName( std::ostream& os ) const
        {
            os << "group-abbreviation";
            return os;
        }


        std::ostream& GroupAbbreviation::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        GroupAbbreviationAttributesPtr GroupAbbreviation::getAttributes() const
        {
            return myAttributes;
        }


        void GroupAbbreviation::setAttributes( const GroupAbbreviationAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString GroupAbbreviation::getValue() const
        {
            return myValue;
        }


        void GroupAbbreviation::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool GroupAbbreviation::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
