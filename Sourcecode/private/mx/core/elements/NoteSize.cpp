// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/NoteSize.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        NoteSize::NoteSize()
        :myValue()
        ,myAttributes( std::make_shared<NoteSizeAttributes>() )
        {}


        NoteSize::NoteSize( const NonNegativeDecimal& value )
        :myValue( value )
        ,myAttributes( std::make_shared<NoteSizeAttributes>() )
        {}


        bool NoteSize::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool NoteSize::hasContents() const
        {
            return true;
        }


        std::ostream& NoteSize::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& NoteSize::streamName( std::ostream& os ) const
        {
            os << "note-size";
            return os;
        }


        std::ostream& NoteSize::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        NoteSizeAttributesPtr NoteSize::getAttributes() const
        {
            return myAttributes;
        }


        void NoteSize::setAttributes( const NoteSizeAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        NonNegativeDecimal NoteSize::getValue() const
        {
            return myValue;
        }


        void NoteSize::setValue( const NonNegativeDecimal& value )
        {
            myValue = value;
        }


        bool NoteSize::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.parse( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
