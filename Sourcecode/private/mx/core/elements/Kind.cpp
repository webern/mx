// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Kind.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Kind::Kind()
        :myValue( KindValue::none )
        ,myAttributes( std::make_shared<KindAttributes>() )
        {}


        Kind::Kind( const KindValue& value )
        :myValue( value )
        ,myAttributes( std::make_shared<KindAttributes>() )
        {}


        bool Kind::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Kind::hasContents() const
        {
            return true;
        }


        std::ostream& Kind::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Kind::streamName( std::ostream& os ) const
        {
            os << "kind";
            return os;
        }


        std::ostream& Kind::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        KindAttributesPtr Kind::getAttributes() const
        {
            return myAttributes;
        }


        void Kind::setAttributes( const KindAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        KindValue Kind::getValue() const
        {
            return myValue;
        }


        void Kind::setValue( const KindValue& value )
        {
            myValue = value;
        }


        bool Kind::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue = parseKindValue( xelement.getValue() );
            return isSuccess;
        }

    }
}
