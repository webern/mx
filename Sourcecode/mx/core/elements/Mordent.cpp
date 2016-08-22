// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Mordent.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Mordent::Mordent()
        :myAttributes( std::make_shared<MordentAttributes>() )
        {}


        bool Mordent::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& Mordent::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& Mordent::streamName( std::ostream& os ) const
        {
            os << "mordent";
            return os;
        }


        bool Mordent::hasContents() const
        {
            return false;
        }


        std::ostream& Mordent::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        MordentAttributesPtr Mordent::getAttributes() const
        {
            return myAttributes;
        }


        void Mordent::setAttributes( const MordentAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool Mordent::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
