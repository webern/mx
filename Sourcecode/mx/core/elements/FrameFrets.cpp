// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/FrameFrets.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        FrameFrets::FrameFrets()
        :myValue()
        {}


        FrameFrets::FrameFrets( const PositiveInteger& value )
        :myValue( value )
        {}


        bool FrameFrets::hasAttributes() const
        {
            return false;
        }


        bool FrameFrets::hasContents() const
        {
            return true;
        }


        std::ostream& FrameFrets::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& FrameFrets::streamName( std::ostream& os ) const
        {
            os << "frame-frets";
            return os;
        }


        std::ostream& FrameFrets::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        PositiveInteger FrameFrets::getValue() const
        {
            return myValue;
        }


        void FrameFrets::setValue( const PositiveInteger& value )
        {
            myValue = value;
        }


        bool FrameFrets::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
