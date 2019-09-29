// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Offset.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Offset::Offset()
        :myValue()
        ,myAttributes( std::make_shared<OffsetAttributes>() )
        {}


        Offset::Offset( const DivisionsValue& value )
        :myValue( value )
        ,myAttributes( std::make_shared<OffsetAttributes>() )
        {}


        bool Offset::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Offset::hasContents() const
        {
            return true;
        }


        std::ostream& Offset::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Offset::streamName( std::ostream& os ) const
        {
            os << "offset";
            return os;
        }


        std::ostream& Offset::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        OffsetAttributesPtr Offset::getAttributes() const
        {
            return myAttributes;
        }


        void Offset::setAttributes( const OffsetAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        DivisionsValue Offset::getValue() const
        {
            return myValue;
        }


        void Offset::setValue( const DivisionsValue& value )
        {
            myValue = value;
        }


        bool Offset::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.parse( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
