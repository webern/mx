// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/BeatType.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        BeatType::BeatType()
        :myValue()
        {}


        BeatType::BeatType( const XsString& value )
        :myValue( value )
        {}


        bool BeatType::hasAttributes() const
        {
            return false;
        }


        bool BeatType::hasContents() const
        {
            return true;
        }


        std::ostream& BeatType::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& BeatType::streamName( std::ostream& os ) const
        {
            os << "beat-type";
            return os;
        }


        std::ostream& BeatType::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        XsString BeatType::getValue() const
        {
            return myValue;
        }


        void BeatType::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool BeatType::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.setValue( xelement.getValue() );
            return true;
        }

    }
}
