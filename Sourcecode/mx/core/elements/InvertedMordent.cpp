// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/InvertedMordent.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        InvertedMordent::InvertedMordent()
        :myAttributes( std::make_shared<InvertedMordentAttributes>() )
        {}


        bool InvertedMordent::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& InvertedMordent::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& InvertedMordent::streamName( std::ostream& os ) const
        {
            os << "inverted-mordent";
            return os;
        }


        bool InvertedMordent::hasContents() const
        {
            return false;
        }


        std::ostream& InvertedMordent::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        InvertedMordentAttributesPtr InvertedMordent::getAttributes() const
        {
            return myAttributes;
        }


        void InvertedMordent::setAttributes( const InvertedMordentAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }
        
        
        bool InvertedMordent::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
