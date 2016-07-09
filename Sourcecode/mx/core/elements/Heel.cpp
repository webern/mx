// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Heel.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Heel::Heel()
        :myAttributes( std::make_shared<HeelAttributes>() )
        {}


        bool Heel::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& Heel::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& Heel::streamName( std::ostream& os ) const
        {
            os << "heel";
            return os;
        }


        bool Heel::hasContents() const
        {
            return false;
        }


        std::ostream& Heel::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        HeelAttributesPtr Heel::getAttributes() const
        {
            return myAttributes;
        }


        void Heel::setAttributes( const HeelAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool Heel::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
