// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Creator.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Creator::Creator()
        :myValue()
        ,myAttributes( std::make_shared<CreatorAttributes>() )
        {}


        Creator::Creator( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<CreatorAttributes>() )
        {}


        bool Creator::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Creator::hasContents() const
        {
            return true;
        }


        std::ostream& Creator::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Creator::streamName( std::ostream& os ) const
        {
            os << "creator";
            return os;
        }


        std::ostream& Creator::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        CreatorAttributesPtr Creator::getAttributes() const
        {
            return myAttributes;
        }


        void Creator::setAttributes( const CreatorAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString Creator::getValue() const
        {
            return myValue;
        }


        void Creator::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool Creator::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            return isSuccess;
        }

    }
}
