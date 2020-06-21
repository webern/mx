// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/TupletNumber.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        TupletNumber::TupletNumber()
        :myValue()
        ,myAttributes( std::make_shared<TupletNumberAttributes>() )
        {}


        TupletNumber::TupletNumber( const NonNegativeInteger& value )
        :myValue( value )
        ,myAttributes( std::make_shared<TupletNumberAttributes>() )
        {}


        bool TupletNumber::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool TupletNumber::hasContents() const
        {
            return true;
        }


        std::ostream& TupletNumber::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& TupletNumber::streamName( std::ostream& os ) const
        {
            os << "tuplet-number";
            return os;
        }


        std::ostream& TupletNumber::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        TupletNumberAttributesPtr TupletNumber::getAttributes() const
        {
            return myAttributes;
        }


        void TupletNumber::setAttributes( const TupletNumberAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        NonNegativeInteger TupletNumber::getValue() const
        {
            return myValue;
        }


        void TupletNumber::setValue( const NonNegativeInteger& value )
        {
            myValue = value;
        }


        bool TupletNumber::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.parse( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
