// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/SlashDot.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        SlashDot::SlashDot() : ElementInterface() {}


        bool SlashDot::hasAttributes() const { return false; }


        bool SlashDot::hasContents() const  { return false; }
        std::ostream& SlashDot::streamAttributes( std::ostream& os ) const { return os; }
        std::ostream& SlashDot::streamName( std::ostream& os ) const  { os << "slash-dot"; return os; }
        std::ostream& SlashDot::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        bool SlashDot::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            return true;
        }

    }
}
