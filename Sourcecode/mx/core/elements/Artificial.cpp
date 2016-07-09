// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Artificial.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Artificial::Artificial() : ElementInterface() {}


        bool Artificial::hasAttributes() const { return false; }


        bool Artificial::hasContents() const  { return false; }
        std::ostream& Artificial::streamAttributes( std::ostream& os ) const { return os; }
        std::ostream& Artificial::streamName( std::ostream& os ) const  { os << "artificial"; return os; }
        std::ostream& Artificial::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        bool Artificial::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            return true;
        }

    }
}
