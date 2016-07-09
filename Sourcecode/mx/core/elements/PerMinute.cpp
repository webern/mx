// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/PerMinute.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        PerMinute::PerMinute()
        :myValue()
        ,myAttributes( std::make_shared<PerMinuteAttributes>() )
        {}


        PerMinute::PerMinute( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<PerMinuteAttributes>() )
        {}


        bool PerMinute::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool PerMinute::hasContents() const
        {
            return true;
        }


        std::ostream& PerMinute::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& PerMinute::streamName( std::ostream& os ) const
        {
            os << "per-minute";
            return os;
        }


        std::ostream& PerMinute::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        PerMinuteAttributesPtr PerMinute::getAttributes() const
        {
            return myAttributes;
        }


        void PerMinute::setAttributes( const PerMinuteAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString PerMinute::getValue() const
        {
            return myValue;
        }


        void PerMinute::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool PerMinute::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
