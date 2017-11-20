// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/AccordionMiddle.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        AccordionMiddle::AccordionMiddle()
        :myValue()
        {}


        AccordionMiddle::AccordionMiddle( const AccordionMiddleValue& value )
        :myValue( value )
        {}


        bool AccordionMiddle::hasAttributes() const
        {
            return false;
        }


        bool AccordionMiddle::hasContents() const
        {
            return true;
        }


        std::ostream& AccordionMiddle::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& AccordionMiddle::streamName( std::ostream& os ) const
        {
            os << "accordion-middle";
            return os;
        }


        std::ostream& AccordionMiddle::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        AccordionMiddleValue AccordionMiddle::getValue() const
        {
            return myValue;
        }


        void AccordionMiddle::setValue( const AccordionMiddleValue& value )
        {
            myValue = value;
        }


        bool AccordionMiddle::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
