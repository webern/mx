// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/CircularArrow.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        CircularArrow::CircularArrow()
        :myValue( CircularArrowEnum::clockwise )
        {}


        CircularArrow::CircularArrow( const CircularArrowEnum& value )
        :myValue( value )
        {}


        bool CircularArrow::hasAttributes() const
        {
            return false;
        }


        bool CircularArrow::hasContents() const
        {
            return true;
        }


        std::ostream& CircularArrow::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& CircularArrow::streamName( std::ostream& os ) const
        {
            os << "circular-arrow";
            return os;
        }


        std::ostream& CircularArrow::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        CircularArrowEnum CircularArrow::getValue() const
        {
            return myValue;
        }


        void CircularArrow::setValue( const CircularArrowEnum& value )
        {
            myValue = value;
        }


        bool CircularArrow::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            myValue = parseCircularArrowEnum( xelement.getValue() );
            return true;
        }

    }
}
