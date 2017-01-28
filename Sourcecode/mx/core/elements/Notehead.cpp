// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Notehead.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Notehead::Notehead()
        :myValue( NoteheadValue::normal )
        ,myAttributes( std::make_shared<NoteheadAttributes>() )
        {}


        Notehead::Notehead( const NoteheadValue& value )
        :myValue( value )
        ,myAttributes( std::make_shared<NoteheadAttributes>() )
        {}


        bool Notehead::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Notehead::hasContents() const
        {
            return true;
        }


        std::ostream& Notehead::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Notehead::streamName( std::ostream& os ) const
        {
            os << "notehead";
            return os;
        }


        std::ostream& Notehead::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        NoteheadAttributesPtr Notehead::getAttributes() const
        {
            return myAttributes;
        }


        void Notehead::setAttributes( const NoteheadAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        NoteheadValue Notehead::getValue() const
        {
            return myValue;
        }


        void Notehead::setValue( const NoteheadValue& value )
        {
            myValue = value;
        }


        bool Notehead::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue = parseNoteheadValue( xelement.getValue() );
            return isSuccess;
        }

    }
}
