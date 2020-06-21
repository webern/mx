// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/GroupBarline.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        GroupBarline::GroupBarline()
        :myValue( GroupBarlineValue::yes )
        ,myAttributes( std::make_shared<GroupBarlineAttributes>() )
        {}


        GroupBarline::GroupBarline( const GroupBarlineValue& value )
        :myValue( value )
        ,myAttributes( std::make_shared<GroupBarlineAttributes>() )
        {}


        bool GroupBarline::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool GroupBarline::hasContents() const
        {
            return true;
        }


        std::ostream& GroupBarline::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& GroupBarline::streamName( std::ostream& os ) const
        {
            os << "group-barline";
            return os;
        }


        std::ostream& GroupBarline::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        GroupBarlineAttributesPtr GroupBarline::getAttributes() const
        {
            return myAttributes;
        }


        void GroupBarline::setAttributes( const GroupBarlineAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        GroupBarlineValue GroupBarline::getValue() const
        {
            return myValue;
        }


        void GroupBarline::setValue( const GroupBarlineValue& value )
        {
            myValue = value;
        }


        bool GroupBarline::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue = parseGroupBarlineValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
