// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/StrongAccent.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        StrongAccent::StrongAccent()
        :myAttributes( std::make_shared<StrongAccentAttributes>() )
        {}


        bool StrongAccent::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& StrongAccent::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& StrongAccent::streamName( std::ostream& os ) const
        {
            os << "strong-accent";
            return os;
        }


        bool StrongAccent::hasContents() const
        {
            return false;
        }


        std::ostream& StrongAccent::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        StrongAccentAttributesPtr StrongAccent::getAttributes() const
        {
            return myAttributes;
        }


        void StrongAccent::setAttributes( const StrongAccentAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool StrongAccent::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
