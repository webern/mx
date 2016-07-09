// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/FrameStrings.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        FrameStrings::FrameStrings()
        :myValue()
        {}


        FrameStrings::FrameStrings( const PositiveInteger& value )
        :myValue( value )
        {}


        bool FrameStrings::hasAttributes() const
        {
            return false;
        }


        bool FrameStrings::hasContents() const
        {
            return true;
        }


        std::ostream& FrameStrings::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& FrameStrings::streamName( std::ostream& os ) const
        {
            os << "frame-strings";
            return os;
        }


        std::ostream& FrameStrings::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        PositiveInteger FrameStrings::getValue() const
        {
            return myValue;
        }


        void FrameStrings::setValue( const PositiveInteger& value )
        {
            myValue = value;
        }


        bool FrameStrings::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.parse( xelement.getValue() );
            return true;
        }

    }
}
