// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/TupletType.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        TupletType::TupletType()
        :myValue( NoteTypeValue::eighth )
        ,myAttributes( std::make_shared<TupletTypeAttributes>() )
        {}


        TupletType::TupletType( const NoteTypeValue& value )
        :myValue( value )
        ,myAttributes( std::make_shared<TupletTypeAttributes>() )
        {}


        bool TupletType::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool TupletType::hasContents() const
        {
            return true;
        }


        std::ostream& TupletType::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& TupletType::streamName( std::ostream& os ) const
        {
            os << "tuplet-type";
            return os;
        }


        std::ostream& TupletType::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        TupletTypeAttributesPtr TupletType::getAttributes() const
        {
            return myAttributes;
        }


        void TupletType::setAttributes( const TupletTypeAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        NoteTypeValue TupletType::getValue() const
        {
            return myValue;
        }


        void TupletType::setValue( const NoteTypeValue& value )
        {
            myValue = value;
        }


        bool TupletType::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue = parseNoteTypeValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
