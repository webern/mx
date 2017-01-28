// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/HoleShape.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        HoleShape::HoleShape()
        :myValue()
        {}


        HoleShape::HoleShape( const XsString& value )
        :myValue( value )
        {}


        bool HoleShape::hasAttributes() const
        {
            return false;
        }


        bool HoleShape::hasContents() const
        {
            return true;
        }


        std::ostream& HoleShape::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& HoleShape::streamName( std::ostream& os ) const
        {
            os << "hole-shape";
            return os;
        }


        std::ostream& HoleShape::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        XsString HoleShape::getValue() const
        {
            return myValue;
        }


        void HoleShape::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool HoleShape::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.setValue( xelement.getValue() );
            return true;
        }

    }
}
