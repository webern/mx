// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Opus.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Opus::Opus()
        :ElementInterface()
        ,myAttributes( std::make_shared<OpusAttributes>() )
        {}


        bool Opus::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Opus::hasContents() const  { return false; }
        std::ostream& Opus::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Opus::streamName( std::ostream& os ) const  { os << "opus"; return os; }
        std::ostream& Opus::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        OpusAttributesPtr Opus::getAttributes() const
        {
            return myAttributes;
        }


        void Opus::setAttributes( const OpusAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool Opus::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
