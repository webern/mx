// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/BeatUnit.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        BeatUnit::BeatUnit()
        :myValue( NoteTypeValue::eighth )
        {}


        BeatUnit::BeatUnit( const NoteTypeValue& value )
        :myValue( value )
        {}


        bool BeatUnit::hasAttributes() const
        {
            return false;
        }


        bool BeatUnit::hasContents() const
        {
            return true;
        }


        std::ostream& BeatUnit::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& BeatUnit::streamName( std::ostream& os ) const
        {
            os << "beat-unit";
            return os;
        }


        std::ostream& BeatUnit::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        NoteTypeValue BeatUnit::getValue() const
        {
            return myValue;
        }


        void BeatUnit::setValue( const NoteTypeValue& value )
        {
            myValue = value;
        }


        bool BeatUnit::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            myValue = parseNoteTypeValue( xelement.getValue() );
            return true;
        }

    }
}
