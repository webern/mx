// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Falloff.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Falloff::Falloff()
        :ElementInterface()
        ,myAttributes( std::make_shared<EmptyLineAttributes>() )
        {}


        bool Falloff::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Falloff::hasContents() const  { return false; }
        std::ostream& Falloff::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Falloff::streamName( std::ostream& os ) const  { os << "falloff"; return os; }
        std::ostream& Falloff::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        EmptyLineAttributesPtr Falloff::getAttributes() const
        {
            return myAttributes;
        }


        void Falloff::setAttributes( const EmptyLineAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool Falloff::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
