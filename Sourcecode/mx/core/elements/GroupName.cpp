// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/GroupName.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        GroupName::GroupName()
        :myValue()
        ,myAttributes( std::make_shared<GroupNameAttributes>() )
        {}


        GroupName::GroupName( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<GroupNameAttributes>() )
        {}


        bool GroupName::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool GroupName::hasContents() const
        {
            return true;
        }


        std::ostream& GroupName::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& GroupName::streamName( std::ostream& os ) const
        {
            os << "group-name";
            return os;
        }


        std::ostream& GroupName::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        GroupNameAttributesPtr GroupName::getAttributes() const
        {
            return myAttributes;
        }


        void GroupName::setAttributes( const GroupNameAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString GroupName::getValue() const
        {
            return myValue;
        }


        void GroupName::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool GroupName::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
