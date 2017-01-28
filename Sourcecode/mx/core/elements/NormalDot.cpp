// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/NormalDot.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        NormalDot::NormalDot() : ElementInterface() {}


        bool NormalDot::hasAttributes() const { return false; }


        bool NormalDot::hasContents() const  { return false; }
        std::ostream& NormalDot::streamAttributes( std::ostream& os ) const { return os; }
        std::ostream& NormalDot::streamName( std::ostream& os ) const  { os << "normal-dot"; return os; }
        std::ostream& NormalDot::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        bool NormalDot::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            return true;
        }

    }
}
