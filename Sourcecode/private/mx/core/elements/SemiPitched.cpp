// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/SemiPitched.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        SemiPitched::SemiPitched()
        :myValue( SemiPitchedEnum::medium )
        {}


        SemiPitched::SemiPitched( const SemiPitchedEnum& value )
        :myValue( value )
        {}


        bool SemiPitched::hasAttributes() const
        {
            return false;
        }


        bool SemiPitched::hasContents() const
        {
            return true;
        }


        std::ostream& SemiPitched::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& SemiPitched::streamName( std::ostream& os ) const
        {
            os << "semi-pitched";
            return os;
        }


        std::ostream& SemiPitched::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        SemiPitchedEnum SemiPitched::getValue() const
        {
            return myValue;
        }


        void SemiPitched::setValue( const SemiPitchedEnum& value )
        {
            myValue = value;
        }


        bool SemiPitched::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            myValue = parseSemiPitchedEnum( xelement.getValue() );
            return true;
        }

    }
}
