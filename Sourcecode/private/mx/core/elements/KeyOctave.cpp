// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/KeyOctave.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        KeyOctave::KeyOctave()
        :myValue()
        ,myAttributes( std::make_shared<KeyOctaveAttributes>() )
        {}


        KeyOctave::KeyOctave( const OctaveValue& value )
        :myValue( value )
        ,myAttributes( std::make_shared<KeyOctaveAttributes>() )
        {}


        bool KeyOctave::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool KeyOctave::hasContents() const
        {
            return true;
        }


        std::ostream& KeyOctave::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& KeyOctave::streamName( std::ostream& os ) const
        {
            os << "key-octave";
            return os;
        }


        std::ostream& KeyOctave::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        KeyOctaveAttributesPtr KeyOctave::getAttributes() const
        {
            return myAttributes;
        }


        void KeyOctave::setAttributes( const KeyOctaveAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        OctaveValue KeyOctave::getValue() const
        {
            return myValue;
        }


        void KeyOctave::setValue( const OctaveValue& value )
        {
            myValue = value;
        }


        bool KeyOctave::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.parse( xelement.getValue() );
            return isSuccess;
        }

    }
}
