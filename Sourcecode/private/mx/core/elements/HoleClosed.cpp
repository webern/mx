// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/HoleClosed.h"
#include "mx/xml/XElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        HoleClosed::HoleClosed()
        :myValue( HoleClosedValue::no )
        ,myAttributes( std::make_shared<HoleClosedAttributes>() )
        {}


        HoleClosed::HoleClosed( const HoleClosedValue& value )
        :myValue( value )
        ,myAttributes( std::make_shared<HoleClosedAttributes>() )
        {}


        bool HoleClosed::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool HoleClosed::hasContents() const
        {
            return true;
        }


        std::ostream& HoleClosed::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& HoleClosed::streamName( std::ostream& os ) const
        {
            os << "hole-closed";
            return os;
        }


        std::ostream& HoleClosed::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        HoleClosedAttributesPtr HoleClosed::getAttributes() const
        {
            return myAttributes;
        }


        void HoleClosed::setAttributes( const HoleClosedAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        HoleClosedValue HoleClosed::getValue() const
        {
            return myValue;
        }


        void HoleClosed::setValue( const HoleClosedValue& value )
        {
            myValue = value;
        }
        
        
        bool HoleClosed::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue = parseHoleClosedValue( xelement.getValue() );
            return true;
        }
    }
}
