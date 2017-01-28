// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Natural.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Natural::Natural() : ElementInterface() {}


        bool Natural::hasAttributes() const { return false; }


        bool Natural::hasContents() const  { return false; }
        std::ostream& Natural::streamAttributes( std::ostream& os ) const { return os; }
        std::ostream& Natural::streamName( std::ostream& os ) const  { os << "natural"; return os; }
        std::ostream& Natural::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        bool Natural::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            return true;
        }

    }
}
