// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Distance.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Distance::Distance()
        :myValue()
        ,myAttributes( std::make_shared<DistanceAttributes>() )
        {}


        Distance::Distance( const TenthsValue& value )
        :myValue( value )
        ,myAttributes( std::make_shared<DistanceAttributes>() )
        {}


        bool Distance::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Distance::hasContents() const
        {
            return true;
        }


        std::ostream& Distance::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Distance::streamName( std::ostream& os ) const
        {
            os << "distance";
            return os;
        }


        std::ostream& Distance::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        DistanceAttributesPtr Distance::getAttributes() const
        {
            return myAttributes;
        }


        void Distance::setAttributes( const DistanceAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        TenthsValue Distance::getValue() const
        {
            return myValue;
        }


        void Distance::setValue( const TenthsValue& value )
        {
            myValue = value;
        }


        bool Distance::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.parse( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
