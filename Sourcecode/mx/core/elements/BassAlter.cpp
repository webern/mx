// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/BassAlter.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        BassAlter::BassAlter()
        :myValue()
        ,myAttributes( std::make_shared<BassAlterAttributes>() )
        {}


        BassAlter::BassAlter( const Semitones& value )
        :myValue( value )
        ,myAttributes( std::make_shared<BassAlterAttributes>() )
        {}


        bool BassAlter::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool BassAlter::hasContents() const
        {
            return true;
        }


        std::ostream& BassAlter::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& BassAlter::streamName( std::ostream& os ) const
        {
            os << "bass-alter";
            return os;
        }


        std::ostream& BassAlter::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        BassAlterAttributesPtr BassAlter::getAttributes() const
        {
            return myAttributes;
        }


        void BassAlter::setAttributes( const BassAlterAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        Semitones BassAlter::getValue() const
        {
            return myValue;
        }


        void BassAlter::setValue( const Semitones& value )
        {
            myValue = value;
        }


        bool BassAlter::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.parse( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
