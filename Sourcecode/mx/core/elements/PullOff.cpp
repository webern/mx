// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/PullOff.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        PullOff::PullOff()
        :myValue()
        ,myAttributes( std::make_shared<PullOffAttributes>() )
        {}


        PullOff::PullOff( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<PullOffAttributes>() )
        {}


        bool PullOff::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool PullOff::hasContents() const
        {
            return true;
        }


        std::ostream& PullOff::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& PullOff::streamName( std::ostream& os ) const
        {
            os << "pull-off";
            return os;
        }


        std::ostream& PullOff::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        PullOffAttributesPtr PullOff::getAttributes() const
        {
            return myAttributes;
        }


        void PullOff::setAttributes( const PullOffAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString PullOff::getValue() const
        {
            return myValue;
        }


        void PullOff::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool PullOff::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
