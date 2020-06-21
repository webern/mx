// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Humming.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Humming::Humming() : ElementInterface() {}


        bool Humming::hasAttributes() const { return false; }


        bool Humming::hasContents() const  { return false; }
        std::ostream& Humming::streamAttributes( std::ostream& os ) const { return os; }
        std::ostream& Humming::streamName( std::ostream& os ) const  { os << "humming"; return os; }
        std::ostream& Humming::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        bool Humming::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            return true;
        }

    }
}
